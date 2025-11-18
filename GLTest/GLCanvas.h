#pragma once

#include "pch.h"
#include "IGame.h"
#include "IGameEngine.h"
#include <glad/glad.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public ref class GLCanvas : public UserControl, public IGameEngine
{
private:
	HDC deviceContext;
	HGLRC glContext;
	bool mouseCaptured;
	float mouseX;
	float mouseY;
	array<bool>^ keys;
	IGame^ game;

public:
	GLCanvas(IGame^ game);

protected:
	void OnHandleCreated(EventArgs^ e) override;

	void OnHandleDestroyed(EventArgs^ e) override;

	void OnPaintBackground(PaintEventArgs^ e) override {}

	void OnPaint(PaintEventArgs^ e) override;

	void OnPreviewKeyDown(PreviewKeyDownEventArgs^ e) override;

	void OnKeyDown(KeyEventArgs^ e) override;

	void OnKeyUp(KeyEventArgs^ e) override;

	void OnMouseDown(MouseEventArgs^ e) override;

	void OnMouseUp(MouseEventArgs^ e) override;

	void OnMouseMove(MouseEventArgs^ e) override;

	void OnResize(EventArgs^ e) override;

private:
	void OnApplicationIdle(Object^ sender, EventArgs^ e);

	void HandleMouseCapture();

public:
	virtual property bool AllowRepeatKeys;

	virtual property int Width { int get() { return this->ClientSize.Width; } }

	virtual property int Height { int get() { return this->ClientSize.Height; } }

	virtual property float MouseX { float get() { return this->mouseX; } }

	virtual property float MouseY { float get() { return this->mouseY; } }

	virtual property bool IsFocused { bool get() { return this->Focused; } }

	virtual void SetMouseCapture(bool capture);

	virtual bool GetKeyState(int key);
};