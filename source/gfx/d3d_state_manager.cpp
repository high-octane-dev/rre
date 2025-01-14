#include "d3d_state_manager.hpp"
#include "x360_video_card.hpp"

D3DStateManager* lpD3DStateManager = nullptr;

// OFFSET: 0x004157c0
D3DStateManager::D3DStateManager() {
	D3DCAPS9 caps{};
	g_D3DDevice9->GetDeviceCaps(&caps);
	max_registers = 0x10;
	fullscreen_effects_enabled = 0;
	sampler_states = new SamplerState[16];
	unk_sampler_indexed1 = new unsigned int [max_registers];
	unk_sampler_indexed2 = new unsigned int [max_registers];
	Reset();
}

// OFFSET: 0x00415550
D3DStateManager::~D3DStateManager() {
	delete[] sampler_states;
	delete[] unk_sampler_indexed1;
	delete[] unk_sampler_indexed2;
}

// OFFSET: 0x00415590
void D3DStateManager::Reset() {
}

// OFFSET: 0x00415660
void D3DStateManager::SendData() {
}

// OFFSET: 0x004025c0
void D3DStateManager::SetRenderState(D3DRENDERSTATETYPE type, DWORD value) {
	g_D3DDevice9->SetRenderState(type, value);
	render_states[type].state_value = value;
	render_states[type].set_state_value = value;
}

// OFFSET: 0x00404bd0
void D3DStateManager::SetSamplerState(D3DSAMPLERSTATETYPE sampler_state, D3DRENDERSTATETYPE render_state, DWORD value) {
}
