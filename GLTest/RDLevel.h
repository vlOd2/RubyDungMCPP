#pragma once

#include "RDLevelListener.h"
#include "RDAABB.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

public ref class RDLevel {
public:
	int width;
	int height;
	int depth;

private:
	array<unsigned char>^ blocks;
	array<int>^ lightDepths;
	List<RDLevelListener^>^ listeners;

public:
	RDLevel(int w, int h, int d) {
		this->width = w;
		this->height = h;
		this->depth = d;
		this->blocks = gcnew array<unsigned char>(w * h * d);
		this->lightDepths = gcnew array<int>(w * h);
		this->listeners = gcnew List<RDLevelListener^>();

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < d; y++) {
				for (int z = 0; z < h; z++) {
					int i = (y * this->height + z) * this->width + x;
					this->blocks[i] = y <= d * 2 / 3 ? 1 : 0;
				}
			}
		}

		CalcLightDepths(0, 0, w, h);
		Load();
	}

	void Save() {
		try {
			Console::WriteLine("Saving level");
			FileStream^ stream = File::Open("level.dat", FileMode::Create);
			BinaryWriter^ writer = gcnew BinaryWriter(stream);
			writer->Write(this->width);
			writer->Write(this->height);
			writer->Write(this->depth);
			writer->Write(this->blocks);
			writer->Flush();
			delete writer;
			delete stream;
		}
		catch (Exception^ ex) {
			Console::WriteLine("Failed to save level: " + ex);
		}
	}

	void Load() {
		try {
			Console::WriteLine("Loading level");
			FileStream^ stream = File::Open("level.dat", FileMode::Open);
			BinaryReader^ reader = gcnew BinaryReader(stream);

			this->width = reader->ReadInt32();
			this->height = reader->ReadInt32();
			this->depth = reader->ReadInt32();
			this->blocks = reader->ReadBytes(this->width * this->height * this->depth);

			delete reader;
			delete stream;

			CalcLightDepths(0, 0, this->width, this->height);
			for each (RDLevelListener ^ listener in this->listeners) {
				listener->AllChanged();
			}
		}
		catch (Exception^ ex) {
			Console::WriteLine("Failed to load level: " + ex);
		}
	}

	void CalcLightDepths(int x0, int y0, int x1, int y1) {
		for (int x = x0; x < x0 + x1; x++) {
			for (int z = y0; z < y0 + y1; z++) {
				int oldDepth = this->lightDepths[x + z * this->width];
				int y = this->depth - 1;

				while (y > 0 && !this->IsLightBlocker(x, y, z)) {
					y--;
				}

				this->lightDepths[x + z * this->width] = y;
				if (oldDepth != y) {
					int yl0 = oldDepth < y ? oldDepth : y;
					int yl1 = oldDepth > y ? oldDepth : y;

					for each (RDLevelListener^ listener in this->listeners) {
						listener->LightColumnChanged(x, z, yl0, yl1);
					}
				}
			}
		}
	}

	void AddListener(RDLevelListener^ listener) {
		this->listeners->Add(listener);
	}

	void RemoveListener(RDLevelListener^ listener) {
		this->listeners->Remove(listener);
	}

	bool IsTile(int x, int y, int z) {
		return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? 
			this->blocks[(y * this->height + z) * this->width + x] == 1 : false;
	}

	bool IsSolidTile(int x, int y, int z) {
		return this->IsTile(x, y, z);
	}

	bool IsLightBlocker(int x, int y, int z) {
		return this->IsSolidTile(x, y, z);
	}

	List<RDAABB^>^ GetCubes(RDAABB^ aABB) {
		List<RDAABB^>^ aABBs = gcnew List<RDAABB^>();
		int x0 = (int)aABB->x0;
		int x1 = (int)(aABB->x1 + 1.0F);
		int y0 = (int)aABB->y0;
		int y1 = (int)(aABB->y1 + 1.0F);
		int z0 = (int)aABB->z0;
		int z1 = (int)(aABB->z1 + 1.0F);

		if (x0 < 0) {
			x0 = 0;
		}
		if (y0 < 0) {
			y0 = 0;
		}
		if (z0 < 0) {
			z0 = 0;
		}

		if (x1 > this->width) {
			x1 = this->width;
		}
		if (y1 > this->depth) {
			y1 = this->depth;
		}
		if (z1 > this->height) {
			z1 = this->height;
		}

		for (int x = x0; x < x1; x++) {
			for (int y = y0; y < y1; y++) {
				for (int z = z0; z < z1; z++) {
					if (this->IsSolidTile(x, y, z)) {
						aABBs->Add(gcnew RDAABB((float)x, (float)y, (float)z, (float)(x + 1), (float)(y + 1), (float)(z + 1)));
					}
				}
			}
		}

		return aABBs;
	}

	float GetBrightness(int x, int y, int z) {
		float dark = 0.8F;
		float light = 1.0F;
		return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? 
			(y < this->lightDepths[x + z * this->width] ? dark : light) : light;
	}

	void SetTile(int x, int y, int z, int type) {
		if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) {
			this->blocks[(y * this->height + z) * this->width + x] = (unsigned char)type;
			this->CalcLightDepths(x, z, 1, 1);

			for each (RDLevelListener ^ listener in this->listeners) {
				listener->TileChanged(x, y, z);
			}
		}
	}
};