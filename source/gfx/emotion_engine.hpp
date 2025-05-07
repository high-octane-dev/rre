#pragma once
#include <d3d9.h>
#include <type_traits>
#include "util/macros.hpp"

namespace EE {
	class RefObject {
	protected:
		int ref_count;
	public:
		RRE_DISABLE_COPY(RefObject);
		virtual ~RefObject();
		inline void AddRef() {
			ref_count++;
		}
		inline void Release() {
			ref_count--;
			if (ref_count == 0) {
				delete this;
			}
		}
	};

	template <class T, typename = std::enable_if_t<std::is_base_of_v<RefObject, std::remove_pointer_t<T>>>> class RefObjectList {
	public:
		T* data;
		std::size_t capacity;
		std::size_t len;
	public:
		RRE_DISABLE_COPY(RefObjectList);
		RefObjectList(std::size_t initial_capacity) {
			capacity = initial_capacity;
			data = reinterpret_cast<T*>(malloc(initial_capacity * sizeof(T)));
			len = 0;
			std::memset(data, 0, initial_capacity);
		}
		~RefObjectList() {
			for (std::size_t i = 0; i < len; i++) {
				reinterpret_cast<RefObject*>(data[i])->Release();
			}
			free(data);
		}
		void Insert(std::size_t index, T* element) {
			if (index < len) {
				if (data[index] != nullptr) {
					reinterpret_cast<RefObject*>(data[index])->Release();
				}
				data[index] = *element;
				return;
			}
			if (index + 1 > capacity) {
				std::size_t new_capacity = capacity;
				do {
					new_capacity = new_capacity * 2 + 1;
				} while (new_capacity < index + 1);
				data = reinterpret_cast<T*>(realloc(data, new_capacity * sizeof(T)));
				capacity = new_capacity;
			}
			while (index + 1 > len) {
				data[len] = nullptr;
				len++;
			}
			data[index] = *element;
			if (*element != nullptr) {
				reinterpret_cast<RefObject*>(*element)->AddRef();
			}
		}
	};

	template <class T> class List {
	public:
		T* data;
		std::size_t capacity;
		std::size_t len;
	public:
		RRE_DISABLE_COPY(List);
		List(std::size_t initial_capacity) {
			capacity = initial_capacity;
			data = reinterpret_cast<T*>(malloc(initial_capacity * sizeof(T)));
			len = 0;
			std::memset(data, 0, initial_capacity);
		}
		~List() {
			free(data);
		}
		void InsertRaw(std::size_t index, T* element) {
			if (index < len) {
				data[index] = *element;
				return;
			}
			if (index + 1 > capacity) {
				std::size_t new_capacity = capacity;
				do {
					new_capacity = new_capacity * 2 + 1;
				} while (new_capacity < index + 1);
				data = reinterpret_cast<T*>(realloc(data, new_capacity * sizeof(T)));
				capacity = new_capacity;
			}
			while (index + 1 > len) {
				data[len] = T();
				len++;
			}
			data[index] = *element;
		}
	};

	class D3DDepthStencil : public RefObject {
	private:
		D3DFORMAT surface_format;
		bool fullscreen;
		bool windowed;
		unsigned int windowed_quality_level;
		unsigned int unk_bitfield_windowed;
		unsigned int fullscreen_quality_level;
		unsigned int unk_bitfield_fullscreen;
	public:
		void ResolveQualityLevel(IDirect3D9* d3d9, unsigned int adapter, D3DDEVTYPE device_type, D3DFORMAT depth_stencil_format);
	};

	class D3DRenderTarget : public RefObject {
	private:
		D3DFORMAT format;
		RefObjectList<D3DDepthStencil*> depth_stencils;
	public:
		int unused;
	public:
		D3DRenderTarget(D3DFORMAT _format);
		virtual ~D3DRenderTarget() override;
		void CreateDepthStencils(IDirect3D9* d3d9, unsigned int adapter, D3DDEVTYPE device_type, D3DFORMAT render_target_format, D3DFORMAT surface_format, bool fullscreen, bool windowed);
	};

