#include "pch.h"
#include "Game.h"

Game::Game() {
}

void Game::Init(IGameEngine^ engine) {
	int col = 920330;
	float fr = 0.5F;
	float fg = 0.8F;
	float fb = 1.0F;
	fogColor = new float[] { (float)(col >> 16 & 255) / 255.0F, (float)(col >> 8 & 255) / 255.0F, (float)(col & 255) / 255.0F, 1.0F };

	this->engine = engine;
	this->width = engine->Width;
	this->height = engine->Height;
	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(fr, fg, fb, 0.0F);
	glClearDepth(1.0F);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	this->level = gcnew RDLevel(256, 256, 64);
	this->levelRenderer = gcnew RDLevelRenderer(this->level);
	this->player = gcnew RDPlayer(engine, this->level);

	//this->engine->SetMouseCapture(true);
}

void Game::OnResize(int width, int height) {
	Console::WriteLine("Resized to {0}x{1}", width, height);
	this->width = width;
	this->height = height;
}

void Game::OnKeyState(int key, char c, bool state, bool repeat) {
}

void Game::OnMouseMove(float x, float y, float xd, float yd) {
	this->player->Turn(xd, yd);
}

void Game::OnMouseButton(int btn, bool state) {
	if (!state || !this->hitResult) {
		return;
	}
	if (btn == 1) {
		this->level->SetTile(this->hitResult->x, this->hitResult->y, this->hitResult->z, 0);
	}
	if (btn == 0) {
		int x = this->hitResult->x;
		int y = this->hitResult->y;
		int z = this->hitResult->z;

		if (this->hitResult->f == 0) {
			y--;
		}
		if (this->hitResult->f == 1) {
			y++;
		}
		if (this->hitResult->f == 2) {
			z--;
		}
		if (this->hitResult->f == 3) {
			z++;
		}
		if (this->hitResult->f == 4) {
			x--;
		}
		if (this->hitResult->f == 5) {
			x++;
		}

		this->level->SetTile(x, y, z, 1);
	}
}

void Game::Draw() {
	this->timer->AdvanceTime();

	for (int e = 0; e < this->timer->ticks; e++) {
		this->Tick();
	}

	this->Render(this->timer->a);
	this->frames++;

	while (RDTimer::MilliTime()  >= lastTime + 1000L) {
		Console::WriteLine(frames + " fps, " + RDChunk::updates);
		RDChunk::updates = 0;
		this->lastTime += 1000L;
		this->frames = 0;
	}
}

void Game::Tick() {
	this->player->Tick();
}

void Game::MoveCameraToPlayer(float a) {
	glTranslatef(0.0F, 0.0F, -0.3F);
	glRotatef(this->player->xRot, 1.0F, 0.0F, 0.0F);
	glRotatef(this->player->yRot, 0.0F, 1.0F, 0.0F);
	float x = this->player->xo + (this->player->x - this->player->xo) * a;
	float y = this->player->yo + (this->player->y - this->player->yo) * a;
	float z = this->player->zo + (this->player->z - this->player->zo) * a;
	glTranslatef(-x, -y, -z);
}

void Game::SetupCamera(float a) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Matrix4x4 perspective = Matrix4x4::CreatePerspectiveFieldOfView(70.0F * (Math::PI / 180.0F),
		(float)this->width / (float)this->height, 0.05F, 1000.0F);
	glLoadMatrixf(&perspective.M11);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->MoveCameraToPlayer(a);
}

void Game::SetupPickCamera(float a, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glGetIntegerv(GL_VIEWPORT, this.viewportBuffer);
	//GLU.gluPickMatrix((float)x, (float)y, 5.0F, 5.0F, this.viewportBuffer);
	//GLU.gluPerspective(70.0F, (float)this.width / (float)this.height, 0.05F, 1000.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->MoveCameraToPlayer(a);
}

void Game::Render(float a) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->SetupCamera(a);
	glEnable(GL_CULL_FACE);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.2F);
	glFogfv(GL_FOG_COLOR, this->fogColor);
	glDisable(GL_FOG);
	this->levelRenderer->Render(this->player, 0);
	glEnable(GL_FOG);
	this->levelRenderer->Render(this->player, 1);
	glDisable(GL_TEXTURE_2D);
	if (this->hitResult != nullptr) {
		this->levelRenderer->RenderHit(this->hitResult);
	}
	glDisable(GL_FOG);
}
