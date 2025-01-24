#include <algorithm>
#include "gfx/emotion_engine.hpp"
#include "x360_video_card.hpp"

EE::D3DDeviceManager* lpD3DDeviceManager = nullptr;

// OFFSET: INLINE, STATUS: COMPLETE
EE::RefObject::~RefObject() {
}

// OFFSET: 0x0063c290, STATUS: COMPLETE
EE::D3DDeviceManager::D3DDeviceManager(IDirect3D9* new_d3d9) : adapter_list(32) {
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

// OFFSET: 0x0063c3d0, STATUS: COMPLETE
EE::D3DDeviceManager::~D3DDeviceManager() {	
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

// OFFSET: 0x0063b420, STATUS: COMPLETE
void EE::D3DDeviceManager::ReleaseResources() {
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

// OFFSET: 0x0063b2e0, STATUS: COMPLETE
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

// OFFSET: 0x0063b1d0, STATUS: COMPLETE
int EE::D3DDeviceManager::IsMultiSamplingEnabled() {
	return presentation_params.MultiSampleType != D3DMULTISAMPLE_NONE;
}

// OFFSET: 0x0063cba0, STATUS: COMPLETE
void EE::D3DDeviceManager::Initialize() {
	std::uint32_t adapter_count = d3d9->GetAdapterCount();
	for (std::size_t i = 0; i < adapter_count; i++) {
		D3DAdapter* adapter = new D3DAdapter(i);
		adapter->AddRef();
		if (adapter->CreateBackBuffers(d3d9, device_type)) {
			adapter_list.Insert(adapter_list.len, &adapter);
		}
		adapter->Release();
	}
}

// OFFSET: 0x0063b610, STATUS: COMPLETE
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

// OFFSET: 0x0063b470, STATUS: COMPLETE
IDirect3DTexture9* EE::D3DDeviceManager::GetBackBuffer() {
	IDirect3DSurface9* back_buffer_surface = this->back_buffer;
	if (back_buffer_surface == nullptr) {
		return nullptr;
	}

	D3DSURFACE_DESC surface_desc;
	back_buffer_surface->GetDesc(&surface_desc);

	unsigned int texture_flag = 0x400;
	IDirect3DTexture9* new_texture = nullptr;
	HRESULT result = device->CreateTexture(0x400, 0x200, 1, 1, surface_desc.Format, D3DPOOL_DEFAULT, &new_texture, nullptr);

	return result >= 0 ? new_texture : nullptr;
}

// OFFSET: 0x0063b7d0, STATUS: TODO
D3DFORMAT EE::D3DDeviceManager::GetBestFormat(D3DAdapter* adapter) {
	// TODO: This is a stub for now. The real deal is quite complicated... :/
	return D3DFORMAT::D3DFMT_X8R8G8B8;
}

// OFFSET: 0x0063b930, STATUS: TODO
D3DFORMAT EE::D3DDeviceManager::GetBestDepthStencilFormat(D3DAdapter* adapter, int back_buffer_index) {
	// TODO: This is a stub for now. The real deal is quite complicated... :/
	return D3DFORMAT::D3DFMT_D24S8;
}

// OFFSET: 0x0063bdb0, STATUS: COMPLETE
int EE::D3DDeviceManager::CreateAdapters(HWND hwnd) {
	if (is_reference_device) {
		for (std::size_t i = 0; i < adapter_list.len; i++) {
			if (strstr(adapter_list.data[i]->identifier.Description, "PerfHUD") != nullptr) {
				if (CreateDeviceForAdapter(hwnd, i) != 0) {
					adapter_index = i;
					return 1;
				}
			}
		}
	}
	is_reference_device = false;
	device_type = D3DDEVTYPE_HAL;
	for (std::size_t i = 0; i < adapter_list.len; i++) {
		if (CreateDeviceForAdapter(hwnd, i) != 0) {
			this->adapter_index = i;
			return 1;
		}
	}
	adapter_index = 0xffffffff;
	return 0;
}

// OFFSET: 0x0063cc70, STATUS: COMPLETE
int EE::D3DDeviceManager::Create() {
	Initialize();
	return 1;
}

// OFFSET: 0x0063b230, STATUS: COMPLETE
bool EE::D3DDeviceManager::CheckAdapterDisplayMode(D3DAdapter* adapter, D3DFORMAT desired_format, int* desired_width, int* desired_height) {
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

// OFFSET: 0x0063b580, STATUS: COMPLETE
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

// OFFSET: 0x0063bac0, STATUS: COMPLETE
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

// OFFSET: 0x0063b1b0, STATUS: COMPLETE
void EE::D3DDeviceManager::SetBackBufferResolution(int w, int h) {
	presentation_params.BackBufferWidth = w;
	presentation_params.BackBufferHeight = h;
}

// OFFSET: 0x0063bbf0, STATUS: COMPLETE
int EE::D3DDeviceManager::CreateDeviceForAdapter(HWND hwnd, int adapter_index) {
	std::size_t current_flag_index = 0;
	DWORD test_flags[] = { 0x50, 0x40, 0x80, 0x20 };

	if (adapter_list.data[adapter_index]->capabilities.VertexShaderVersion < min_vertex_shader_version || adapter_list.data[adapter_index]->capabilities.PixelShaderVersion < min_pixel_shader_version) {
		current_flag_index = 2;
	}

	for (; current_flag_index < sizeof(test_flags) / sizeof(DWORD); current_flag_index++) {
		behavior_flag_2 = test_flags[current_flag_index];
		if (SetPresentationParams(hwnd, adapter_list.data[adapter_index]) != 0) {
			std::size_t bbc = presentation_params.BackBufferCount;
			IDirect3DDevice9* temp_device = nullptr;
			if (SUCCEEDED(d3d9->CreateDevice(adapter_index, device_type, hwnd, behavior_flag_1 | behavior_flag_2, &presentation_params, &temp_device))) {
				if (device != nullptr) {
					device->Release();
				}
				device = temp_device;
				ResetBackBufferAndDepthStencilSurface();
				device->SetSoftwareVertexProcessing(~(behavior_flag_2 >> 6) & 1);
				return 1;
			}
		}
	}
	return 0;
}

// OFFSET: 0x0063b4e0, STATUS: COMPLETE
bool EE::D3DDeviceManager::ResolveFrameBuffer(IDirect3DTexture9* texture) {
	D3DSURFACE_DESC desc{};
	IDirect3DSurface9* surface = nullptr;
	back_buffer->GetDesc(&desc);
	texture->GetSurfaceLevel(0, &surface);

	RECT src_rect{
		.left = 0,
		.top = 0,
		.right = static_cast<int>(desc.Width),
		.bottom = static_cast<int>(desc.Height),
	};

	RECT dst_rect{
		.left = 0,
		.top = 0,
		.right = 0x400,
		.bottom = 0x200,
	};

	HRESULT result = device->StretchRect(back_buffer, &src_rect, surface, &dst_rect, D3DTEXF_NONE);
	surface->Release();
	return SUCCEEDED(result);
}

// OFFSET: 0x0063b340, STATUS: COMPLETE
void EE::D3DDeviceManager::ResetBackBufferAndDepthStencilSurface() {
	IDirect3DSurface9* new_back_buffer = nullptr;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &new_back_buffer);
	if (back_buffer != nullptr) {
		back_buffer->Release();
	}
	back_buffer = new_back_buffer;

	IDirect3DSurface9* new_depth_stencil = nullptr;
	device->GetDepthStencilSurface(&new_depth_stencil);
	if (depth_stencil_surface != nullptr) {
		depth_stencil_surface->Release();
	}
	depth_stencil_surface = new_depth_stencil;
}

// OFFSET: 0x0063b3f0, STATUS: COMPLETE
bool EE::D3DDeviceManager::Reset() {
	if (SUCCEEDED(device->Reset(&presentation_params))) {
		ResetBackBufferAndDepthStencilSurface();
		return true;
	}
	return false;
}

// OFFSET: 0x0063b010, STATUS: COMPLETE
void EE::D3DDepthStencil::ResolveQualityLevel(IDirect3D9* d3d9, unsigned int adapter, D3DDEVTYPE device_type, D3DFORMAT depth_stencil_format) {
	this->unk_bitfield_windowed = 0;
	this->unk_bitfield_fullscreen = 0;

	for (int sample_count = 1; sample_count < 17; sample_count++) {
		HRESULT result;
		DWORD quality_level;

		if (this->windowed) {
			result = d3d9->CheckDeviceMultiSampleType(adapter, device_type, depth_stencil_format, TRUE, static_cast<D3DMULTISAMPLE_TYPE>(sample_count), &quality_level);

			if (SUCCEEDED(result)) {
				result = d3d9->CheckDeviceMultiSampleType(adapter, device_type, this->surface_format, TRUE, static_cast<D3DMULTISAMPLE_TYPE>(sample_count), &quality_level);

				if (SUCCEEDED(result)) {
					if (sample_count == 1) {
						this->windowed_quality_level = quality_level;
					}
					this->unk_bitfield_windowed |= (1 << sample_count);
				}
			}
		}

		if (this->fullscreen) {
			result = d3d9->CheckDeviceMultiSampleType(adapter, device_type, depth_stencil_format, FALSE, static_cast<D3DMULTISAMPLE_TYPE>(sample_count), &quality_level);

			if (SUCCEEDED(result)) {
				result = d3d9->CheckDeviceMultiSampleType(adapter, device_type, this->surface_format, FALSE, static_cast<D3DMULTISAMPLE_TYPE>(sample_count), &quality_level);

				if (SUCCEEDED(result)) {
					if (sample_count == 1) {
						this->fullscreen_quality_level = quality_level;
					}
					this->unk_bitfield_fullscreen |= (1 << sample_count);
				}
			}
		}
	}
}

// OFFSET: 0x0063c4a0, STATUS: COMPLETE
EE::D3DAdapter::D3DAdapter(unsigned int index) : RefObject(), display_mode_list(32), back_buffer_list(32), adapter_index(index) {
}

// OFFSET: 0x0063c580, STATUS: COMPLETE
EE::D3DAdapter::~D3DAdapter() {
}

// OFFSET: 0x0063bf30, STATUS: COMPLETE
void EE::D3DAdapter::AddDisplayMode(D3DDISPLAYMODE* _display_mode) {
	for (std::size_t i = 0; i < display_mode_list.len; i++) {
		if (display_mode_list.data[i]->format == _display_mode->Format && display_mode_list.data[i]->width == _display_mode->Width && display_mode_list.data[i]->height == _display_mode->Height) {
			unsigned int refresh_rate = _display_mode->RefreshRate;
			display_mode_list.data[i]->refresh_rate_list.InsertRaw(display_mode_list.data[i]->refresh_rate_list.len, &refresh_rate);
			return;
		}
	}
	D3DDisplayMode* new_display_mode = new D3DDisplayMode();
	new_display_mode->AddRef();
	new_display_mode->format = _display_mode->Format;
	new_display_mode->width = _display_mode->Width;
	new_display_mode->height = _display_mode->Height;
	unsigned int refresh_rate = _display_mode->RefreshRate;
	new_display_mode->refresh_rate_list.InsertRaw(new_display_mode->refresh_rate_list.len, &refresh_rate);
	display_mode_list.Insert(display_mode_list.len, &new_display_mode);
	new_display_mode->Release();
}

D3DFORMAT c_UnkFormatArray[] ={
	D3DFMT_X8R8G8B8,
	D3DFMT_R5G6B5,
	D3DFMT_X1R5G5B5,
	D3DFMT_A8R8G8B8,
	D3DFMT_D16_LOCKABLE,
	D3DFMT_D32,
	D3DFMT_D15S1,
	D3DFMT_D24S8,
	D3DFMT_D24X8,
	D3DFMT_D24X4S4,
	D3DFMT_D32F_LOCKABLE,
	D3DFMT_D24FS8,
	D3DFMT_D16,
	D3DFMT_A8R8G8B8,
};

// OFFSET: 0x0063c970, STATUS: COMPLETE
bool EE::D3DAdapter::CreateBackBuffers(IDirect3D9* d3d9, D3DDEVTYPE device_type) {
	if (FAILED(d3d9->GetDeviceCaps(adapter_index, device_type, &capabilities))) {
		return false;
	}
	d3d9->GetAdapterIdentifier(adapter_index, 0, &identifier);
	d3d9->GetAdapterDisplayMode(adapter_index, &display_mode);
	for (std::size_t i = 0; i < sizeof(c_UnkFormatArray) / sizeof(D3DFORMAT); i++) {
		std::size_t adapter_mode_count = d3d9->GetAdapterModeCount(adapter_index, c_UnkFormatArray[i]);
		for (std::size_t j = 0; j < adapter_mode_count; j++) {
			D3DDISPLAYMODE mode{};
			if (SUCCEEDED(d3d9->EnumAdapterModes(adapter_index, c_UnkFormatArray[i], j, &mode))) {
				AddDisplayMode(&mode);
				HRESULT fullscreen_result = d3d9->CheckDeviceType(adapter_index, device_type, mode.Format, D3DFMT_A8R8G8B8, 0);
				HRESULT windowed_result = d3d9->CheckDeviceType(adapter_index, device_type, display_mode.Format, D3DFMT_A8R8G8B8, 1);
				if (SUCCEEDED(fullscreen_result) || SUCCEEDED(windowed_result)) {
					
					bool already_exists = false;
					for (std::size_t k = 0; k < back_buffer_list.len; k++) {
						if (back_buffer_list.data[k]->surface_format == mode.Format) {
							already_exists = true;
						}
					}

					if (already_exists) {
						continue;
					}

					// FIXME: Not so sure about this one. Shouldn't we process fullscreen back buffers seperately?
					D3DBackBuffer* new_bb = new D3DBackBuffer(mode.Format, SUCCEEDED(fullscreen_result), SUCCEEDED(windowed_result));
					new_bb->AddRef();
					new_bb->CreateRenderTargets(d3d9, adapter_index, mode.Format, device_type);
					back_buffer_list.Insert(back_buffer_list.len, &new_bb);
					new_bb->Release();
				}
			}
		}
	}
	return true;
}

// OFFSET: 0x0063c710, STATUS: COMPLETE
EE::D3DRenderTarget::D3DRenderTarget(D3DFORMAT _format) : RefObject(), format(_format), depth_stencils(32), unused(0) {
}

// OFFSET: 0x0063c7b0, STATUS: COMPLETE
EE::D3DRenderTarget::~D3DRenderTarget() {
}

// OFFSET: 0x0063c0d0, STATUS: COMPLETE
void EE::D3DRenderTarget::CreateDepthStencils(IDirect3D9* d3d9, unsigned int adapter, D3DDEVTYPE device_type, D3DFORMAT adapter_format, D3DFORMAT depth_stencil_format, bool fullscreen, bool windowed) {
	for (std::size_t i = 4; i < 13; i++) {
		if (SUCCEEDED(d3d9->CheckDeviceFormat(adapter, device_type, adapter_format, 2, D3DRTYPE_SURFACE, c_UnkFormatArray[i]))) {
			if (SUCCEEDED(d3d9->CheckDepthStencilMatch(adapter, device_type, adapter_format, format, c_UnkFormatArray[i]))) {
				D3DDepthStencil* depth_stencil = new D3DDepthStencil();
				depth_stencil->AddRef();
				depth_stencil->ResolveQualityLevel(d3d9, adapter, device_type, depth_stencil_format);
				depth_stencils.Insert(depth_stencils.len, &depth_stencil);
				depth_stencil->Release();
			}
		}
	}
}

// OFFSET: 0x0063c600, STATUS: COMPLETE
EE::D3DBackBuffer::D3DBackBuffer(D3DFORMAT _format, bool _fullscreen, bool _windowed) : RefObject(), surface_format(_format), fullscreen(_fullscreen), windowed(_windowed), render_target_list(32), unused(0) {
}

// OFFSET: 0x0063c6b0, STATUS: COMPLETE
EE::D3DBackBuffer::~D3DBackBuffer() {
}

// OFFSET: 0x0063c0d0, STATUS: COMPLETE
void EE::D3DBackBuffer::CreateRenderTargets(IDirect3D9* d3d9, unsigned int adapter, D3DFORMAT render_target_format, D3DDEVTYPE device_type) {
	if (SUCCEEDED(d3d9->CheckDeviceFormat(adapter, device_type, render_target_format, 1, D3DRTYPE_SURFACE, D3DFMT_A8R8G8B8))) {
		D3DRenderTarget* render_target = new D3DRenderTarget(D3DFMT_A8R8G8B8);
		render_target->AddRef();
		render_target->CreateDepthStencils(d3d9, adapter, device_type, render_target_format, surface_format, fullscreen, windowed);
		if (render_target->unused > 0) {
			render_target_list.Insert(render_target_list.len, &render_target);
		}
		render_target->Release();
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
EE::D3DDisplayMode::D3DDisplayMode() : RefObject(), refresh_rate_list(32) {
}

// OFFSET: 0x0063c080, STATUS: COMPLETE
EE::D3DDisplayMode::~D3DDisplayMode() {
}
