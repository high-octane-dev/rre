#include "x360_render_target.hpp"
#include "x360_video_card.hpp"
#include "math/vector4.hpp"
#include "cursor.hpp"
#include "x360_game.hpp"
#include "d3d_state_manager.hpp"
#include "emotion_engine.hpp"
#include "x360_vertex_attribute_format_string_table.hpp"

X360RenderTarget* g_RenderTarget = nullptr;
int g_RenderTargetVAFIndex = -1;

// OFFSET: 0x00566470, STATUS: COMPLETE
UnkPixelFormat MapToUnkPixelFormat(D3DFORMAT param_1) {
	switch (param_1) {
	case 5:
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
		return UnkPixelFormat::A8R8G8B8;
	default:
		return UnkPixelFormat::AlsoInvalid;
	case D3DFMT_R5G6B5:
		return UnkPixelFormat::R5G6B5;
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
		return UnkPixelFormat::A1R5G5B5;
	case D3DFMT_A4R4G4B4:
		return UnkPixelFormat::A4R4G4B4;
	case D3DFMT_A8:
		return UnkPixelFormat::A8;
	}
}

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

	D3DRECT viewport_rect{ static_cast<long>(viewport.X), static_cast<long>(viewport.Y), static_cast<long>(viewport.X + viewport.Width), static_cast<long>(viewport.Y + viewport.Height) };
	g_D3DDevice9->Clear(1, &viewport_rect, flags, 0, 0.0, 0);

	float w = std::abs(static_cast<float>(viewport.Width));
	float h = std::abs(static_cast<float>(viewport.Height));
	Vector4 constant{ 2.0f / w, -2.0f / h, -1.0f, 1.0f };
	g_D3DDevice9->SetVertexShaderConstantF(34, &constant.x, 1);
	g_D3DDevice9->SetVertexShaderConstantF(35, &constant.x, 1);
	StartFrame(0);
}

// OFFSET: 0x004140b0, STATUS: COMPLETE
bool X360RenderTarget::CheckDeviceFormatImpl(D3DFORMAT format) {
	IDirect3D9* d3d9 = nullptr;
	D3DDEVICE_CREATION_PARAMETERS creation_params{};
	if (SUCCEEDED(g_D3DDevice9->GetDirect3D(&d3d9))) {
		if (SUCCEEDED(g_D3DDevice9->GetCreationParameters(&creation_params))) {
			d3d9->Release();
			return SUCCEEDED(d3d9->CheckDeviceFormat(creation_params.AdapterOrdinal, creation_params.DeviceType, D3DFMT_UNKNOWN, 0, D3DRTYPE_TEXTURE, format));
		}
	}
	return false;
}

// OFFSET: 0x00413bd0, STATUS: COMPLETE
void X360RenderTarget::Clear() {
	vertex_shader_manager->current_vertex_format_index = -1;
	for (std::size_t i = 0; i < 16; i++) {
		g_D3DDevice9->SetStreamSource(i, nullptr, 0, 0);
	}
	for (std::size_t i = 0; i < 4; i++) {
		g_D3DDevice9->SetTexture(i, nullptr);
	}
	return;
}

// OFFSET: 0x004148e0, STATUS: COMPLETE
int X360RenderTarget::DrawCursorImpl() {
	D3DVIEWPORT9 cursor_viewport{};
	if (g_lpCursor != nullptr) {
		cursor_viewport.MinZ = viewport.MinZ;
		cursor_viewport.Y = 0;
		cursor_viewport.X = 0;
		cursor_viewport.MaxZ = viewport.MaxZ;
		cursor_viewport.Height = g_ScreenEffectHeight;
		cursor_viewport.Width = g_ScreenEffectWidth;
		g_D3DDevice9->SetViewport(&cursor_viewport);
		g_lpCursor->Draw();
		g_D3DDevice9->SetViewport(&viewport);
	}
	g_D3DDevice9->EndScene();
	return 1;
}

