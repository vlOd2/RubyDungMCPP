#pragma once

#include <glad/glad.h>
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
		this->vertexBuffer = (float*)calloc(300000, sizeof(float));
		this->texCoordBuffer = (float*)calloc(200000, sizeof(float));
		this->colorBuffer = (float*)calloc(300000, sizeof(float));
	}

	!RDTesselator() {
		free(this->vertexBuffer);
		free(this->texCoordBuffer);
		free(this->colorBuffer);
	}

	void Flush() {
		glVertexPointer(3, GL_FLOAT, 0, this->vertexBuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		if (this->hasTexture) {
			glTexCoordPointer(2, GL_FLOAT, 0, this->texCoordBuffer);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if (this->hasColor) {
			glColorPointer(3, GL_FLOAT, 0, this->colorBuffer);
			glEnableClientState(GL_COLOR_ARRAY);
		}

		glDrawArrays(GL_QUADS, 0, this->vertices);

		glDisableClientState(GL_VERTEX_ARRAY);
		if (this->hasTexture) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if (this->hasColor) {
			glDisableClientState(GL_COLOR_ARRAY);
		}

		this->Clear();
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
		this->vertexBuffer[this->vertices * 3 + 0] = x;
		this->vertexBuffer[this->vertices * 3 + 1] = y;
		this->vertexBuffer[this->vertices * 3 + 2] = z;

		if (this->hasTexture) {
			this->texCoordBuffer[this->vertices * 2 + 0] = this->u;
			this->texCoordBuffer[this->vertices * 2 + 1] = this->v;
		}

		if (this->hasColor) {
			this->colorBuffer[this->vertices * 3 + 0] = this->r;
			this->colorBuffer[this->vertices * 3 + 1] = this->g;
			this->colorBuffer[this->vertices * 3 + 2] = this->b;
		}

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