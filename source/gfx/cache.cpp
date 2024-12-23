#include "cache.hpp"

// OFFSET: INLINE
Cache::Cache()
{
	offset = 0;
	len = 0;
	linked_list = nullptr;
}

// OFFSET: 0x005ea720
Cache::~Cache()
{
}

// OFFSET: 0x005d0d10
void Cache::FreeAllNodes()
{
}

// OFFSET: 0x00543870
ListEntry* Cache::CreateListEntry(int _offset, int _len)
{
	return new ListEntry(_offset, _len);
}

// OFFSET: 0x00577780
void Cache::FreeListEntry(ListEntry* entry)
{
	if (entry != nullptr) delete entry;
}

// OFFSET: 0x005777a0
ListEntry* Cache::ClearAndAddListEntry(int)
{
	return nullptr;
}