// OFFSET: 0x00414990, STATUS: COMPLETE
int X360RenderTarget::DrawFullscreenEffects() {
	if (unk_texture == nullptr) {
		return 0;
	}
	vertex_shader_manager->Reset();
	pixel_shader_manager->Reset();
	pixel_shader_manager->SetIsFullscreenEffect(1);
	vertex_shader_manager->SetIsFullscreenEffect(1);
	g_lpD3DStateManager->fullscreen_effects_enabled = 1;

	IDirect3DSurface9* unk_surface = nullptr;
	if (SUCCEEDED(unk_texture->GetSurfaceLevel(0, &unk_surface))) {
		g_D3DDevice9->SetRenderTarget(0, unk_surface);
		unk_surface->Release();
	}
	g_D3DDevice9->SetDepthStencilSurface(depth_stencil_surface);

	D3DRECT rect{ 0, 0, 1024, 1024 };
	g_D3DDevice9->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x000000FF, 0.0, 0);

	float w = std::abs(static_cast<float>(viewport.Width));
	float h = std::abs(static_cast<float>(viewport.Height));
	Vector4 constant{ 2.0f / w, -2.0f / h, -1.0f, 1.0f };
	g_D3DDevice9->SetVertexShaderConstantF(34, &constant.x, 1);
	g_D3DDevice9->SetVertexShaderConstantF(35, &constant.x, 1);
	return static_cast<int>(StartFrame(1));
}

// OFFSET: 0x00414690, STATUS: COMPLETE
int X360RenderTarget::EndFrame()  {
	g_D3DDevice9->EndScene();
	g_D3DDevice9->SetRenderState(D3DRS_DEPTHBIAS, 0);
	g_D3DDevice9->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	pixel_shader_manager->SetIsFullscreenEffect(0);
	vertex_shader_manager->SetIsFullscreenEffect(0);
	g_lpD3DStateManager->fullscreen_effects_enabled = 0;
	g_D3DDevice9->SetRenderTarget(0, g_lpD3DDeviceManager->back_buffer);
	g_D3DDevice9->SetDepthStencilSurface(g_lpD3DDeviceManager->depth_stencil_surface);
	return 1;
}

// OFFSET: 0x0055de10, STATUS: COMPLETE
void X360RenderTarget::IncrementFrameCount() {
	frame_count++;
}

// OFFSET: 0x00566420, STATUS: COMPLETE
D3DFORMAT X360RenderTarget::MapToD3DFormat(UnkPixelFormat param_1) {
	if (param_1 == UnkPixelFormat::A1R5G5B5) {
		return D3DFMT_A1R5G5B5;
	}
	if (param_1 == UnkPixelFormat::A8) {
		return D3DFMT_A8;
	}
	if (param_1 == UnkPixelFormat::R5G6B5) {
		return D3DFMT_R5G6B5;
	}
	if (param_1 == UnkPixelFormat::A4R4G4B4) {
		return D3DFMT_A4R4G4B4;
	}
	if (param_1 == UnkPixelFormat::A8R8G8B8) {
		return D3DFMT_A8R8G8B8;
	}
	return D3DFMT_UNKNOWN;
}	

// OFFSET: 0x004145c0, STATUS: WIP
int X360RenderTarget::Recreate() {
	// Blocked on CarsUIManager, CarsUIVideoManager, X360MediaControl, and X360FilterManager
	if (g_lpD3DDeviceManager->Reset()) {
		/*
		if (lpUIManager != NULL && lpUIManager->UIVideoManager != NULL && lpUIManager->UIVideoManager->X360MediaControl != NULL) {
			X360MediaControl::FUN_00417b80(lpUIManager->UIVideoManager->X360MediaControl);
		}
		*/
		g_D3DDevice9->CreateQuery(D3DQUERYTYPE_OCCLUSION, &query);
		g_D3DDevice9->CreateDepthStencilSurface(1024, 1024, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, 1, &depth_stencil_surface, 0);
		g_D3DDevice9->CreateTexture(1024, 1024, 1, 1, D3DFMT_R32F, D3DPOOL_DEFAULT, &unk_texture, 0);
		/*
		if (lpGlobalFilterManager != NULL) {
			X360FilterManager::FUN_0040dd20(lpGlobalFilterManager);
		}
		*/
		g_lpD3DStateManager->Reset();
		SetDimensions(g_VideoCard);
		return 1;
	}
	return 0;
}

