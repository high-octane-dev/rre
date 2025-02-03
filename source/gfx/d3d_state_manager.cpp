#include "d3d_state_manager.hpp"
#include "x360_video_card.hpp"

D3DStateManager* g_lpD3DStateManager = nullptr;

// OFFSET: 0x004157c0, STATUS: COMPLETE
D3DStateManager::D3DStateManager() {
	D3DCAPS9 caps{};
	g_D3DDevice9->GetDeviceCaps(&caps);
	max_samplers = 16;
	fullscreen_effects_enabled = 0;
	sampler_states = new SamplerStateCache[max_samplers];
	min_sampler_state_type_used = new unsigned int [max_samplers];
	max_sampler_state_type_used = new unsigned int [max_samplers];
	Reset();
}

// OFFSET: 0x00415550, STATUS: COMPLETE
D3DStateManager::~D3DStateManager() {
	delete[] sampler_states;
	delete[] min_sampler_state_type_used;
	delete[] max_sampler_state_type_used;
}

// OFFSET: 0x00415590, STATUS: COMPLETE
void D3DStateManager::Reset() {
	for (auto& render_state : render_states) {
		render_state.value = -1;
		render_state.previous_value = -1;
	}
	for (std::size_t i = 0; i < 16; i++) {
		for (auto& sampler_state_variable : sampler_states[i].sampler_desc) {
			sampler_state_variable.value = -1;
			sampler_state_variable.previous_value = -1;
		}
	}
	g_D3DDevice9->SetRenderState(D3DRS_ALPHAREF, 0);
	render_states[D3DRS_ALPHAREF].value = 0;
	render_states[D3DRS_ALPHAREF].previous_value = 0;
	min_render_state_type = MAX_RENDER_STATE_TYPE;
	max_render_state_type = 0;
	min_sampler_used = max_samplers;
	max_sampler_used = 0;
	for (std::size_t i = 0; i < max_samplers; i++) {
		min_sampler_state_type_used[i] = MAX_SAMPLER_STATE_TYPE;
		max_sampler_state_type_used[i] = 0;
	}
}

// OFFSET: 0x00415660, STATUS: COMPLETE
void D3DStateManager::SendData() {
	if (min_render_state_type != MAX_RENDER_STATE_TYPE) {
		CachedValue* render_state = &render_states[min_render_state_type];
		for (std::size_t i = 0; i <= max_render_state_type - min_render_state_type; i++) {
			std::size_t render_state_index = min_render_state_type + i;
			DWORD value_to_set = render_state->value;

			if (fullscreen_effects_enabled && (render_state_index == D3DRS_ALPHATESTENABLE || render_state_index == D3DRS_ALPHABLENDENABLE)) {
				value_to_set = 0;
			}

			if (value_to_set != render_state->previous_value) {
				g_D3DDevice9->SetRenderState(static_cast<D3DRENDERSTATETYPE>(render_state_index), value_to_set);
				render_state->previous_value = value_to_set;
			}

			render_state++;
		}
		min_render_state_type = MAX_RENDER_STATE_TYPE;
		max_render_state_type = 0;
	}

	if (min_sampler_used != max_samplers) {
		for (std::size_t sampler_index = min_sampler_used; sampler_index <= max_sampler_used; sampler_index++) {
			CachedValue* sampler_state = &sampler_states[sampler_index].sampler_desc[min_sampler_state_type_used[sampler_index]];
			for (std::size_t i = 0; i <= max_sampler_state_type_used[sampler_index] - min_sampler_state_type_used[sampler_index]; ++i) {
				std::size_t sampler_state_type = min_sampler_state_type_used[sampler_index] + i;

				if (sampler_state->value != sampler_state->previous_value) {
					g_D3DDevice9->SetSamplerState(sampler_index, static_cast<D3DSAMPLERSTATETYPE>(sampler_state_type), sampler_state->value);
					sampler_state->previous_value = sampler_state->value;
				}

				sampler_state++;
			}

			min_sampler_state_type_used[sampler_index] = MAX_SAMPLER_STATE_TYPE;
			max_sampler_state_type_used[sampler_index] = 0;
		}

		min_sampler_used = max_samplers;
		max_sampler_used = 0;
	}
}

// OFFSET: 0x004025c0, STATUS: COMPLETE
void D3DStateManager::SetInitialRenderState(D3DRENDERSTATETYPE type, DWORD value) {
	g_D3DDevice9->SetRenderState(type, value);
	render_states[type].value = value;
	render_states[type].previous_value = value;
}

// OFFSET: INLINE, STATUS: COMPLETE
void D3DStateManager::SetRenderState(D3DRENDERSTATETYPE type, DWORD value) {
	render_states[type].value = value;
	if (render_states[type].previous_value != value) {
		if (static_cast<unsigned int>(type) < min_render_state_type) {
			min_render_state_type = static_cast<unsigned int>(type);
		}
		if (max_render_state_type < static_cast<unsigned int>(type)) {
			max_render_state_type = static_cast<unsigned int>(type);
		}
	}
}

// OFFSET: 0x00404bd0, STATUS: COMPLETE
void D3DStateManager::SetSamplerState(DWORD sampler_index, D3DSAMPLERSTATETYPE sampler_state_type, DWORD value) {
	sampler_states[sampler_index].sampler_desc[sampler_state_type].value = value;
	if (value != sampler_states[sampler_index].sampler_desc[sampler_state_type].previous_value) {
		if (sampler_index < min_sampler_used) {
			min_sampler_used = sampler_index;
		}
		if (max_sampler_used < sampler_index) {
			max_sampler_used = sampler_index;
		}
		if (static_cast<unsigned int>(sampler_state_type) < min_sampler_state_type_used[sampler_index]) {
			min_sampler_state_type_used[sampler_index] = static_cast<unsigned int>(sampler_state_type);
		}
		if (max_sampler_state_type_used[sampler_index] < static_cast<unsigned int>(sampler_state_type)) {
			max_sampler_state_type_used[sampler_index] = static_cast<unsigned int>(sampler_state_type);
		}
	}
}
