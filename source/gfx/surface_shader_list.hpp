#pragma once
#include <cstddef>
#include <array>
#include "block_allocator.hpp"
#include "shader_pass_descriptor.hpp"

constexpr std::size_t MAX_SURFACE_SHADER_PASSES = 4096;

struct SurfaceShader {
	int next_index;
	BlockAllocator* block_allocator;
};

class SurfaceShaderList {
public:
	std::size_t surface_shader_pass_count;
	std::array<void*, MAX_SURFACE_SHADER_PASSES> surface_shader_passes;
	SurfaceShader shaders[4];
public:
	SurfaceShaderList();
	SurfaceShaderList(const SurfaceShaderList&) = delete;
	SurfaceShaderList& operator=(const SurfaceShaderList&) = delete;
	virtual ~SurfaceShaderList();
	int AddShaderPass(ShaderPassDescriptor* desc, int unused, uintptr_t unk);
	void ActivateMaterial(int material, int shader);
	void UNK_0040cd00(int shader, int unk);
};

class X360SurfaceShaderList : public SurfaceShaderList {
public:
	virtual ~X360SurfaceShaderList() override;
};

extern X360SurfaceShaderList* g_lpSurfaceShaderList;