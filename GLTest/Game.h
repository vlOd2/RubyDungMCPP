#pragma once

#include "pch.h"
#include "IGame.h"
#include <glad/glad.h>

using namespace System;

public ref class Game : public IGame
{
private:
	IGameEngine^ engine;

public:
	Game();

	virtual void Init(IGameEngine^ engine);

	virtual void OnResize(int width, int height);

	virtual void OnKeyState(int key, char c, bool state, bool repeat);

	virtual void OnMouseMove(float x, float y, float xd, float yd);

	virtual void OnMouseButton(int btn, bool state);

	virtual void Draw();
};
