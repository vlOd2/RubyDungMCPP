#pragma once

#include "RDLevelListener.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class RDLevel {
public:
	const int width;
	const int height;
	const int depth;

private:
	array<unsigned char>^ blocks;
	array<int>^ lightDepths;
	List<RDLevelListener^>^ listeners;

	RDLevel(int w, int h, int d) : width(w), height(h), depth(d) {
		this->blocks = gcnew array<unsigned char>(w * h * d);
		this->lightDepths = gcnew array<int>(w * d);

		for (int x = 0; x < w; x++) {
			for (int y = 0; y < d; y++) {
				for (int z = 0; z < h; z++) {
					int i = (y * this->height + z) * this->width + x;
					this->blocks[i] = y <= d * 2 / 3 ? 1 : 0;
				}
			}
		}
	}
};