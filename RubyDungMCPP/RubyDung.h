#pragma once

#include "pch.h"
#include "IGame.h"
#include "RDTimer.h"
#include "RDLevel.h"
#include "RDLevelRenderer.h"
#include "RDPlayer.h"
#include "RDHitResult.h"
#include "RDTextures.h"
#include <glad/glad.h>
#include <gl/GLU.h>

using namespace System;

public ref class RubyDung : public IGame
{
private:
	IGameEngine^ engine;
	int width;
	int height;
	float* fogColor;
	RDTimer^ timer = gcnew RDTimer(60.0F);
	RDLevel^ level;
	RDLevelRenderer^ levelRenderer;
	RDPlayer^ player;
	RDHitResult^ hitResult;
	int* viewportBuffer;
	int* selectBuffer;
	long long lastTime = RDTimer::MilliTime();
	int frames = 0;
	bool mouseCaptured;

	void Tick();

	void MoveCameraToPlayer(float a);

	void SetupCamera(float a);

	void SetupPickCamera(float a, int x, int y);

	void Pick(float a);

	void Render(float a);

public:
	RubyDung();

	virtual void Init(IGameEngine^ engine);

	virtual void OnResize(int width, int height);

	virtual void OnKeyState(int key, char c, bool state, bool repeat);

	virtual void OnMouseMove(float x, float y, float xd, float yd);

	virtual void OnMouseButton(int btn, bool state);

	virtual void Draw();

	virtual void Destroy();
};
