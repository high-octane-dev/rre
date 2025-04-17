#include "surface_shader_list.hpp"

X360SurfaceShaderList* g_lpSurfaceShaderList = nullptr;

// OFFSET: 0x005658c0, STATUS: COMPLETE
SurfaceShaderList::SurfaceShaderList() {
	std::fill(surface_shader_passes.begin(), surface_shader_passes.end(), nullptr);
	surface_shader_pass_count = 0;
	shaders[0].block_allocator = new BlockAllocator(28, 4096);
	shaders[1].block_allocator = new BlockAllocator(28, 4096);
	shaders[2].block_allocator = new BlockAllocator(28, 4096);
	shaders[3].block_allocator = new BlockAllocator(28, 4096);
	shaders[0].next_index = 1;
	shaders[1].next_index = 1;
	shaders[2].next_index = 1;
	shaders[3].next_index = 1;
	if (g_lpSurfaceShaderList == nullptr) {
		g_lpSurfaceShaderList = static_cast<X360SurfaceShaderList*>(this);
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
SurfaceShaderList::~SurfaceShaderList() {
	if (shaders[0].block_allocator != nullptr) {
		shaders[0].block_allocator->Release();
	}
	if (shaders[1].block_allocator != nullptr) {
		shaders[1].block_allocator->Release();
	}
	if (shaders[2].block_allocator != nullptr) {
		shaders[2].block_allocator->Release();
	}
	if (shaders[3].block_allocator != nullptr) {
		shaders[3].block_allocator->Release();
	}
	if (g_lpSurfaceShaderList == this) {
		g_lpSurfaceShaderList = nullptr;
	}
	for (std::size_t i = 0; i < surface_shader_pass_count; i++) {
		delete surface_shader_passes[i];
	}
}

// OFFSET: 0x00565af0, STATUS: TODO
int SurfaceShaderList::AddShaderPass(ShaderPassDescriptor* desc, int unused, uintptr_t unk) {
	return 0;
}

// OFFSET: 0x00565d40, STATUS: TODO
void SurfaceShaderList::ActivateMaterial(int material, int shader) {
}

// OFFSET: 0x0040cd00, STATUS: TODO
void SurfaceShaderList::UNK_0040cd00(int shader, int unk) {
}

// OFFSET: 0x00565970, STATUS: COMPLETE
X360SurfaceShaderList::~X360SurfaceShaderList() {
}
