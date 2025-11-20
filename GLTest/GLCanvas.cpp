#include "pch.h"
#include "GLCanvas.h"

constexpr int CENTER_CURSOR_THRESHOLD = 5;

typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);

HMODULE libGLHandle = LoadLibraryA("opengl32.dll");

static void* GLGetProcAddress(LPCSTR name) {
	void* ptr = (void*)wglGetProcAddress(name);
	if (!ptr) {
		ptr = (void*)GetProcAddress(libGLHandle, name);
	}
	return ptr;
}

static char GetShiftASCII(char c)
{
	switch (c)
	{
	case '1': return '!';
	case '2': return '@';
	case '3': return '#';
	case '4': return '$';
	case '5': return '%';
	case '6': return '^';
	case '7': return '&';
	case '8': return '*';
	case '9': return '(';
	case '0': return ')';
	case '-': return '_';
	case '=': return '+';
	case '[': return '{';
	case ']': return '}';
	case '\\': return '|';
	case ';': return ':';
	case '\'': return '"';
	case ',': return '<';
	case '.': return '>';
	case '/': return '?';
	default:
		return c;
	}
}

static char GetKeyChar(Keys key, Keys mods)
{
	if ((int)key < 0x20 || (int)key > 0x7F)
		return '\0';

	char c = (char)key;
	bool hasCaps = mods.HasFlag(Keys::CapsLock);
	bool hasShift = mods.HasFlag(Keys::ShiftKey);

	if (c >= 'A' && c <= 'Z')
	{
		if ((!hasCaps && !hasShift) || (hasCaps && hasShift))
			c = (char)((int)key + 32);
	}
	else if (hasShift)
		c = GetShiftASCII(c);

	return c;
}

static int MapMouseButton(MouseButtons btn) {
	switch (btn) {
		case MouseButtons::Left: return 0;
		case MouseButtons::Right: return 1;
		case MouseButtons::Middle: return 2;
		default: return -1;
	}
}

GLCanvas::GLCanvas(IGame^ game)
{
	this->game = game;
	this->keys = gcnew array<bool>(256);
	SetStyle(ControlStyles::ContainerControl, false);
	Application::Idle += gcnew EventHandler(this, &GLCanvas::OnApplicationIdle);
}

void GLCanvas::OnHandleCreated(EventArgs^ e) {
	UserControl::OnHandleCreated(e);
	this->deviceContext = GetDC(CastPtr<HWND>(this->Handle));

	PIXELFORMATDESCRIPTOR pixelFormat{};
	pixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormat.nVersion = 1;
	pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.cColorBits = 32;
	pixelFormat.cDepthBits = 24;

	int format = ChoosePixelFormat(this->deviceContext, &pixelFormat);
	if (!format)
		throw gcnew Exception("Failed to choose pixel format");
	if (!SetPixelFormat(this->deviceContext, format, &pixelFormat))
		throw gcnew Exception("Failed to set pixel format");

	this->glContext = wglCreateContext(this->deviceContext);
	if (!this->glContext)
		throw gcnew Exception("Failed to create OpenGL context");
	if (!wglMakeCurrent(this->deviceContext, this->glContext))
		throw gcnew Exception("Failed to make OpenGL context current");
	gladLoadGLLoader(GLGetProcAddress);

	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT) {
		wglSwapIntervalEXT(0);
	}

	this->game->Init(this);
}

void GLCanvas::OnHandleDestroyed(EventArgs^ e) {
	UserControl::OnHandleDestroyed(e);
	this->game->Destroy();

	if (this->glContext) {
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(this->glContext);
		this->glContext = nullptr;
	}

	if (this->deviceContext) {
		ReleaseDC(CastPtr<HWND>(this->Handle), this->deviceContext);
		this->deviceContext = nullptr;
	}
}

void GLCanvas::OnPaint(PaintEventArgs^ e)
{
	if (!this->deviceContext || !this->glContext) {
		e->Graphics->Clear(Color::Black);
		e->Graphics->DrawString("No OpenGL context", this->Font, Brushes::White, PointF(0, 0));
		return;
	}
}

void GLCanvas::OnApplicationIdle(Object^ sender, EventArgs^ e) {
	// this function is only called after all events have been processed,
	// so we loop till there is an event, this basically creates our main loop
	MSG msg;
	while (!PeekMessage(&msg, NULL, 0, 0, 0)) {
		if (!this->deviceContext || !this->glContext)
			return;
		this->HandleMouseCapture();
		this->game->Draw();
		SwapBuffers(this->deviceContext);
	}
}

