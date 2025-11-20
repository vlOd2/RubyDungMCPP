#pragma once

#include "RDAABB.h"
#include <glad/glad.h>

public ref class RDFrustum {
private:
    array<float, 2>^ m_Frustum;
    float* proj;
    float* modl;
    float* clip;

	void NormalizePlane(int side) {
		float magnitude = (float)Math::Sqrt(
			this->m_Frustum[side, 0] * this->m_Frustum[side, 0] + 
			this->m_Frustum[side, 1] * this->m_Frustum[side, 1] + 
			this->m_Frustum[side, 2] * this->m_Frustum[side, 2]
		);
		this->m_Frustum[side, 0] /= magnitude;
		this->m_Frustum[side, 1] /= magnitude;
		this->m_Frustum[side, 2] /= magnitude;
		this->m_Frustum[side, 3] /= magnitude;
	}

public:
    RDFrustum() {
        this->m_Frustum = gcnew array<float, 2>(6, 4);
		this->proj = new float[16];
		this->modl = new float[16];
		this->clip = new float[16];
    }

	void Calculate() {
		glGetFloatv(GL_PROJECTION_MATRIX, this->proj);
		glGetFloatv(GL_MODELVIEW_MATRIX, this->modl);
		this->clip[0] = this->modl[0] * this->proj[0] + this->modl[1] * this->proj[4] + this->modl[2] * this->proj[8] + this->modl[3] * this->proj[12];
		this->clip[1] = this->modl[0] * this->proj[1] + this->modl[1] * this->proj[5] + this->modl[2] * this->proj[9] + this->modl[3] * this->proj[13];
		this->clip[2] = this->modl[0] * this->proj[2] + this->modl[1] * this->proj[6] + this->modl[2] * this->proj[10] + this->modl[3] * this->proj[14];
		this->clip[3] = this->modl[0] * this->proj[3] + this->modl[1] * this->proj[7] + this->modl[2] * this->proj[11] + this->modl[3] * this->proj[15];
		this->clip[4] = this->modl[4] * this->proj[0] + this->modl[5] * this->proj[4] + this->modl[6] * this->proj[8] + this->modl[7] * this->proj[12];
		this->clip[5] = this->modl[4] * this->proj[1] + this->modl[5] * this->proj[5] + this->modl[6] * this->proj[9] + this->modl[7] * this->proj[13];
		this->clip[6] = this->modl[4] * this->proj[2] + this->modl[5] * this->proj[6] + this->modl[6] * this->proj[10] + this->modl[7] * this->proj[14];
		this->clip[7] = this->modl[4] * this->proj[3] + this->modl[5] * this->proj[7] + this->modl[6] * this->proj[11] + this->modl[7] * this->proj[15];
		this->clip[8] = this->modl[8] * this->proj[0] + this->modl[9] * this->proj[4] + this->modl[10] * this->proj[8] + this->modl[11] * this->proj[12];
		this->clip[9] = this->modl[8] * this->proj[1] + this->modl[9] * this->proj[5] + this->modl[10] * this->proj[9] + this->modl[11] * this->proj[13];
		this->clip[10] = this->modl[8] * this->proj[2] + this->modl[9] * this->proj[6] + this->modl[10] * this->proj[10] + this->modl[11] * this->proj[14];
		this->clip[11] = this->modl[8] * this->proj[3] + this->modl[9] * this->proj[7] + this->modl[10] * this->proj[11] + this->modl[11] * this->proj[15];
		this->clip[12] = this->modl[12] * this->proj[0] + this->modl[13] * this->proj[4] + this->modl[14] * this->proj[8] + this->modl[15] * this->proj[12];
		this->clip[13] = this->modl[12] * this->proj[1] + this->modl[13] * this->proj[5] + this->modl[14] * this->proj[9] + this->modl[15] * this->proj[13];
		this->clip[14] = this->modl[12] * this->proj[2] + this->modl[13] * this->proj[6] + this->modl[14] * this->proj[10] + this->modl[15] * this->proj[14];
		this->clip[15] = this->modl[12] * this->proj[3] + this->modl[13] * this->proj[7] + this->modl[14] * this->proj[11] + this->modl[15] * this->proj[15];
		this->m_Frustum[0, 0] = this->clip[3] - this->clip[0];
		this->m_Frustum[0, 1] = this->clip[7] - this->clip[4];
		this->m_Frustum[0, 2] = this->clip[11] - this->clip[8];
		this->m_Frustum[0, 3] = this->clip[15] - this->clip[12];
		this->NormalizePlane(0);
		this->m_Frustum[1, 0] = this->clip[3] + this->clip[0];
		this->m_Frustum[1, 1] = this->clip[7] + this->clip[4];
		this->m_Frustum[1, 2] = this->clip[11] + this->clip[8];
		this->m_Frustum[1, 3] = this->clip[15] + this->clip[12];
		this->NormalizePlane(1);
		this->m_Frustum[2, 0] = this->clip[3] + this->clip[1];
		this->m_Frustum[2, 1] = this->clip[7] + this->clip[5];
		this->m_Frustum[2, 2] = this->clip[11] + this->clip[9];
		this->m_Frustum[2, 3] = this->clip[15] + this->clip[13];
		this->NormalizePlane(2);
		this->m_Frustum[3, 0] = this->clip[3] - this->clip[1];
		this->m_Frustum[3, 1] = this->clip[7] - this->clip[5];
		this->m_Frustum[3, 2] = this->clip[11] - this->clip[9];
		this->m_Frustum[3, 3] = this->clip[15] - this->clip[13];
		this->NormalizePlane(3);
		this->m_Frustum[4, 0] = this->clip[3] - this->clip[2];
		this->m_Frustum[4, 1] = this->clip[7] - this->clip[6];
		this->m_Frustum[4, 2] = this->clip[11] - this->clip[10];
		this->m_Frustum[4, 3] = this->clip[15] - this->clip[14];
		this->NormalizePlane(4);
		this->m_Frustum[5, 0] = this->clip[3] + this->clip[2];
		this->m_Frustum[5, 1] = this->clip[7] + this->clip[6];
		this->m_Frustum[5, 2] = this->clip[11] + this->clip[10];
		this->m_Frustum[5, 3] = this->clip[15] + this->clip[14];
		this->NormalizePlane(5);
	}

    bool Contains(float x1, float y1, float z1, float x2, float y2, float z2) {
		for (int i = 0; i < 6; i++) {
			if (this->m_Frustum[i, 0] * x1 + this->m_Frustum[i, 1] * y1 + this->m_Frustum[i, 2] * z1 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x2 + this->m_Frustum[i, 1] * y1 + this->m_Frustum[i, 2] * z1 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x1 + this->m_Frustum[i, 1] * y2 + this->m_Frustum[i, 2] * z1 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x2 + this->m_Frustum[i, 1] * y2 + this->m_Frustum[i, 2] * z1 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x1 + this->m_Frustum[i, 1] * y1 + this->m_Frustum[i, 2] * z2 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x2 + this->m_Frustum[i, 1] * y1 + this->m_Frustum[i, 2] * z2 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x1 + this->m_Frustum[i, 1] * y2 + this->m_Frustum[i, 2] * z2 + this->m_Frustum[i, 3] <= 0.0F && 
				this->m_Frustum[i, 0] * x2 + this->m_Frustum[i, 1] * y2 + this->m_Frustum[i, 2] * z2 + this->m_Frustum[i, 3] <= 0.0F) {
				return false;
			}
		}

        return true;
    }

    bool Contains(RDAABB^ bb) {
		return this->Contains(bb->x0, bb->y0, bb->z0, bb->x1, bb->y1, bb->z1);
    }
};