// OFFSET: 0x00414530, STATUS: WIP
void X360RenderTarget::Reset() {
	if (query != nullptr) {
		query->Release();
		query = nullptr;
	}
	if (depth_stencil_surface != nullptr) {
		depth_stencil_surface->Release();
		depth_stencil_surface = nullptr;
	}
	if (unk_texture != nullptr) {
		unk_texture->Release();
		unk_texture = nullptr;
	}
	/*
	if (lpGlobalFilterManager != NULL) {
		X360FilterManager::FUN_0040dcd0(lpGlobalFilterManager);
	}
	if (lpUIManager != NULL && lpUIManager->UIVideoManager != NULL && lpUIManager->UIVideoManager->X360MediaControl != NULL) {
		X360MediaControl::FUN_00417920(lpUIManager->UIVideoManager->X360MediaControl);
	}
	*/
	if (g_lpD3DDeviceManager->depth_stencil_surface != nullptr) {
		g_lpD3DDeviceManager->depth_stencil_surface->Release();
		g_lpD3DDeviceManager->depth_stencil_surface = nullptr;
	}
	if (g_lpD3DDeviceManager->back_buffer != nullptr) {
		g_lpD3DDeviceManager->back_buffer->Release();
		g_lpD3DDeviceManager->back_buffer = nullptr;
	}
}

// OFFSET: 0x0055de20, STATUS: WIP
void X360RenderTarget::SetCamera(Camera* _cam) {
	camera = _cam;
	/*
	if (_cam != nullptr && (width != _cam->max_viewport_width || (height != _cam->max_viewport_height))) {
		_cam->SurfaceChanged(g_AspectRatios[g_ScreenMode], g_CameraWidth, g_CameraHeight);
	}
	*/
}

// OFFSET: 0x00413b90, STATUS: COMPLETE
void X360RenderTarget::SetUnkViewportIndex(int index) {
	unk_viewport_index = index;
}

// OFFSET: 0x00414850, STATUS: COMPLETE
bool X360RenderTarget::StartFrame(int unk_recreate_if_lost) {
	HRESULT result = g_D3DDevice9->TestCooperativeLevel();
	if (result == D3DERR_DEVICELOST) {
		if (!device_lost) {
			device_lost = true;
			Reset();
		}
	}
	else {
		if (result != D3DERR_DEVICENOTRESET) {
			device_lost = false;
			return SUCCEEDED(g_D3DDevice9->BeginScene());
		}
		if (unk_recreate_if_lost != 0) {
			if (!device_lost) {
				device_lost = true;
				Reset();
			}
			if (Recreate() != 0) {
				device_lost = false;
				return SUCCEEDED(g_D3DDevice9->BeginScene());
			}
		}
	}
	return false;
}