void GLCanvas::OnPreviewKeyDown(PreviewKeyDownEventArgs^ e) {
	e->IsInputKey = true;
}

void GLCanvas::OnKeyDown(KeyEventArgs^ e) {
	char c = GetKeyChar(e->KeyCode, e->Modifiers);
	if (!this->keys[e->KeyValue]) {
		this->keys[e->KeyValue] = true;
		this->game->OnKeyState(e->KeyValue, c, true, false);
	} else if (this->AllowRepeatKeys) {
		this->game->OnKeyState(e->KeyValue, c, true, true);
	}
}

void GLCanvas::OnKeyUp(KeyEventArgs^ e) {
	this->keys[e->KeyValue] = false;
	this->game->OnKeyState(e->KeyValue, '\0', false, false);
}

void GLCanvas::OnMouseDown(MouseEventArgs^ e) {
	UserControl::OnMouseDown(e);
	this->game->OnMouseButton(MapMouseButton(e->Button), true);
}

void GLCanvas::OnMouseUp(MouseEventArgs^ e) {
	UserControl::OnMouseUp(e);
	this->game->OnMouseButton(MapMouseButton(e->Button), false);
}

void GLCanvas::OnMouseMove(MouseEventArgs^ e) {
	if (this->mouseSkipPos) {
		this->mouseSkipPos--;
		return;
	}

	float mx = (float)e->X;
	float my = (float)e->Y;
	float xd = mx - this->mouseX;
	float yd = my - this->mouseY;
	this->mouseX = mx;
	this->mouseY = my;

	if (this->mouseCaptured && this->Focused) {
		int left = CENTER_CURSOR_THRESHOLD - e->X;
		int top = CENTER_CURSOR_THRESHOLD - e->Y;
		int right = e->X - (this->ClientSize.Width - CENTER_CURSOR_THRESHOLD);
		int bottom = e->Y - (this->ClientSize.Height - CENTER_CURSOR_THRESHOLD);
		bool oob = left > 0 || top > 0 || right > 0 || bottom > 0;

		if (oob) {
			//Console::WriteLine("OOB {0} {1} {2} {3}", left, top, right, bottom);
			Point center = Point(this->ClientSize.Width / 2, this->ClientSize.Height / 2);
			this->mouseX = (float)center.X;
			this->mouseY = (float)center.Y;
			xd = (float)Math::Max(0, Math::Max(left, right));
			yd = (float)Math::Max(0, Math::Max(top, bottom));
			this->Cursor->Position = this->PointToScreen(center);
			this->mouseSkipPos = 2;
		}
	}

	this->game->OnMouseMove(this->mouseX, this->mouseY, xd, yd);
}

void GLCanvas::OnResize(EventArgs^ e) {
	UserControl::OnResize(e);
	if (!this->deviceContext || !this->glContext)
		return;
	this->game->OnResize(this->ClientSize.Width, this->ClientSize.Height);
}

void GLCanvas::SetMouseCapture(bool capture) {
	this->mouseCaptured = capture;
	this->Focus();
	this->Cursor = Cursors::Default;
	this->HandleMouseCapture();
	if (capture) {
		this->Cursor->Hide();
	}
	else {
		this->Cursor->Show();
	}
}

void GLCanvas::HandleMouseCapture() {
	this->Capture = this->mouseCaptured;
	if (this->mouseCaptured && this->Focused) {
		this->Cursor->Clip = RectangleToScreen(Drawing::Rectangle(Point::Empty, ClientSize));
	}
	else {
		this->Cursor->Clip = Rectangle::Empty;
	}
}

bool GLCanvas::GetKeyState(int key) {
	if (key < 0 || key >= this->keys->Length)
		return false;
	return this->keys[key];
}

void GLCanvas::OnLostFocus(EventArgs^ e) {
	UserControl::OnLostFocus(e);
	this->game->OnMouseButton(0, false);
	this->game->OnMouseButton(1, false);
	this->game->OnMouseButton(2, false);
	for (int i = 0; i < this->keys->Length; i++) {
		if (this->keys[i]) {
			this->keys[i] = false;
			this->game->OnKeyState(i, '\0', false, false);
		}
	}
}