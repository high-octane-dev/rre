#include <algorithm>
#include "gfx/emotion_engine.hpp"
#include "x360_video_card.hpp"


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
int EE::D3DDeviceManager::IsResolutionAvailable(int w, int h) {
	D3DAdapter* current_adapter = adapter_list.data[adapter_index];
	
	for (std::size_t i = 0; i < current_adapter->display_mode_list.len; ++i) {
		D3DDisplayMode* current_mode = current_adapter->display_mode_list.data[i];

		if (current_mode->format == this->surface_format &&
			current_mode->width == w &&
			current_mode->height == h) {
			return true;
		}
	}

	return false;
}

// OFFSET: 0x0063b1d0
int EE::D3DDeviceManager::IsMultiSamplingEnabled()
{
	return presentation_params.MultiSampleType != D3DMULTISAMPLE_NONE;
}

// OFFSET: 0x0063cba0
void EE::D3DDeviceManager::Initialize()
{
	std::uint32_t adapter_count = d3d9->GetAdapterCount();
	for (std::size_t i = 0; i < adapter_count; i++) {
		D3DAdapter* adapter = new D3DAdapter(i);
		adapter->AddRef();
		if (adapter->CreateBackBuffers(d3d9, device_type)) {
			adapter_list.InsertRefObject(adapter_list.len, adapter);
		}
		adapter->Release();
	}
}

// OFFSET: 0x0063b610
const char* EE::D3DDeviceManager::GetRelativeShaderPath() {
	unsigned int pixel_shader_version = adapter_list.data[adapter_index]->capabilities.PixelShaderVersion;
	if (pixel_shader_version > 0xffff02ff) {
		if (g_VideoCard->IsFXEnabled()) {
			return "3\\";
		}
		return "3.nofx\\";
	}
	if (pixel_shader_version < 0xffff0200) {
		return "1\\";
	}
	if (g_VideoCard->IsFXEnabled()) {
		return "2\\";
	}
	return "2.nofx\\";
}

// OFFSET: 0x0063b470
IDirect3DTexture9* EE::D3DDeviceManager::GetBackBuffer() {
	IDirect3DSurface9* back_buffer_surface = this->back_buffer;
	if (back_buffer_surface == nullptr) {
		return nullptr;
	}

	D3DSURFACE_DESC surface_desc;
	back_buffer_surface->GetDesc(&surface_desc);

	unsigned int texture_flag = 0x400;
	IDirect3DTexture9* new_texture = nullptr;
	HRESULT result = device->CreateTexture(
		0x400,
		0x200,
		1,
		1,
		surface_desc.Format,
		D3DPOOL_DEFAULT,
		&new_texture,
		nullptr
	);

	return result >= 0 ? new_texture : nullptr;
}

// OFFSET: 0x0063b7d0
D3DFORMAT EE::D3DDeviceManager::GetBestFormat(D3DAdapter* adapter)
{
	return D3DFORMAT::D3DFMT_A1;
}

