#include "x360_render_target.hpp"

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

X360RenderTarget::~X360RenderTarget()
{
}

void X360RenderTarget::ApplyViewportImpl()
{
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

void X360RenderTarget::Create(int unused)
{
}

void X360RenderTarget::SetDimensions(int unused0, int unused1)
{
}

void X360RenderTarget::ApplyViewport()
{
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
