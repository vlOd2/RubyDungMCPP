#include "pch.h"
#include "Game.h"
#include "RDTextures.h"

Game::Game() {
}

void Game::Init(IGameEngine^ engine) {
	this->engine = engine;
	glClearColor(0.392F, 0.584F, 0.929F, 1.0F);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//this->engine->SetMouseCapture(true);
}

void Game::OnResize(int width, int height) {
	Console::WriteLine("Resized to {0}x{1}", width, height);
}

void Game::OnKeyState(int key, char c, bool state, bool repeat) {
	Console::WriteLine("Key {0} ({1}) is {2} {3}", key, c, state ? "down" : "up", repeat ? "(repeat)" : "");
}

void Game::OnMouseMove(float x, float y, float xd, float yd) {
	Console::WriteLine("Mouse moved to ({0}, {1}) delta ({2}, {3})", x, y, xd, yd);
}

void Game::OnMouseButton(int btn, bool state) {
	Console::WriteLine("Mouse button {0} is {1}", btn, state ? "down" : "up");
}

void Game::Draw() {
	glViewport(0, 0, this->engine->Width, this->engine->Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0F, 0.0F, 0.0F);
	glVertex2f(-0.5F, -0.5F);
	glColor3f(0.0F, 1.0F, 0.0F);
	glVertex2f(0.5F, -0.5F);
	glColor3f(0.0F, 0.0F, 1.0F);
	glVertex2f(0.0F, 0.5F);
	glEnd();
}