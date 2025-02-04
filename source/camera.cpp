#include "camera.hpp"
#include "x360_game.hpp"
#include <gfx\x360_render_target.hpp>

// OFFSET: 0x005d0d60, STATUS: TODO
Camera::Camera(bool start_enabled) : DynamicSearchableObject(), GameObject(start_enabled) {
}

// OFFSET: 0x005d1080, STATUS: TODO
Camera::~Camera() {
}

// OFFSET: 0x00577b40, STATUS: TODO
void Camera::Disable() {
}

// OFFSET: 0x00577ac0, STATUS: COMPLETE
void Camera::Enable() {
	GameObject::Enable();
	g_RenderTarget->camera = this;
	if (g_RenderTarget->width != max_viewport_width || g_RenderTarget->height != max_viewport_height) {
		SurfaceChanged(g_AspectRatios[g_ScreenMode], g_CameraWidth, g_CameraHeight);
	}
	unk_render_target_related = g_RenderTarget->unk_camera_related + 1;
}

// OFFSET: 0x005438b0, STATUS: COMPLETE
GameObject* Camera::Create() {
	return Create(g_AspectRatios[g_ScreenMode], g_CameraWidth, g_CameraHeight);
}

// OFFSET: 0x005ea7f0, STATUS: TODO
int Camera::Tick(float) {
	return 0;
}

// OFFSET: 0x00577bc0, STATUS: COMPLETE
int Camera::Restore() {
	GameObject::Restore();
	unk_render_target_related = g_RenderTarget->unk_camera_related + 1;
	return 0;
}

// OFFSET: 0x0056c000, STATUS: COMPLETE
int Camera::KeyDownHandler(ProcessNode* process_node, KeyInfo* key_info) {
	GameObject::KeyDownHandler(process_node, key_info);
	return 0;
}

// OFFSET: 0x005778b0, STATUS: COMPLETE
GameObject* Camera::Create(float aspect_ratio, int width, int height) {
	if (SurfaceChanged(g_AspectRatios[g_ScreenMode], g_CameraWidth, g_CameraHeight) == 0) {
		Release();
		return nullptr;
	}
	UpdateMatrices();
	if (GameObject::flags.enabled) {
		g_RenderTarget->SetCamera(this);
	}
	return static_cast<GameObject*>(this);
}

// OFFSET: INLINE, STATUS: COMPLETE
void Camera::DumpSnapShot() {
}

// OFFSET: 0x00543c70, STATUS: TODO
void Camera::SetDumpCubeViewParameters() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void Camera::DumpCubeFace() {
}

// OFFSET: 0x00543f20, STATUS: TODO
void Camera::SetDumpSpinViewParameters() {
}

// OFFSET: 0x00543eb0, STATUS: COMPLETE
void Camera::DumpSpin(Vector4* axis, Vector4* offset, int cycle_count) {
	dump_spin_axis = *axis;
	dump_spin_offset = *offset;
	dump_spin_cycle_count = cycle_count;
	SetDumpSpinViewParameters();
}

// OFFSET: INLINE, STATUS: COMPLETE
void Camera::DumpSpinFrame() {
}

// OFFSET: 0x005a5f90, STATUS: TODO
void Camera::UpdateMatrices() {
}

// OFFSET: 0x00543bd0, STATUS: COMPLETE
void Camera::ComputeGuardBandMatrix(Matrix4x4* out, Matrix4x4* out_projection, float xfov, float yfov) {
	*out_projection = projection_matrix;
	out_projection->m[0][0] = xfov * out_projection->m[0][0];
	out_projection->m[1][0] = out_projection->m[1][0] * xfov;
	out_projection->m[2][0] = out_projection->m[2][0] * xfov;
	out_projection->m[3][0] = xfov * out_projection->m[3][0];
	out_projection->m[0][1] = yfov * out_projection->m[0][1];
	out_projection->m[1][1] = yfov * out_projection->m[1][1];
	out_projection->m[2][1] = out_projection->m[2][1] * yfov;
	out_projection->m[3][1] = yfov * out_projection->m[3][1];
	D3DXMatrixMultiply(out, &view_matrix, out_projection);
	return;
}

// OFFSET: 0x00543c50, STATUS: COMPLETE
void Camera::UpdateClipDistances(float _near, float _far) {
	near_clip_distance = _near;
	far_clip_distance = _far;
	UpdateMatrices();
}

// OFFSET: 0x00577950, STATUS: WIP
int Camera::LookAt(Vector4* at, Vector4* up) {
	if ((at->z == position.z && at->y == position.y && at->x == position.x) || dump_cube_tick == -1 || dump_spin_tick == -1) {
		return 0;
	}
	up_vector = *up;
	look_vector = {at->x - position.x, at->y - position.y, at->z - position.z, at->w - position.w};
	look_vector.Normalize3D();
	
	// what does this even do...
	if (at->z == position.z || std::isnan(at->z) || std::isnan(position.z) || at->x == position.x || std::isnan(at->x) || std::isnan(position.x)) {
		if ((up->x == 0.0f && up->y == 1.0f && up->z == 0.0f)) {
			Vector4 x_unit{ 1.0f, 0.0f, 0.0f, 1.0f };
			up_vector = look_vector.Cross3D(x_unit);
			unk_render_target_related = g_RenderTarget->unk_camera_related + 1;
			return 1;
		}
	}

	Vector4 up_cross_look = up_vector.Cross3D(look_vector);
	up_cross_look.Normalize3D();
	up_vector = look_vector.Cross3D(up_cross_look);
	unk_render_target_related = g_RenderTarget->unk_camera_related + 1;
	return 1;
}

