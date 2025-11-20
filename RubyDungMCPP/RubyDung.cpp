#include "pch.h"
#include "RubyDung.h"

RubyDung::RubyDung() {
	this->viewportBuffer = (int*)calloc(16, sizeof(int));
	this->selectBuffer = (int*)calloc(2000, sizeof(int));
}

void RubyDung::Init(IGameEngine^ engine) {
	int col = 920330;
	float fr = 0.5F;
	float fg = 0.8F;
	float fb = 1.0F;
	fogColor = new float[4] { (float)(col >> 16 & 255) / 255.0F, (float)(col >> 8 & 255) / 255.0F, (float)(col & 255) / 255.0F, 1.0F };

	this->engine = engine;
	this->width = engine->Width;
	this->height = engine->Height;
	
	glViewport(0, 0, this->width, this->height);
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

	//this->mouseCaptured = true;
	//this->engine->SetMouseCapture(this->mouseCaptured);
}

void RubyDung::OnResize(int width, int height) {
	Console::WriteLine("Resized to {0}x{1}", width, height);
	this->width = width;
	this->height = height;
}

void RubyDung::OnKeyState(int key, char c, bool state, bool repeat) {
	if (key == (int)Keys::Escape && state) {
		Console::WriteLine("Toggling mouse");
		this->mouseCaptured = !this->mouseCaptured;
		this->engine->SetMouseCapture(this->mouseCaptured);
	}
	if (key == (int)Keys::Enter && state) {
		this->level->Save();
	}
}

void RubyDung::OnMouseMove(float x, float y, float xd, float yd) {
	this->player->Turn(xd, -yd);
}

void RubyDung::OnMouseButton(int btn, bool state) {
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

void RubyDung::Draw() {
	this->timer->AdvanceTime();

	for (int e = 0; e < this->timer->ticks; e++) {
		this->Tick();
	}

	this->Render(this->timer->a);
	this->frames++;

	while (RDTimer::MilliTime() >= this->lastTime + 1000L) {
		Console::WriteLine("{0} fps, {1}", this->frames, RDChunk::updates);
		RDChunk::updates = 0;
		this->lastTime += 1000L;
		this->frames = 0;
	}
}

void RubyDung::Tick() {
	this->player->Tick();
}

void RubyDung::MoveCameraToPlayer(float a) {
	glTranslatef(0.0F, 0.0F, -0.3F);
	glRotatef(this->player->xRot, 1.0F, 0.0F, 0.0F);
	glRotatef(this->player->yRot, 0.0F, 1.0F, 0.0F);
	float x = this->player->xo + (this->player->x - this->player->xo) * a;
	float y = this->player->yo + (this->player->y - this->player->yo) * a;
	float z = this->player->zo + (this->player->z - this->player->zo) * a;
	glTranslatef(-x, -y, -z);
}

void RubyDung::SetupCamera(float a) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0F, (float)this->width / (float)this->height, 0.05F, 1000.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->MoveCameraToPlayer(a);
}

void RubyDung::SetupPickCamera(float a, int x, int y) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, this->viewportBuffer);
	gluPickMatrix((float)x, (float)y, 5.0F, 5.0F, this->viewportBuffer);
	gluPerspective(70.0F, (float)this->width / (float)this->height, 0.05F, 1000.0F);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->MoveCameraToPlayer(a);
}

void RubyDung::Pick(float a) {
	int bufOff = 0;

	glSelectBuffer(2000, (unsigned int*)this->selectBuffer);
	glRenderMode(GL_SELECT);
	this->SetupPickCamera(a, this->width / 2, this->height / 2);
	this->levelRenderer->Pick(this->player);
	int hits = glRenderMode(GL_RENDER);

	long closest = 0L;
	int names[10]{};
	int hitNameCount = 0;

	for (int i = 0; i < hits; i++) {
		int nameCount = this->selectBuffer[bufOff++];
		long minZ = (long)this->selectBuffer[bufOff++];
		bufOff++;

		int j;
		if (minZ >= closest && i != 0) {
			for (j = 0; j < nameCount; ++j) {
				bufOff++;
			}
		}
		else {
			closest = minZ;
			hitNameCount = nameCount;

			for (j = 0; j < nameCount; ++j) {
				names[j] = this->selectBuffer[bufOff++];
			}
		}
	}

	if (hitNameCount > 0) {
		this->hitResult = gcnew RDHitResult(names[0], names[1], names[2], names[3], names[4]);
	}
	else {
		this->hitResult = nullptr;
	}
}

void RubyDung::Render(float a) {
	this->Pick(a);
	glViewport(0, 0, this->width, this->height);
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

void RubyDung::Destroy() {
	this->level->Save();
}