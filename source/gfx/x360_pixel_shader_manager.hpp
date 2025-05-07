#pragma once
#include <d3d9.h>
#include "x360_shader.hpp"
#include "util/macros.hpp"

class X360PixelShaderManager {
private:
	IDirect3DPixelShader9* current_shader;
	int is_fullscreen_effect;
public:
	RRE_DISABLE_COPY(X360PixelShaderManager);
	X360PixelShaderManager();
	~X360PixelShaderManager();
	void Reset();
	void SetIsFullscreenEffect(int);
	void SetPixelShader(X360PixelShader*);
};