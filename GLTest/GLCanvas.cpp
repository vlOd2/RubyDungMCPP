#include "pch.h"
#include "GLCanvas.h"

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

	this->game->Init(this);
}

void GLCanvas::OnHandleDestroyed(EventArgs^ e) {
	UserControl::OnHandleDestroyed(e);
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
	float mx = (float)e->X;
	float my = (float)e->Y;
	float xd = mx - this->mouseX;
	float yd = my - this->mouseY;
	this->mouseX = mx;
	this->mouseY = my;
	this->game->OnMouseMove(this->mouseX, this->mouseY, xd, yd);
	if (this->mouseCaptured && this->Focused) {
		// warp mouse when close to edge
		if (e->X < 5 || e->X >= this->ClientSize.Width - 5 ||
			e->Y < 5 || e->Y >= this->ClientSize.Height - 5) {

		}
	}
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
}

void GLCanvas::HandleMouseCapture() {
	this->Capture = this->mouseCaptured;
	if (this->mouseCaptured && this->Focused) {
		this->Cursor->Clip = RectangleToScreen(Drawing::Rectangle(Point::Empty, ClientSize));
		//this->Cursor->Hide();
	}
	else {
		this->Cursor->Clip = Rectangle::Empty;
		//this->Cursor->Show();
	}
}

bool GLCanvas::GetKeyState(int key) {
	if (key < 0 || key >= this->keys->Length)
		return false;
	return this->keys[key];
}