// OFFSET: 0x0063b930
D3DFORMAT EE::D3DDeviceManager::GetBestDepthStencilFormat(D3DAdapter* adapter, int back_buffer_index)
{
	return D3DFORMAT::D3DFMT_A1;
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
bool EE::D3DDeviceManager::CheckAdapterDisplayMode(D3DAdapter* adapter, D3DFORMAT desired_format, int* desired_width, int* desired_height)
{
	int closest_width = 0;
	int closest_height = 0;

	for (std::size_t i = 0; i < adapter->display_mode_list.len; i++) {
		D3DDisplayMode* current_mode = adapter->display_mode_list.data[i];
		if (current_mode->format == desired_format) {

			int width_difference = std::abs(current_mode->width - *desired_width);
			int closest_width_difference = std::abs(closest_width - *desired_width);

			int height_difference = std::abs(current_mode->height - *desired_height);
			int closest_height_difference = std::abs(closest_height - *desired_height);

			if (width_difference <= closest_width_difference && height_difference <= closest_height_difference) {
				closest_width = current_mode->width;
				closest_height = current_mode->height;
			}
		}
	}

	return closest_width != 0;
}

// OFFSET: 0x0063b580
void EE::D3DDeviceManager::SetMultiSampleType(bool enable) {
	if (enable) {
		int result = this->d3d9->CheckDeviceMultiSampleType(
			this->adapter_index,
			this->device_type,
			this->presentation_params.BackBufferFormat,
			this->windowed,
			D3DMULTISAMPLE_4_SAMPLES,
			nullptr
		);

		if (SUCCEEDED(result)) {
			this->presentation_params.Flags = 0;
			this->presentation_params.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
			return;
		}

		result = this->d3d9->CheckDeviceMultiSampleType(
			this->adapter_index,
			this->device_type,
			this->presentation_params.BackBufferFormat,
			this->windowed,
			D3DMULTISAMPLE_2_SAMPLES,
			nullptr
		);

		if (SUCCEEDED(result)) {
			this->presentation_params.Flags = 0;
			this->presentation_params.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
			return;
		}
	}

	this->presentation_params.MultiSampleType = D3DMULTISAMPLE_NONE;
	this->presentation_params.Flags = 1;
}

// OFFSET: 0x0063bac0
int EE::D3DDeviceManager::SetPresentationParams(HWND hwnd, D3DAdapter* adapter) {
	memset(&this->presentation_params, 0, sizeof(this->presentation_params));

	D3DFORMAT surface_format = GetBestFormat(adapter);
	this->surface_format = surface_format;
	if (surface_format == D3DFMT_UNKNOWN) {
		return false;
	}

	D3DFORMAT depth_stencil_format = GetBestDepthStencilFormat(adapter, surface_format);
	if (depth_stencil_format == D3DFMT_UNKNOWN) {
		return false;
	}

	int back_buffer_width = this->back_buffer_width;
	int back_buffer_height = this->back_buffer_height;

	if (!this->windowed) {
		bool is_valid_display_mode = CheckAdapterDisplayMode(
			adapter,
			surface_format,
			&back_buffer_width,
			&back_buffer_height
		);
		if (!is_valid_display_mode) {
			return false;
		}
	}

	this->presentation_params.AutoDepthStencilFormat = depth_stencil_format;
	this->presentation_params.BackBufferWidth = back_buffer_width;
	this->presentation_params.BackBufferHeight = back_buffer_height;
	this->presentation_params.BackBufferFormat = surface_format;
	this->presentation_params.BackBufferCount = this->back_buffer_count;
	this->presentation_params.Windowed = this->windowed;
	this->presentation_params.EnableAutoDepthStencil = true;
	this->presentation_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	this->presentation_params.MultiSampleType = this->multi_sample_type;
	this->presentation_params.MultiSampleQuality= 0;
	this->presentation_params.Flags = 0;
	this->presentation_params.FullScreen_RefreshRateInHz= 0;
	this->presentation_params.hDeviceWindow = hwnd;
	this->presentation_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT result = this->d3d9->CheckDeviceMultiSampleType(
		adapter->adapter_index,
		this->device_type,
		surface_format,
		this->windowed,
		this->multi_sample_type,
		nullptr
	);

	if (SUCCEEDED(result)) {
		this->presentation_params.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	if (this->presentation_params.BackBufferCount == 0) {
		this->presentation_params.BackBufferCount = 1;
	}
	else if (this->presentation_params.BackBufferCount > 3) {
		this->presentation_params.BackBufferCount = 3;
	}

	return true;
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

EE::D3DAdapter::D3DAdapter(unsigned int index) : RefObject(), display_mode_list(32), back_buffer_list(32)
{
	adapter_index = index;
}

bool EE::D3DAdapter::CreateBackBuffers(IDirect3D9* d3d9, D3DDEVTYPE device_type)
{
	return false;
}
