#include "x360_render_target.hpp"
#include "x360_video_card.hpp"
#include "math/vector4.hpp"

X360RenderTarget* g_RenderTarget = nullptr;

// OFFSET: 0x00413730, STATUS: COMPLETE
X360RenderTarget::X360RenderTarget() : RenderTarget() {
	fog_parameters[0] = 0.0;
	fog_parameters[1] = 0.0;
	unk_array_1[0] = nullptr;
	fog_parameters[2] = 0.0;
	unk_array_1[1] = nullptr;
	unk_array_1[2] = nullptr;
	unk_array_2[0] = nullptr;
	fog_parameters[3] = 1.0;
	unk_array_2[1] = nullptr;
	unk = nullptr;
	unused8 = 0;
	unused9 = 0;
	unused10 = 0;
	depth_stencil_surface = nullptr;
	unk_texture = nullptr;
	unused11 = 0;
	unused12 = 0;
	unk_viewport_index = 0;
	used_camera = nullptr;
	vertex_buffer_manager = nullptr;
	index_buffer_manager = nullptr;
	vertex_shader_manager = nullptr;
	pixel_shader_manager = nullptr;
	blt_vertex = nullptr;
	blt_pixel = nullptr;
	query = nullptr;
	device_lost = false;
	unk_filter_index = -1;
	unused13 = 3;
	texcoord_indices[0] = -1;
	texcoord_indices[1] = -1;
	texcoord_indices[2] = -1;
	texcoord_indices[3] = -1;
}

// OFFSET: 0x004143b0, STATUS: COMPLETE
X360RenderTarget::~X360RenderTarget() {
	for (std::size_t i = 0; i < sizeof(unk_array_1) / sizeof(IUnknown*); i++) {
		if (unk_array_1[i] != nullptr) {
			unk_array_1[i]->Release();
			unk_array_1[i] = nullptr;
		}
	}
	for (std::size_t i = 0; i < sizeof(unk_array_2) / sizeof(IUnknown*); i++) {
		if (unk_array_2[i] != nullptr) {
			unk_array_2[i]->Release();
			unk_array_2[i] = nullptr;
		}
	}
	if (vertex_buffer_manager != nullptr) {
		delete vertex_buffer_manager;
		vertex_buffer_manager = nullptr;
	}
	if (index_buffer_manager != nullptr) {
		delete index_buffer_manager;
		index_buffer_manager = nullptr;
	}
	if (vertex_shader_manager != nullptr) {
		delete vertex_shader_manager;
		vertex_shader_manager = nullptr;
	}
	if (pixel_shader_manager != nullptr) {
		delete pixel_shader_manager;
		pixel_shader_manager = nullptr;
	}
	if (blt_vertex != nullptr) {
		delete blt_vertex;
		blt_vertex = nullptr;
	}
	if (blt_pixel != nullptr) {
		delete blt_pixel;
		blt_pixel = nullptr;
	}
}

// OFFSET: 0x00414b40, STATUS: COMPLETE
void X360RenderTarget::ApplyViewportImpl(int clear_surface, int clear_depth_buffer, unsigned int unused0, unsigned int unused1) {
	vertex_shader_manager->Reset();
	pixel_shader_manager->Reset();
	g_D3DDevice9->SetViewport(&viewport);

	DWORD flags = 0;
	if (clear_surface != 0) {
		flags |= D3DCLEAR_TARGET;
	}
	if (clear_depth_buffer != 0) {
		flags |= (D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
	}

	D3DRECT viewport_rect{ viewport.X, viewport.Y, viewport.X + viewport.Width, viewport.Y + viewport.Height };
	g_D3DDevice9->Clear(1, &viewport_rect, flags, 0, 0.0, 0);

	float w = std::abs(static_cast<float>(viewport.Width));
	float h = std::abs(static_cast<float>(viewport.Height));
	Vector4 constant{ 2.0f / w, -2.0f / h, -1.0f, 1.0f };
	g_D3DDevice9->SetVertexShaderConstantF(34, &constant.x, 1);
	g_D3DDevice9->SetVertexShaderConstantF(35, &constant.x, 1);
}

bool X360RenderTarget::CheckDeviceFormatImpl(D3DFORMAT)
{
	return false;
}

void X360RenderTarget::Clear()
{
}

int X360RenderTarget::DrawCursorImpl()
{
	return 0;
}

int X360RenderTarget::DrawFullscreenEffects()
{
	return 0;
}

int X360RenderTarget::EndFrame()
{
	return 0;
}

void X360RenderTarget::IncrementFrameCount()
{
}

D3DFORMAT X360RenderTarget::MapToD3DFormat(UnkPixelFormat)
{
	return D3DFORMAT();
}

int X360RenderTarget::Recreate()
{
	return 0;
}

void X360RenderTarget::Reset()
{
}

void X360RenderTarget::SetCamera(Camera*)
{
}

void X360RenderTarget::SetUnkViewportIndex(int)
{
}

bool X360RenderTarget::StartFrame()
{
	return false;
}

void X360RenderTarget::Create(VideoCard* unused)
{
}

void X360RenderTarget::SetDimensions(VideoCard* unused)
{
}

// OFFSET: 0x00414cd0, STATUS: COMPLETE
void X360RenderTarget::ApplyViewport() {
	ApplyViewportImpl(1, 1, 1, 1);
}

void X360RenderTarget::DrawCursor()
{
}

void X360RenderTarget::Blt(unsigned int unused, TextureMap* tex, int alpha_blend, D3DCOLOR color)
{
}

int X360RenderTarget::Unk6(int, int, int)
{
	return 0;
}

int X360RenderTarget::Unk7(int)
{
	return 0;
}

void X360RenderTarget::Unk8(int)
{
}

void X360RenderTarget::Unk9()
{
}

int X360RenderTarget::Unk10(int, int)
{
	return 0;
}

bool X360RenderTarget::CheckDeviceFormat(UnkPixelFormat)
{
	return false;
}

int X360RenderTarget::SetViewport(D3DVIEWPORT9*)
{
	return 0;
}

void X360RenderTarget::SetFogDistances(float min, float max)
{
}
