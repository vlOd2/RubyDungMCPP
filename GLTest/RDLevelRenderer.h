#pragma once

#include "RDLevelListener.h"
#include "RDLevel.h"
#include "RDChunk.h"
#include "RDFrustum.h"
#include "RDHitResult.h"
#include "RDTesselator.h"
#include "RDTile.h"
#include "RDTimer.h"
#include "RDPlayer.h"

using namespace System;

public ref class RDLevelRenderer : public RDLevelListener {
private:
	RDLevel^ level;
	array<RDChunk^>^ chunks;
	int xChunks;
	int yChunks;
	int zChunks;
	RDTesselator^ t = gcnew RDTesselator();

public:
	RDLevelRenderer(RDLevel^ level) {
		this->level = level;
		level->AddListener(this);
		this->xChunks = level->width / 16;
		this->yChunks = level->depth / 16;
		this->zChunks = level->height / 16;
		this->chunks = gcnew array<RDChunk^>(this->xChunks * this->yChunks * this->zChunks);

		for (int x = 0; x < this->xChunks; x++) {
			for (int y = 0; y < this->yChunks; y++) {
				for (int z = 0; z < this->zChunks; z++) {
					int x0 = x * 16;
					int y0 = y * 16;
					int z0 = z * 16;
					int x1 = (x + 1) * 16;
					int y1 = (y + 1) * 16;
					int z1 = (z + 1) * 16;

					if (x1 > level->width) {
						x1 = level->width;
					}
					if (y1 > level->depth) {
						y1 = level->depth;
					}
					if (z1 > level->height) {
						z1 = level->height;
					}

					this->chunks[(x + y * this->xChunks) * this->zChunks + z] = gcnew RDChunk(level, x0, y0, z0, x1, y1, z1);
				}
			}
		}
	}

	void Render(RDPlayer^ player, int layer) {
		RDChunk::rebuiltThisFrame = 0;
		RDFrustum^ frustum = gcnew RDFrustum();
		frustum->Calculate();

		for (int i = 0; i < this->chunks->Length; i++) {
			if (frustum->Contains(this->chunks[i]->aabb)) {
				this->chunks[i]->Render(layer);
			}
		}
	}

	void Pick(RDPlayer^ player) {
		float r = 3.0F;
		RDAABB^ box = player->bb->Grow(r, r, r);
		int x0 = (int)box->x0;
		int x1 = (int)(box->x1 + 1.0F);
		int y0 = (int)box->y0;
		int y1 = (int)(box->y1 + 1.0F);
		int z0 = (int)box->z0;
		int z1 = (int)(box->z1 + 1.0F);
		glInitNames();

		for (int x = x0; x < x1; x++) {
			glPushName(x);

			for (int y = y0; y < y1; y++) {
				glPushName(y);

				for (int z = z0; z < z1; z++) {
					glPushName(z);
					if (this->level->IsSolidTile(x, y, z)) {
						glPushName(0);

						for (int i = 0; i < 6; ++i) {
							glPushName(i);
							this->t->Init();
							RDTile::rock->RenderFace(this->t, x, y, z, i);
							this->t->Flush();
							glPopName();
						}

						glPopName();
					}

					glPopName();
				}

				glPopName();
			}

			glPopName();
		}
	}

	void RenderHit(RDHitResult^ h) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(1.0F, 1.0F, 1.0F, (float)Math::Sin((double)RDTimer::MilliTime() / 100.0) * 0.2 + 0.4);
		this->t->Init();
		RDTile::rock->RenderFace(this->t, h->x, h->y, h->z, h->f);
		this->t->Flush();
		glDisable(GL_BLEND);
	}

	void SetDirty(int x0, int y0, int z0, int x1, int y1, int z1) {
		x0 /= 16;
		x1 /= 16;
		y0 /= 16;
		y1 /= 16;
		z0 /= 16;
		z1 /= 16;

		if (x0 < 0) {
			x0 = 0;
		}
		if (y0 < 0) {
			y0 = 0;
		}
		if (z0 < 0) {
			z0 = 0;
		}

		if (x1 >= this->xChunks) {
			x1 = this->xChunks - 1;
		}
		if (y1 >= this->yChunks) {
			y1 = this->yChunks - 1;
		}
		if (z1 >= this->zChunks) {
			z1 = this->zChunks - 1;
		}

		for (int x = x0; x <= x1; x++) {
			for (int y = y0; y <= y1; y++) {
				for (int z = z0; z <= z1; z++) {
					this->chunks[(x + y * this->xChunks) * this->zChunks + z]->SetDirty();
				}
			}
		}
	}

	virtual void TileChanged(int x, int y, int z) {
		this->SetDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
	}

	virtual void LightColumnChanged(int x, int z, int y0, int y1) {
		this->SetDirty(x - 1, y0 - 1, z - 1, x + 1, y1 + 1, z + 1);
	}

	virtual void AllChanged() {
		this->SetDirty(0, 0, 0, this->level->width, this->level->depth, this->level->height);
	}
};