// OFFSET: 0x00413800, STATUS: COMPLETE
void X360RenderTarget::Create(VideoCard* unused) {
	fog_uninitialized = 0;
	fog_color = 0;
	vertex_buffer_manager = new X360VertexBufferManager();
	index_buffer_manager = new X360IndexBufferManager();
	g_D3DDevice9->CreateQuery(D3DQUERYTYPE_OCCLUSION, &query);
	
	D3DDISPLAYMODE mode{};
	static_cast<X360Game*>(lpGame)->d3d9->GetAdapterDisplayMode(0, &mode);
	if (g_VideoCard->IsFXEnabled()) {
		g_D3DDevice9->CreateDepthStencilSurface(1024, 1024, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, 1, &depth_stencil_surface, 0);
		g_D3DDevice9->CreateTexture(1024, 1024, 1, 1, D3DFMT_R32F, D3DPOOL_DEFAULT, &unk_texture, 0);

		shadow_vertex = new X360VertexShader();
		shadow_vertex->Create("360_Shadow.hlsl", 0);
		
		shadow_pixel= new X360PixelShader();
		shadow_pixel->Create("360_Shadow.hlsl", 0);

		shadow_skinned_vertex = new X360VertexShader();
		shadow_skinned_vertex->Create("360_Shadow_Skinned.hlsl", 0);

		shadow_puppet_vertex = new X360VertexShader();
		shadow_puppet_vertex->Create("360_Shadow_Puppet.hlsl", 0);
	}
	
	vertex_shader_manager = new X360VertexShaderManager();
	pixel_shader_manager = new X360PixelShaderManager();

	blt_vertex = new X360VertexShader();
	blt_vertex->Create("360_Blt.hlsl", 0);

	blt_pixel = new X360PixelShader();
	blt_pixel->Create("360_Blt.hlsl", 0);

	D3DVERTEXELEMENT9 elements[] = {
		D3DVERTEXELEMENT9{
			.Stream = 0,
			.Offset = 0,
			.Type = D3DDECLTYPE_FLOAT2,
			.Method = D3DDECLMETHOD_DEFAULT,
			.Usage = D3DDECLUSAGE_POSITION,
			.UsageIndex = 0,
		},
		D3DVERTEXELEMENT9{
			.Stream = 0,
			.Offset = 8,
			.Type = D3DDECLTYPE_FLOAT2,
			.Method = D3DDECLMETHOD_DEFAULT,
			.Usage = D3DDECLUSAGE_TEXCOORD,
			.UsageIndex = 0,
		},
		D3DVERTEXELEMENT9{
			.Stream = 0xFF,
			.Offset = 0,
			.Type = D3DDECLTYPE_UNUSED,
			.Method = D3DDECLMETHOD_DEFAULT,
			.Usage = D3DDECLUSAGE_POSITION,
			.UsageIndex = 0,
		},
	};

	g_RenderTargetVAFIndex = g_lpVertexAttributeFormatStringTable->AddItem("X360RenderTarget", elements);

}

// OFFSET: 0x00413ba0, STATUS: COMPLETE
void X360RenderTarget::SetDimensions(VideoCard* unused) {
	width = g_CameraWidth;
	height = g_CameraHeight;
	unk_pixel_format = MapToUnkPixelFormat(D3DFMT_A8R8G8B8);
}

// OFFSET: 0x00414cd0, STATUS: COMPLETE
void X360RenderTarget::ApplyViewport() {
	ApplyViewportImpl(1, 1, 1, 1);
}

// OFFSET: 0x00414cb0, STATUS: COMPLETE
void X360RenderTarget::DrawCursor() {
	DrawCursorImpl();
}

