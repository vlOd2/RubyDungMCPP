#pragma once

public interface struct IGameEngine {
	property bool AllowRepeatKeys;

	property int Width { int get(); }

	property int Height { int get(); }

	property float MouseX { float get(); }

	property float MouseY { float get(); }

	property bool IsFocused { bool get(); }

	void SetMouseCapture(bool capture);

	bool GetKeyState(int key);
};