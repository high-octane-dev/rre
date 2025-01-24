#pragma once
#include <d3d9.h>
#include "x360_shader.hpp"

class X360PixelShaderManager {
private:
	IDirect3DPixelShader9* current_shader;
	int is_fullscreen_effect;
public:
	X360PixelShaderManager();
	~X360PixelShaderManager();
	X360PixelShaderManager(const X360PixelShaderManager&) = delete;
	X360PixelShaderManager& operator=(const X360PixelShaderManager&) = delete;

	void Reset();
	void SetIsFullscreenEffect(int);
	void SetPixelShader(X360PixelShader*);
};