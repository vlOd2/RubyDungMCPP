#pragma once

#include "pch.h"
#include "IGame.h"
#include <glad/glad.h>
#include "RDTimer.h"
#include "RDLevel.h"
#include "RDLevelRenderer.h"
#include "RDPlayer.h"
#include "RDHitResult.h"
#include "RDTextures.h"

using namespace System;
using namespace System::Numerics;

public ref class Game : public IGame
{
private:
	IGameEngine^ engine;
	int width;
	int height;
	RDTimer^ timer = gcnew RDTimer(60.0F);
	RDLevel^ level;
	RDLevelRenderer^ levelRenderer;
	RDPlayer^ player;
	RDHitResult^ hitResult;
	long long lastTime = RDTimer::MilliTime();
	int frames = 0;
	float* fogColor;
	bool mouseCaptured;

	void Tick();

	void Render(float a);

	void MoveCameraToPlayer(float a);

	void SetupCamera(float a);

	void SetupPickCamera(float a, int x, int y);

public:
	Game();

	virtual void Init(IGameEngine^ engine);

	virtual void OnResize(int width, int height);

	virtual void OnKeyState(int key, char c, bool state, bool repeat);

	virtual void OnMouseMove(float x, float y, float xd, float yd);

	virtual void OnMouseButton(int btn, bool state);

	virtual void Draw();
};
