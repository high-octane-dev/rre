#pragma once
#include <d3d9.h>
#include "x360_shader.hpp"

class X360VertexShaderManager {
private:
	IDirect3DVertexShader9* current_shader;
	int current_vertex_format_index;
	int is_fullscreen_effect;
public:
	X360VertexShaderManager();
	~X360VertexShaderManager();
	X360VertexShaderManager(const X360VertexShaderManager&) = delete;
	X360VertexShaderManager& operator=(const X360VertexShaderManager&) = delete;

	void Reset();
	void SetVertexFormatIndex(int);
	void SetVertexShader(X360VertexShader*);
};