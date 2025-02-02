#include "x360_index_buffer_manager.hpp"

// OFFSET: 0x004129d0, STATUS: COMPLETE
X360IndexBufferManager::X360IndexBufferManager() {
	unk_capacity = 0;
	cache_list = nullptr;
	unused = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
X360IndexBufferManager::~X360IndexBufferManager() {
	EmptyCacheList();
}

// OFFSET: 0x00413360, STATUS: TODO
int X360IndexBufferManager::CreateIndexBuffer(D3DFORMAT format, int index_count) {
	return 0;
}

// OFFSET: 0x00413070, STATUS: TODO
void X360IndexBufferManager::EmptyCacheList() {
}
