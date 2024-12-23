#include "cache.hpp"

Cache::Cache()
{
	offset = 0;
	len = 0;
	linked_list = nullptr;
}

Cache::~Cache()
{
}

ListEntry* Cache::CreateListEntry(int, int)
{
	return nullptr;
}

void Cache::FreeListEntry(void*)
{
}

ListEntry* Cache::ClearAndAddListEntry(int)
{
	return nullptr;
}
