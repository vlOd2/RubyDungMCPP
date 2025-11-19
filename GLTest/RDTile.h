#pragma once

#include "RDTesselator.h"
#include "RDLevel.h"

#define IS_SHADOW_LAYER(defcolor) ((br == defcolor) ^ layer) == 1

public ref class RDTile {
public:
	static RDTile^ rock = gcnew RDTile(0);
	static RDTile^ grass = gcnew RDTile(1);

private:
	int tex;

	RDTile(int tex) {
		this->tex = tex;
	}

public:
	void Render(RDTesselator^ t, RDLevel^ level, int layer, int x, int y, int z) {
		float u0 = (float)this->tex / 16.0F;
		float u1 = u0 + 0.0624375F;
		float v0 = 0.0F;
		float v1 = v0 + 0.0624375F;
		float c1 = 1.0F;
		float c2 = 0.8F;
		float c3 = 0.6F;
		float x0 = (float)x + 0.0F;
		float x1 = (float)x + 1.0F;
		float y0 = (float)y + 0.0F;
		float y1 = (float)y + 1.0F;
		float z0 = (float)z + 0.0F;
		float z1 = (float)z + 1.0F;
		
		float br;
		if (!level->IsSolidTile(x, y - 1, z)) {
			br = level->GetBrightness(x, y - 1, z) * c1;
			if (IS_SHADOW_LAYER(c1)) {
				t->Color(br, br, br);
				t->Tex(u0, v1);
				t->Vertex(x0, y0, z1);
				t->Tex(u0, v0);
				t->Vertex(x0, y0, z0);
				t->Tex(u1, v0);
				t->Vertex(x1, y0, z0);
				t->Tex(u1, v1);
				t->Vertex(x1, y0, z1);
			}
		}

		if (!level->IsSolidTile(x, y + 1, z)) {
			br = level->GetBrightness(x, y, z) * c1;
			if (IS_SHADOW_LAYER(c1)) {
				t->Color(br, br, br);
				t->Tex(u1, v1);
				t->Vertex(x1, y1, z1);
				t->Tex(u1, v0);
				t->Vertex(x1, y1, z0);
				t->Tex(u0, v0);
				t->Vertex(x0, y1, z0);
				t->Tex(u0, v1);
				t->Vertex(x0, y1, z1);
			}
		}

		if (!level->IsSolidTile(x, y, z - 1)) {
			br = level->GetBrightness(x, y, z - 1) * c2;
			if (IS_SHADOW_LAYER(c2)) {
				t->Color(br, br, br);
				t->Tex(u1, v0);
				t->Vertex(x0, y1, z0);
				t->Tex(u0, v0);
				t->Vertex(x1, y1, z0);
				t->Tex(u0, v1);
				t->Vertex(x1, y0, z0);
				t->Tex(u1, v1);
				t->Vertex(x0, y0, z0);
			}
		}

		if (!level->IsSolidTile(x, y, z + 1)) {
			br = level->GetBrightness(x, y, z + 1) * c2;
			if (IS_SHADOW_LAYER(c2)) {
				t->Color(br, br, br);
				t->Tex(u0, v0);
				t->Vertex(x0, y1, z1);
				t->Tex(u0, v1);
				t->Vertex(x0, y0, z1);
				t->Tex(u1, v1);
				t->Vertex(x1, y0, z1);
				t->Tex(u1, v0);
				t->Vertex(x1, y1, z1);
			}
		}

		if (!level->IsSolidTile(x - 1, y, z)) {
			br = level->GetBrightness(x - 1, y, z) * c3;
			if (IS_SHADOW_LAYER(c3)) {
				t->Color(br, br, br);
				t->Tex(u1, v0);
				t->Vertex(x0, y1, z1);
				t->Tex(u0, v0);
				t->Vertex(x0, y1, z0);
				t->Tex(u0, v1);
				t->Vertex(x0, y0, z0);
				t->Tex(u1, v1);
				t->Vertex(x0, y0, z1);
			}
		}

		if (!level->IsSolidTile(x + 1, y, z)) {
			br = level->GetBrightness(x + 1, y, z) * c3;
			if (IS_SHADOW_LAYER(c3)) {
				t->Color(br, br, br);
				t->Tex(u0, v1);
				t->Vertex(x1, y0, z1);
				t->Tex(u1, v1);
				t->Vertex(x1, y0, z0);
				t->Tex(u1, v0);
				t->Vertex(x1, y1, z0);
				t->Tex(u0, v0);
				t->Vertex(x1, y1, z1);
			}
		}
	}

	void RenderFace(RDTesselator^ t, int x, int y, int z, int face) {
		float x0 = (float)x + 0.0F;
		float x1 = (float)x + 1.0F;
		float y0 = (float)y + 0.0F;
		float y1 = (float)y + 1.0F;
		float z0 = (float)z + 0.0F;
		float z1 = (float)z + 1.0F;

		if (face == 0) {
			t->Vertex(x0, y0, z1);
			t->Vertex(x0, y0, z0);
			t->Vertex(x1, y0, z0);
			t->Vertex(x1, y0, z1);
		}

		if (face == 1) {
			t->Vertex(x1, y1, z1);
			t->Vertex(x1, y1, z0);
			t->Vertex(x0, y1, z0);
			t->Vertex(x0, y1, z1);
		}

		if (face == 2) {
			t->Vertex(x0, y1, z0);
			t->Vertex(x1, y1, z0);
			t->Vertex(x1, y0, z0);
			t->Vertex(x0, y0, z0);
		}

		if (face == 3) {
			t->Vertex(x0, y1, z1);
			t->Vertex(x0, y0, z1);
			t->Vertex(x1, y0, z1);
			t->Vertex(x1, y1, z1);
		}

		if (face == 4) {
			t->Vertex(x0, y1, z1);
			t->Vertex(x0, y1, z0);
			t->Vertex(x0, y0, z0);
			t->Vertex(x0, y0, z1);
		}

		if (face == 5) {
			t->Vertex(x1, y0, z1);
			t->Vertex(x1, y0, z0);
			t->Vertex(x1, y1, z0);
			t->Vertex(x1, y1, z1);
		}
	}
};