#pragma once
#include <array>
#include <d3d9.h>

class D3DStateManager {
public:
	struct RenderStateCache {
		int state_value;
		int set_state_value;
	};

	struct CachedValue {
		int value;
		int set_value;
	};

	struct SamplerStateCache {
		CachedValue Description[12];
	};

	struct SamplerState {
		std::array<SamplerStateCache, 16> render_states;
	};

	unsigned int fullscreen_effects_enabled;
	std::array<RenderStateCache, 210> render_states;
	SamplerState* sampler_states;
	unsigned int* unk_sampler_indexed1;
	unsigned int* unk_sampler_indexed2;
	unsigned int max_registers;
	unsigned int state_index;
	unsigned int unk_state_index;
	unsigned int sampler_state_index;
	unsigned int max_register_index;
public:
	D3DStateManager();
	~D3DStateManager();
	D3DStateManager(const D3DStateManager&) = delete;
	D3DStateManager& operator=(const D3DStateManager&) = delete;
	void Reset();
	void SendData();
	void SetRenderState(D3DRENDERSTATETYPE type, DWORD value);
	void SetSamplerState(D3DSAMPLERSTATETYPE sampler_state, D3DRENDERSTATETYPE render_state, DWORD value);

};

extern D3DStateManager* lpD3DStateManager;