	class D3DBackBuffer : public RefObject {
	public:
		D3DFORMAT surface_format;
		bool fullscreen;
		bool windowed;
		RefObjectList<D3DRenderTarget*> render_target_list;
		int unused;
	public:
		D3DBackBuffer(D3DFORMAT _format, bool _fullscreen, bool _windowed);
		virtual ~D3DBackBuffer() override;
		void CreateRenderTargets(IDirect3D9* d3d9, unsigned int adapter, D3DFORMAT render_target_format, D3DDEVTYPE device_type);
	};

	class D3DDisplayMode : public RefObject {
	public:
		D3DFORMAT format;
		int width;
		int height;
		List<unsigned int> refresh_rate_list;
	public:
		D3DDisplayMode();
		virtual ~D3DDisplayMode() override;
	};

	class D3DAdapter : public RefObject {
	public:
		unsigned int adapter_index;
		D3DADAPTER_IDENTIFIER9 identifier;
		D3DDISPLAYMODE display_mode;
		D3DCAPS9 capabilities;
		RefObjectList<D3DDisplayMode*> display_mode_list;
		RefObjectList<D3DBackBuffer*> back_buffer_list;
		int unused;
	public:
		D3DAdapter(unsigned int index);
		virtual ~D3DAdapter() override;
		bool CreateBackBuffers(IDirect3D9* d3d9, D3DDEVTYPE device_type);
		void AddDisplayMode(D3DDISPLAYMODE* display_modes);
	};

	class D3DDeviceManager : public RefObject {
	public:
		IDirect3D9* d3d9;
		RefObjectList<D3DAdapter*> adapter_list;
		unsigned int adapter_index;
		D3DPRESENT_PARAMETERS presentation_params;
		D3DFORMAT surface_format;
		D3DDEVTYPE device_type;
		unsigned int behavior_flag_2;
		D3DMULTISAMPLE_TYPE multi_sample_type;
		IDirect3DDevice9* device;
		IDirect3DSurface9* back_buffer;
		IDirect3DSurface9* depth_stencil_surface;
		bool windowed;
		int n_depth_stencil_bits;
		int n_depth_bits;
		int n_stencil_bits;
		int back_buffer_count;
		int back_buffer_width;
		int back_buffer_height;
		unsigned int min_vertex_shader_version;
		unsigned int min_pixel_shader_version;
		unsigned int behavior_flag_1;
		bool is_reference_device;
	public:
		D3DDeviceManager(IDirect3D9* _d3d9);
		virtual ~D3DDeviceManager() override;

		void SetMultiSampleType(bool enable);
		int SetPresentationParams(HWND hwnd, D3DAdapter* adapter);
		void SetBackBufferResolution(int w, int h);
		int CreateDeviceForAdapter(HWND hwnd, int adapter_index);
		bool ResolveFrameBuffer(IDirect3DTexture9* texture);
		void ResetBackBufferAndDepthStencilSurface();
		bool Reset();
		void ReleaseResources();
		int IsResolutionAvailable(int w, int h);
		int IsMultiSamplingEnabled();
		void Initialize();
		const char* GetRelativeShaderPath();
		IDirect3DTexture9* GetBackBuffer();
		D3DFORMAT GetBestFormat(D3DAdapter* adapter);
		D3DFORMAT GetBestDepthStencilFormat(D3DAdapter* adapter, int back_buffer_index);
		int CreateAdapters(HWND hwnd);
		int Create();
		bool CheckAdapterDisplayMode(D3DAdapter* adapter, D3DFORMAT fmt, int* w, int* h);
	};
#ifdef _M_IX86
	static_assert(sizeof(D3DDepthStencil) == 0x20);
	static_assert(sizeof(D3DRenderTarget) == 0x1C);
	static_assert(sizeof(D3DBackBuffer) == 0x20);
	static_assert(sizeof(D3DDisplayMode) == 0x20);
	static_assert(sizeof(D3DAdapter) == 0x5b4);
	static_assert(sizeof(D3DDeviceManager) == 0x9c);
#endif
};

extern EE::D3DDeviceManager* g_lpD3DDeviceManager;
