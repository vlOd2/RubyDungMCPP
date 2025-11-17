#pragma once

public ref class RDHitResult {
public:
	int x;
	int y;
	int z;
	int o;
	int f;

	RDHitResult(int x, int y, int z, int o, int f) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->o = o;
		this->f = f;
	}
};