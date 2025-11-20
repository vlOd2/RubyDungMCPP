#pragma once

#include "RDLevel.h"
#include "RDAABB.h"
#include "IGameEngine.h"

using namespace System;
using namespace System::Windows::Forms;

public ref class RDPlayer {
private:
	IGameEngine^ engine;
	RDLevel^ level;
	Random^ rand = gcnew Random();

public:
	float xo;
	float yo;
	float zo;
	float x;
	float y;
	float z;
	float xd;
	float yd;
	float zd;
	float yRot;
	float xRot;
	RDAABB^ bb;
	bool onGround;

	RDPlayer(IGameEngine^ engine, RDLevel^ level) {
		this->engine = engine;
		this->level = level;
		this->ResetPos();
	}

private:
	void ResetPos() {
		float x = (float)this->rand->NextDouble() * (float)this->level->width;
		float y = (float)(this->level->depth + 10);
		float z = (float)this->rand->NextDouble() * (float)this->level->height;
		this->SetPos(x, y, z);
	}

	void SetPos(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		float w = 0.3F;
		float h = 0.9F;
		this->bb = gcnew RDAABB(x - w, y - h, z - w, x + w, y + h, z + w);
	}

public:
	void Turn(float xo, float yo) {
		this->yRot = (float)((double)this->yRot + (double)xo * 0.15F);
		this->xRot = (float)((double)this->xRot - (double)yo * 0.15F);
		if (this->xRot < -90.0F) {
			this->xRot = -90.0F;
		}
		if (this->xRot > 90.0F) {
			this->xRot = 90.0F;
		}
	}

	void Tick() {
		this->xo = this->x;
		this->yo = this->y;
		this->zo = this->z;
		float xa = 0.0F;
		float ya = 0.0F;

		if (this->engine->GetKeyState((int)Keys::R)) {
			this->ResetPos();
		}

		if (this->engine->GetKeyState((int)Keys::Up) || this->engine->GetKeyState((int)Keys::W)) {
			--ya;
		}

		if (this->engine->GetKeyState((int)Keys::Down) || this->engine->GetKeyState((int)Keys::S)) {
			++ya;
		}

		if (this->engine->GetKeyState((int)Keys::Left) || this->engine->GetKeyState((int)Keys::A)) {
			--xa;
		}

		if (this->engine->GetKeyState((int)Keys::Right) || this->engine->GetKeyState((int)Keys::D)) {
			++xa;
		}

		if ((this->engine->GetKeyState((int)Keys::Space) || this->engine->GetKeyState((int)Keys::Alt)) && this->onGround) {
			this->yd = 0.12F;
		}

		this->MoveRelative(xa, ya, this->onGround ? 0.02F : 0.005F);
		this->yd = (float)((double)this->yd - 0.005);
		this->Move(this->xd, this->yd, this->zd);
		this->xd *= 0.91F;
		this->yd *= 0.98F;
		this->zd *= 0.91F;
		if (this->onGround) {
			this->xd *= 0.8F;
			this->zd *= 0.8F;
		}
	}

	void Move(float xa, float ya, float za) {
		float xaOrg = xa;
		float yaOrg = ya;
		float zaOrg = za;
		List<RDAABB^>^ aABBs = this->level->GetCubes(this->bb->Expand(xa, ya, za));

		for each (RDAABB^ bb in aABBs) {
			ya = bb->ClipYCollide(this->bb, ya);
		}
		this->bb->Move(0.0F, ya, 0.0F);

		for each (RDAABB^ bb in aABBs) {
			xa = bb->ClipXCollide(this->bb, xa);
		}
		this->bb->Move(xa, 0.0F, 0.0F);

		for each (RDAABB^ bb in aABBs) {
			za = bb->ClipZCollide(this->bb, za);
		}
		this->bb->Move(0.0F, 0.0F, za);

		this->onGround = yaOrg != ya && yaOrg < 0.0F;
		if (xaOrg != xa) {
			this->xd = 0.0F;
		}
		if (yaOrg != ya) {
			this->yd = 0.0F;
		}
		if (zaOrg != za) {
			this->zd = 0.0F;
		}

		this->x = (this->bb->x0 + this->bb->x1) / 2.0F;
		this->y = this->bb->y0 + 1.62F;
		this->z = (this->bb->z0 + this->bb->z1) / 2.0F;
	}

	void MoveRelative(float xa, float za, float speed) {
		float dist = xa * xa + za * za;
		if (dist >= 0.01F) {
			dist = speed / (float)Math::Sqrt((double)dist);
			xa *= dist;
			za *= dist;
			float sin = (float)Math::Sin((double)this->yRot * Math::PI / 180.0F);
			float cos = (float)Math::Cos((double)this->yRot * Math::PI / 180.0F);
			this->xd += xa * cos - za * sin;
			this->zd += za * cos + xa * sin;
		}
	}
};