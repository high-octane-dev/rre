#include "gfx/emotion_engine.hpp"

EE::D3DDeviceManager* lpD3DDeviceManager = nullptr;

// OFFSET: INLINE
EE::RefObject::~RefObject()
{
}

// OFFSET: 0x0063c290
EE::D3DDeviceManager::D3DDeviceManager(IDirect3D9* new_d3d9) : adapter_list(32)
{
	this->d3d9 = nullptr;
	this->device = nullptr;
	this->back_buffer = nullptr;
	this->depth_stencil_surface = nullptr;
	if (d3d9 != new_d3d9) {
		if (d3d9 != nullptr) {
			d3d9->AddRef();
		}
		this->d3d9 = new_d3d9;
		if (new_d3d9 != nullptr) {
			new_d3d9->AddRef();
		}
	}
	this->n_depth_stencil_bits = 0x20;
	this->n_depth_bits = 0x18;
	this->n_stencil_bits = 8;
	this->back_buffer_count = 1;
	this->min_vertex_shader_version = 0xfffe0101;
	this->min_pixel_shader_version = 0xffff0104;
	this->windowed = true;
	this->back_buffer_width = 1;
	this->back_buffer_height = 1;
	this->behavior_flag_1 = 6;
	this->adapter_index = 0xffffffff;
	memset(&this->presentation_params, 0, 0x38);
	this->is_reference_device = false;
	this->behavior_flag_2 = 0x50;
	this->multi_sample_type = D3DMULTISAMPLE_NONE;
	this->device_type = D3DDEVTYPE_HAL;
}

// OFFSET: 0x0063c3d0
EE::D3DDeviceManager::~D3DDeviceManager()
{	
	if (depth_stencil_surface != nullptr) {
		depth_stencil_surface->Release();
	}
	if (back_buffer != nullptr) {
		back_buffer->Release();
	}
	if (device != nullptr) {
		device->Release();
	}
	if (d3d9 != nullptr) {
		d3d9->Release();
	}
}

// OFFSET: 0x0063b420
void EE::D3DDeviceManager::ReleaseResources()
{
	if (depth_stencil_surface != nullptr) {
		depth_stencil_surface->Release();
		depth_stencil_surface = nullptr;
	}
	if (back_buffer != nullptr) {
		back_buffer->Release();
		back_buffer = nullptr;
	}
	if (device != nullptr) {
		device->Release();
		device = nullptr;
	}
}

// OFFSET: 0x0063b2e0
int EE::D3DDeviceManager::IsResolutionAvailable(int w, int h)
{
	return 0;
}

// OFFSET: 0x0063b1d0
int EE::D3DDeviceManager::IsMultiSamplingEnabled()
{
	return presentation_params.MultiSampleType != D3DMULTISAMPLE_NONE;
}

// OFFSET: 0x0063cba0
void EE::D3DDeviceManager::Initialize()
{
}

// OFFSET: 0x0063b610
char* EE::D3DDeviceManager::GetRelativeShaderPath()
{
	return nullptr;
}

// OFFSET: 0x0063b470
IDirect3DTexture9* EE::D3DDeviceManager::GetBackBuffer()
{
	return nullptr;
}

// OFFSET: 0x0063b7d0
int EE::D3DDeviceManager::Unk(D3DAdapter* adapter)
{
	return 0;
}

// OFFSET: 0x0063b930
int EE::D3DDeviceManager::Unk2(D3DAdapter* adapter, int back_buffer_index)
{
	return 0;
}

// OFFSET: 0x0063bdb0
int EE::D3DDeviceManager::CreateAdapters(HWND hwnd)
{
	return 0;
}

// OFFSET: 0x0063cc70
int EE::D3DDeviceManager::Create()
{
	Initialize();
	return 1;
}

// OFFSET: 0x0063b230
void EE::D3DDeviceManager::CheckAdapterDisplayMode(D3DAdapter* adapter, D3DFORMAT fmt, int* w, int* h)
{
}

// OFFSET: 0x0063b580
void EE::D3DDeviceManager::SetMultiSampleType(bool enable)
{
}

// OFFSET: 0x0063bac0
int EE::D3DDeviceManager::SetPresentationParams(HWND hwnd, D3DAdapter* adapter)
{
	return 0;
}

// OFFSET: 0x0063b1b0
void EE::D3DDeviceManager::SetBackBufferResolution(int w, int h)
{
	presentation_params.BackBufferWidth = w;
	presentation_params.BackBufferHeight = h;
}

// OFFSET: 0x0063bbf0
int EE::D3DDeviceManager::SetBehaviorFlag(HWND hwnd, int adapter_index)
{
	return 0;
}

// OFFSET: 0x0063b4e0
void EE::D3DDeviceManager::ResolveFrameBuffer(IDirect3DTexture9* texture)
{
}

// OFFSET: 0x0063b340
void EE::D3DDeviceManager::ResetBackBufferAndDepthStencilSurface()
{
}

// OFFSET: 0x0063b3f0
void EE::D3DDeviceManager::Reset()
{
}

// OFFSET: 0x0063b010
void EE::D3DDepthStencil::ResolveQualityLevel(IDirect3D9* d3d9, D3DDEVTYPE dev_type, D3DFORMAT fmt, int windowed)
{
}
