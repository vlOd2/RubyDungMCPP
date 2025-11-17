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
		this->Clear();
		this->hasColor = false;
		this->hasTexture = false;
	}

	void Tex(float u, float v) {
		this->hasTexture = true;
		this->u = u;
		this->v = v;
	}

	void Color(float r, float g, float b) {
		this->hasColor = true;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void Vertex(float x, float y, float z) {
		this->vertices++;
		if (this->vertices == 100000) {
			this->Flush();
		}
	}

private:
	void Clear() {
		this->vertices = 0;
	}
};