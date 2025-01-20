#pragma once
#include <array>
#include <d3d9.h>

constexpr std::size_t MAX_RENDER_STATE_TYPE = static_cast<std::size_t>(D3DRS_BLENDOPALPHA) + 1;
constexpr std::size_t MAX_SAMPLER_STATE_TYPE = 48;

class D3DStateManager {
public:
	struct CachedValue {
		DWORD value;
		DWORD previous_value;
	};

	struct SamplerStateCache {
		std::array<CachedValue, 48> sampler_desc;
	};

	unsigned int fullscreen_effects_enabled;
	std::array<CachedValue, MAX_RENDER_STATE_TYPE> render_states;
	SamplerStateCache* sampler_states;
	unsigned int* min_sampler_state_type_used;
	unsigned int* max_sampler_state_type_used;
	unsigned int max_samplers;
	unsigned int min_render_state_type;
	unsigned int max_render_state_type;
	unsigned int min_sampler_used;
	unsigned int max_sampler_used;
public:
	D3DStateManager();
	~D3DStateManager();
	D3DStateManager(const D3DStateManager&) = delete;
	D3DStateManager& operator=(const D3DStateManager&) = delete;
	void Reset();
	void SendData();
	void SetInitialRenderState(D3DRENDERSTATETYPE type, DWORD value);
	void SetRenderState(D3DRENDERSTATETYPE type, DWORD value);
	void SetSamplerState(DWORD sampler_index, D3DSAMPLERSTATETYPE sampler_state_type, DWORD value);

};

extern D3DStateManager* lpD3DStateManager;