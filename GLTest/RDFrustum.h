#pragma once

#include "RDAABB.h"
#include <glad/glad.h>

using namespace System::Numerics;

static Matrix4x4 ExportMatrix(unsigned int name) {
	float matrix[4 * 4];
	glGetFloatv(name, matrix);
	return Matrix4x4(
		matrix[0], matrix[4], matrix[8], matrix[12],
		matrix[1], matrix[5], matrix[9], matrix[13],
		matrix[2], matrix[6], matrix[10], matrix[14],
		matrix[3], matrix[7], matrix[11], matrix[15]
	);
}

public ref class RDFrustum {
private:
	array<Vector4>^ planes;

	RDFrustum() {
		this->planes = gcnew array<Vector4>(6);
	}

public:
	void Calculate() {
		Matrix4x4 projMatrix = ExportMatrix(GL_PROJECTION_MATRIX);
		Matrix4x4 viewMatrix = ExportMatrix(GL_MODELVIEW_MATRIX);
        Matrix4x4 clipMatrix = projMatrix * viewMatrix;

        this->planes[0] = Vector4(
            clipMatrix.M41 - clipMatrix.M11,
            clipMatrix.M42 - clipMatrix.M12,
            clipMatrix.M43 - clipMatrix.M13,
            clipMatrix.M44 - clipMatrix.M14
        );

        this->planes[1] = Vector4(
            clipMatrix.M41 + clipMatrix.M11,
            clipMatrix.M42 + clipMatrix.M12,
            clipMatrix.M43 + clipMatrix.M13,
            clipMatrix.M44 + clipMatrix.M14
        );

        this->planes[2] = Vector4(
            clipMatrix.M41 + clipMatrix.M21,
            clipMatrix.M42 + clipMatrix.M22,
            clipMatrix.M43 + clipMatrix.M23,
            clipMatrix.M44 + clipMatrix.M24
        );

        this->planes[3] = Vector4(
            clipMatrix.M41 - clipMatrix.M21,
            clipMatrix.M42 - clipMatrix.M22,
            clipMatrix.M43 - clipMatrix.M23,
            clipMatrix.M44 - clipMatrix.M24
        );

        this->planes[4] = Vector4(
            clipMatrix.M41 - clipMatrix.M31,
            clipMatrix.M42 - clipMatrix.M32,
            clipMatrix.M43 - clipMatrix.M33,
            clipMatrix.M44 - clipMatrix.M34
        );

        this->planes[5] = Vector4(
            clipMatrix.M41 + clipMatrix.M31,
            clipMatrix.M42 + clipMatrix.M32,
            clipMatrix.M43 + clipMatrix.M33,
            clipMatrix.M44 + clipMatrix.M34
        );
	}

    bool Contains(float x0, float y0, float z0, float x1, float y1, float z1) {
        for each(Vector4 plane in this->planes) {
            float px0 = plane.X * x0;
            float py0 = plane.Y * y0;
            float pz0 = plane.Z * z0;
            float px1 = plane.X * x1;
            float py1 = plane.Y * y1;
            float pz1 = plane.Z * z1;

            float xy00 = px0 + py0;
            float xy10 = px1 + py0;
            float xy01 = px0 + py1;
            float xy11 = px1 + py1;

            if (xy00 + pz0 + plane.W > 0) continue;
            if (xy10 + pz0 + plane.W > 0) continue;
            if (xy01 + pz0 + plane.W > 0) continue;
            if (xy11 + pz0 + plane.W > 0) continue;
            if (xy00 + pz1 + plane.W > 0) continue;
            if (xy10 + pz1 + plane.W > 0) continue;
            if (xy01 + pz1 + plane.W > 0) continue;
            if (xy11 + pz1 + plane.W > 0) continue;

            return false;
        }

        return true;
    }

    bool Contains(RDAABB bb) {
		return this->Contains(bb.x0, bb.y0, bb.z0, bb.x1, bb.y1, bb.z1);
    }
};