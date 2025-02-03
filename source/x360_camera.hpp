#pragma once
#include "math/matrix4x4.hpp"
#include "camera.hpp"

class X360Camera : public Camera {
private:
	Matrix4x4 unused4;
	Matrix4x4 view_projection_matrix_copy;
	Matrix4x4 unk_clip_scale_matrix;
public:
	virtual ~X360Camera() override;
	virtual int RenderPre3D() override;

	virtual void DumpSnapShot() override;
	virtual void UpdateMatrices() override;
};