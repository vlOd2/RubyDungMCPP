#pragma once

#include "IGameEngine.h"

public interface struct IGame {
	void Init(IGameEngine^ engine);

	void OnResize(int width, int height);

	void OnKeyState(int key, char c, bool state, bool repeat);

	void OnMouseMove(float x, float y, float xd, float yd);

	void OnMouseButton(int btn, bool state);

	void Draw();

	void Destroy();
};