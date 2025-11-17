#pragma once
#include <corecrt_malloc.h>

public ref class RDTesselator {
private:
	float* vertexBuffer;
	float* texCoordBuffer;
	float* colorBuffer;
	int vertices;
	float u;
	float v;
	float r;
	float g;
	float b;
	bool hasColor;
	bool hasTexture;

public:
	RDTesselator() {
		vertexBuffer = (float*)calloc(300000, sizeof(float));
		texCoordBuffer = (float*)calloc(200000, sizeof(float));
		colorBuffer = (float*)calloc(300000, sizeof(float));
	}

	void Flush() {

	}

	void Init() {

	}

	void Tex(float u, float v) {

	}

	void Color(float r, float g, float b) {

	}

	void Vertex(float x, float y, float z) {

	}

private:
	void Clear() {

	}
};