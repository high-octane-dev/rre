#pragma once
#include <d3d9.h>
#include "x360_shader.hpp"
#include "util/macros.hpp"

class X360VertexShaderManager {
private:
	IDirect3DVertexShader9* current_shader;
public:
	int current_vertex_format_index;
private:
	int is_fullscreen_effect;
public:
	RRE_DISABLE_COPY(X360VertexShaderManager);
	X360VertexShaderManager();
	~X360VertexShaderManager();
	void Reset();
	void SetVertexFormatIndex(int);
	void SetIsFullscreenEffect(int);
	void SetVertexShader(X360VertexShader*, int);
};