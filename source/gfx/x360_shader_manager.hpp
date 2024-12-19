#pragma once
#include <d3d9.h>
#include <cstdint>
#include <cstddef>

struct Shader {
	char name[260];
	void* data;
};

struct ShaderCollection {
	int len;
	Shader entries[256];
	inline void AddEntry(char* name, void* data) {
		if (this->len < 256) {
			Shader* entry = &this->entries[this->len];
			strcpy(entry->name, name);
			entry->data = data;
			this->len = this->len + 1;
		}
	}
	inline void* GetByName(const char* param_1) const {
		for (std::size_t i = 0; i < len; i++) {
			if (_stricmp(entries[i].name, param_1) == 0) {
				return entries[i].data;
			}
		}
		return nullptr;
	}
};

class X360ShaderManager {
public:
	ShaderCollection compiled_vertex_shaders;
	ShaderCollection compiled_pixel_shaders;
	ShaderCollection shader_source_codes;
public:
	virtual int Unk1(int, int, int, int, int, int);
	virtual int Unk2(int, int);
	
	X360ShaderManager();
	~X360ShaderManager();
	void Create();
	void FreeSourceCode();

	IDirect3DVertexShader9* GetVertexShader(const char*) const;
	IDirect3DPixelShader9* GetPixelShader(const char*) const;

	IDirect3DVertexShader9* LoadVertexShader(const char*);
	IDirect3DPixelShader9* LoadPixelShader(const char*);
};

extern X360ShaderManager* lpX360ShaderManager;