// OFFSET: 0x00577910, STATUS: COMPLETE
int Camera::SurfaceChanged(float _aspect_ratio, int _width, int _height) {
	this->unk_aspect_scale = _aspect_ratio;
	this->max_viewport_width = _width;
	this->max_viewport_height = _height;
	SetUpViewportAndMatrices(0, 0, _width, _height);
	return 1;
}

// OFFSET: 0x005ea7c0, STATUS: COMPLETE
void Camera::AddSearchPlacement(SearchNode* node, unsigned int index) {
	search_node = node;
	Vector4 zero{ 0,0,0,1 };
	PerformInitialPlacement(node, index, &position, &zero, 0.0f, 0, 0, nullptr);
}

// OFFSET: 0x00543ad0, STATUS: COMPLETE
void Camera::GetViewParameters(Vector4* out_pos, Vector4* out_look, Vector4* out_up, float* out_aspect, float* out_fov) {
	if (out_pos != nullptr) {
		*out_pos = position;
	}
	if (out_look != nullptr) {
		*out_look = look_vector;
	}
	if (out_up != nullptr) {
		*out_up = up_vector;
	}
	if (out_aspect != nullptr) {
		*out_aspect = aspect_ratio;
	}
	if (out_fov != nullptr) {
		*out_fov = fov_degrees;
	}
}

// OFFSET: 0x005f9330, STATUS: COMPLETE
void Camera::ResetSearchPlacement(SearchNode* node, unsigned int index) {
	search_node = node;
	Vector4 zero{ 0,0,0,1 };
	ResetInitialPlacement(node, index, &position, &zero, 0.0f, 0, 0, nullptr);
}

// OFFSET: 0x00544210, STATUS: COMPLETE
int Camera::ResizeViewport(int x, int y, int w, int h) {
	if (max_viewport_width < w) {
		w = max_viewport_width;
	}
	if (max_viewport_height < h) {
		h = max_viewport_height;
	}
	SetUpViewportAndMatrices(x, y, w, h);
	return 1;
}

// OFFSET: 0x00544350, STATUS: COMPLETE
void Camera::SetEnablePerspective(int _enable_perspective) {
	enable_perspective = _enable_perspective;
}

// OFFSET: 0x00543970, STATUS: COMPLETE
void Camera::SetFOVDegrees(float _fov) {
	if (!std::isnan(_fov) && _fov != 0.0f) {
		fov_degrees = _fov;
		unk_render_target_related = g_RenderTarget->unk_camera_related + 1;
		return;
	}
	unk_render_target_related = g_RenderTarget->unk_camera_related + 1;
}

// OFFSET: 0x005438e0, STATUS: COMPLETE
void Camera::SetUpViewportAndMatrices(int x, int y, int w, int h) {
	viewport.X = PlatformGraphics_BackBufferViewport_OffsetX + x;
	viewport.Y = PlatformGraphics_BackBufferViewport_OffsetY + y;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	viewport.Width = w;
	viewport.Height = h;
	camera_flags |= 8;
	viewport_to_camera_aspect = (static_cast<float>(h) / w) / (static_cast<float>(max_viewport_height) / max_viewport_width * unk_aspect_scale);
	UpdateMatrices();
}

// OFFSET: 0x00543b80, STATUS: COMPLETE
int Camera::SetUpViewportForLL(int x, int y, int w, int h, float ll_shift) {
	last_laugh_shift = ll_shift;
	is_last_laugh = 1;
	if (max_viewport_width < w) {
		w = max_viewport_width;
	}
	if (max_viewport_height < h) {
		h = max_viewport_height;
	}
	SetUpViewportAndMatrices(x, y, w, h);
	return 1;
}

// OFFSET: 0x00543a20, STATUS: COMPLETE
void Camera::SetViewParameters(Vector4* pos, Vector4* look, Vector4* up, float* aspect, float* fov) {
	if (dump_cube_tick == -1 && dump_spin_tick == -1) {
		if (pos != nullptr) {
			position = *pos;
		}
		if (look != nullptr) {
			look_vector = *look;
			look_vector.Normalize3D();
		}
		if (up != nullptr) {
			up_vector = *up;
			up_vector.Normalize3D();
		}
		if (aspect != nullptr) {
			aspect_ratio = *aspect;
		}
		if (fov != nullptr) {
			SetFOVDegrees(*fov);
		}
	}
}

// OFFSET: 0x005442a0, STATUS: COMPLETE
void Camera::SetViewport(D3DVIEWPORT9* new_viewport, D3DVIEWPORT9* old_viewport) {
	if (old_viewport != nullptr) {
		old_viewport->X = viewport.X;
		old_viewport->Y = viewport.Y;
		old_viewport->Width = viewport.Width;
		old_viewport->Height = viewport.Height;
		old_viewport->MinZ = viewport.MinZ;
		old_viewport->MaxZ = viewport.MaxZ;
		old_viewport->X = old_viewport->X - PlatformGraphics_BackBufferViewport_OffsetX;
		old_viewport->Y = old_viewport->Y - PlatformGraphics_BackBufferViewport_OffsetY;
	}
	viewport.X = new_viewport->X;
	viewport.Y = new_viewport->Y;
	viewport.Width = new_viewport->Width;
	viewport.Height = new_viewport->Height;
	viewport.MinZ = new_viewport->MinZ;
	viewport.MaxZ = new_viewport->MaxZ;
	viewport.X += PlatformGraphics_BackBufferViewport_OffsetX;
	viewport.Y += PlatformGraphics_BackBufferViewport_OffsetY;
}