// OFFSET: 0x00413c30, STATUS: COMPLETE
int X360RenderTarget::Blt(FRECT* dest, TextureMap* texture, FRECT* unused0, unsigned int unused1, int use_linear, int alpha_blend, D3DCOLOR color) {
	if (device_lost) {
		return 1;
	}

	vertex_shader_manager->SetVertexFormatIndex(g_RenderTargetVAFIndex);
	vertex_shader_manager->SetVertexShader(blt_vertex, 0);
	pixel_shader_manager->SetPixelShader(blt_pixel);
	g_D3DDevice9->SetTexture(0, static_cast<X360TextureMap*>(texture)->texture);
	g_lpD3DStateManager->SetRenderState(D3DRS_ZENABLE, 0);
	g_lpD3DStateManager->SetRenderState(D3DRS_ALPHATESTENABLE, 0);
	g_lpD3DStateManager->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	if (use_linear == 0) {
		g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	}
	else {
		g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	}
	
	g_lpD3DStateManager->SetSamplerState(0, D3DSAMP_MIPFILTER, 0);

	if (alpha_blend == 0) {
		g_lpD3DStateManager->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	}
	else {
		g_lpD3DStateManager->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
		g_lpD3DStateManager->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_lpD3DStateManager->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	g_lpD3DStateManager->SendData();

	float clip_space_dest[4]{};
	if (dest == nullptr) {
		clip_space_dest[0] = -1.0f;
		clip_space_dest[3] = -1.0f;
		clip_space_dest[2] = 1.0f;
		clip_space_dest[1] = 1.0f;
	}
	else {
		float inv_half_width = 1.0f / (g_ViewportWidth * 0.5f);
		float inv_half_height = 1.0f / (g_ViewportHeight * 0.5f);
		clip_space_dest[0] = inv_half_width * dest->x1 - 1.0f;
		clip_space_dest[2] = inv_half_width * dest->x2 - 1.0f;
		clip_space_dest[1] = 1.0f - inv_half_height * dest->y1;
		clip_space_dest[3] = 1.0f - inv_half_height * dest->y2;
	}

	Vector4 shader_color_scale = {
		(color >> 16 & 0xFF) / 255.0f,
		(color >> 8 & 0xFF) / 255.0f,
		(color & 0xFF) / 255.0f,
		(color >> 24) / 255.0f,
	};

	float vertices[24]{};
	vertices[0] = clip_space_dest[0]; // Vertex 0 - Position
	vertices[1] = clip_space_dest[1];
	vertices[2] = 0.0f; // Vertex 0 - TexCoord
	vertices[3] = 0.0f;
	vertices[4] = clip_space_dest[2]; // Vertex 1 - Position
	vertices[5] = clip_space_dest[1];
	vertices[6] = 1.0f; // Vertex 1 - TexCoord
	vertices[7] = 0.0f;
	vertices[8] = clip_space_dest[0]; // Vertex 2 - Position
	vertices[9] = clip_space_dest[3];
	vertices[10] = 0.0f; // Vertex 2 - TexCoord
	vertices[11] = 1.0f;
	vertices[12] = clip_space_dest[0]; // Vertex 3 - Position
	vertices[13] = clip_space_dest[3];
	vertices[14] = 0.0f; // Vertex 3 - TexCoord
	vertices[15] = 1.0f;
	vertices[16] = clip_space_dest[2]; // Vertex 4 - Position
	vertices[17] = clip_space_dest[1];
	vertices[18] = 1.0f; // Vertex 4 - TexCoord
	vertices[19] = 0.0f;
	vertices[20] = clip_space_dest[2]; // Vertex 5 - Position
	vertices[21] = clip_space_dest[3];
	vertices[22] = 1.0f; // Vertex 5 - TexCoord
	vertices[23] = 1.0f;

	g_D3DDevice9->SetPixelShaderConstantF(0, &shader_color_scale.x, 1);
	g_D3DDevice9->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, 16);
	g_D3DDevice9->SetTexture(0, nullptr);
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360RenderTarget::Unk6(int, int, int) {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360RenderTarget::Unk7(int) {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360RenderTarget::Unk8(int) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360RenderTarget::Unk9() {
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360RenderTarget::Unk10(int, int) {
	return 1;
}

// OFFSET: 0x00414140, STATUS: COMPLETE
bool X360RenderTarget::CheckDeviceFormat(UnkPixelFormat fmt) {
	return CheckDeviceFormatImpl(MapToD3DFormat(fmt));
}

// OFFSET: 0x00414060, STATUS: COMPLETE
int X360RenderTarget::SetViewport(D3DVIEWPORT9* new_viewport) {
	viewport.X = new_viewport->X;
	viewport.Y = new_viewport->Y;
	viewport.Width = new_viewport->Width;
	viewport.Height = new_viewport->Height;
	viewport.MinZ = new_viewport->MinZ;
	viewport.MaxZ = new_viewport->MaxZ;
	return 0;
}

// OFFSET: 0x00414160, STATUS: COMPLETE
void X360RenderTarget::SetFogDistances(float min, float max) {
	maximum_fog_distance = max;
	minimum_fog_distance = min;
	fog_parameters[0] = -1.0f / (max - min);
	fog_parameters[1] = max / (max - min);
	fog_parameters[2] = 20.0f;
	fog_parameters[3] = 0.001f;
	fog_uninitialized = 0;
}
