#include "cache.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
Cache::Cache() {
	offset = 0;
	len = 0;
	linked_list = nullptr;
}

// OFFSET: 0x005ea720, STATUS: TODO
Cache::~Cache() {
}

// OFFSET: 0x005d0d10, STATUS: TODO
void Cache::FreeAllNodes() {
}

// OFFSET: 0x00543870, STATUS: COMPLETE
ListEntry* Cache::CreateListEntry(int _offset, int _len) {
	return new ListEntry(_offset, _len);
}

// OFFSET: 0x00577780, STATUS: COMPLETE
void Cache::FreeListEntry(ListEntry* entry) {
	if (entry != nullptr) delete entry;
}

// OFFSET: 0x005777a0, STATUS: TODO
ListEntry* Cache::ClearAndAddListEntry(int) {
	return nullptr;
}
