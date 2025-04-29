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

// OFFSET: 0x00413070, STATUS: COMPLETE
void X360IndexBufferManager::EmptyCacheList() {
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
