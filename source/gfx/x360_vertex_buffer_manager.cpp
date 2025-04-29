#include "x360_vertex_buffer_manager.hpp"

// OFFSET: 0x004129a0, STATUS: COMPLETE
X360VertexBufferManager::X360VertexBufferManager() {
	unk_capacity = 0;
	cache_list = nullptr;
	unused = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
X360VertexBufferManager::~X360VertexBufferManager() {
	EmptyCacheList();
}

// OFFSET: 0x00413200, STATUS: TODO
int X360VertexBufferManager::CreateVertexBuffer(D3DFORMAT format, int vertex_count) {
	return 0;
}

// OFFSET: 0x00413000, STATUS: COMPLETE
void X360VertexBufferManager::EmptyCacheList() {
	unk_capacity = 0;
	if (cache_list != nullptr) {
		for (auto& elem : *cache_list) {
			delete elem;
		}
		delete cache_list;
		cache_list = nullptr;
	}
	unused = 0;
}
