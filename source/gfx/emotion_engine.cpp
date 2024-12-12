#include "emotion_engine.hpp"

EE::RefObject::~RefObject()
{
}

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

int EE::D3DDeviceManager::IsResolutionAvailable(int w, int h)
{
	return 0;
}

int EE::D3DDeviceManager::IsMultiSamplingEnabled()
{
	return 0;
}

void EE::D3DDeviceManager::Initialize()
{
}

char* EE::D3DDeviceManager::GetRelativeShaderPath()
{
	return nullptr;
}

IDirect3DTexture9* EE::D3DDeviceManager::GetBackBuffer()
{
	return nullptr;
}

int EE::D3DDeviceManager::Unk(D3DAdapter* adapter)
{
	return 0;
}

int EE::D3DDeviceManager::Unk2(D3DAdapter* adapter, int back_buffer_index)
{
	return 0;
}

int EE::D3DDeviceManager::CreateAdapters(HWND hwnd)
{
	return 0;
}

int EE::D3DDeviceManager::Create()
{
	return 0;
}

void EE::D3DDeviceManager::CheckAdapterDisplayMode(D3DAdapter* adapter, D3DFORMAT fmt, int* w, int* h)
{
}

void EE::D3DDeviceManager::SetMultiSampleType(bool enable)
{
}

int EE::D3DDeviceManager::SetPresentationParams(HWND hwnd, D3DAdapter* adapter)
{
	return 0;
}

void EE::D3DDeviceManager::SetBackBufferResolution(int w, int h)
{
}

int EE::D3DDeviceManager::SetBehaviorFlag(HWND hwnd, int adapter_index)
{
	return 0;
}

void EE::D3DDeviceManager::ResolveFrameBuffer(IDirect3DTexture9* texture)
{
}

void EE::D3DDeviceManager::ResetBackBufferAndDepthStencilSurface()
{
}

void EE::D3DDeviceManager::Reset()
{
}
