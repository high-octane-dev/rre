#pragma once
#include "containers/container_list.hpp"

class StringBlockAllocator {
public:
	struct StringBlock {
		char* start;
		int ref_count;
		char* end;
		// always set to zero so doesn't matter
		int unk;
		StringBlock();
		StringBlock(const StringBlock&) = delete;
		StringBlock& operator=(const StringBlock&) = delete;
		~StringBlock();
	};
private:
	std::size_t initial_string_len;
	int alloc_size;
	ContainerList<StringBlock*> string_blocks;
public:
	StringBlockAllocator(std::size_t _initial_string_len, int _alloc_size);
	StringBlockAllocator(const StringBlockAllocator&) = delete;
	StringBlockAllocator& operator=(const StringBlockAllocator&) = delete;
	~StringBlockAllocator();

	void Create(int unk);
	char* StringBlockAllocator_AllocStringByLength(int length, int unk);
	char* StringBlockAllocator_AllocStringByString(char* str, int unk);
	void FreeString(char* str);
};

#ifdef _M_IX86
static_assert(sizeof(StringBlockAllocator::StringBlock) == 16);
static_assert(sizeof(StringBlockAllocator) == 32);
#endif