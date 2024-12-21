#pragma once
#include <d3d9.h>

class D3DStateManager {
public:

public:
	D3DStateManager();
	~D3DStateManager();
	D3DStateManager(const D3DStateManager&) = delete;
	D3DStateManager& operator=(const D3DStateManager&) = delete;
	void Reset();
	void SendData();
	void SetRenderState(D3DRENDERSTATETYPE type, DWORD value);
	void SetSamplerState(D3DSAMPLERSTATETYPE type, DWORD register_index, DWORD value);

};

extern D3DStateManager* lpD3DStateManager;