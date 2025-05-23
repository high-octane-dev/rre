#pragma once
#include <d3d9.h>

class X360PixelShader {
private:
	IDirect3DPixelShader9* inner;
public:
	X360PixelShader();
	~X360PixelShader();
	bool Create(const char* name, int unused);
	inline IDirect3DPixelShader9* Get() {
		return inner;
	}
};

class X360VertexShader {
private:
	IDirect3DVertexShader9* inner[2];
	char name[260];
public:
	X360VertexShader();
	~X360VertexShader();
	bool Create(const char* name, int index);
	inline IDirect3DVertexShader9* Get(int index) {
		return inner[index];
	}
	inline const char* GetName() {
		return name;
	}
};
