#pragma once

#include <glad/glad.h>
#include "RDAABB.h"
#include "RDLevel.h"
#include "RDTextures.h"
#include "RDTesselator.h"
#include "RDTile.h"

public ref class RDChunk {
public:
	RDAABB^ aabb;
	RDLevel^ level;
	const int x0;
	const int y0;
	const int z0;
	const int x1;
	const int y1;
	const int z1;
	static int rebuiltThisFrame;
	static int updates;

private:
	bool dirty = true;
	unsigned int lists = 0xFFFFFFFF;
	static unsigned int texture = RDTextures::LoadTexture("/terrain.png", GL_NEAREST);
	static RDTesselator^ t = gcnew RDTesselator();

public:
	RDChunk(RDLevel^ level, int x0, int y0, int z0, int x1, int y1, int z1) : x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1) {
		this->level = level;
		this->aabb = gcnew RDAABB((float)x0, (float)y0, (float)z0, (float)x1, (float)y1, (float)z1);
		this->lists = glGenLists(2);
	}

	void Rebuild(int layer) {
		if (rebuiltThisFrame < 2) {
			return;
		}
		this->dirty = false;
		RDChunk::updates++;
		RDChunk::rebuiltThisFrame++;

		glNewList(this->lists + layer, GL_COMPILE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		RDChunk::t->Init();
		int tiles = 0;

		for (int x = this->x0; x < this->x1; x++) {
			for (int y = this->y0; y < this->y1; y++) {
				for (int z = this->z0; z < this->z1; z++) {
					if (this->level->IsTile(x, y, z)) {
						bool tex = y != this->level->depth * 2 / 3;
						tiles++;
						if (!tex) {
							RDTile::rock->Render(t, this->level, layer, x, y, z);
						}
						else {
							RDTile::grass->Render(t, this->level, layer, x, y, z);
						}
					}
				}
			}
		}

		RDChunk::t->Flush();
		glDisable(GL_TEXTURE_2D);
		glEndList();
	}

	void Render(int layer) {
		if (this->dirty) {
			this->Rebuild(0);
			this->Rebuild(1);
		}

		glCallList(this->lists + layer);
	}

	void SetDirty() {
		this->dirty = true;
	}
};