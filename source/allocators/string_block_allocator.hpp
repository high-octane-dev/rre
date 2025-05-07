#pragma once
#include "containers/container_list.hpp"
#include "util/macros.hpp"

class StringBlockAllocator {
public:
	struct StringBlock {
		char* start;
		int ref_count;
		char* end;
		// always set to zero so doesn't matter
		int unk;
		RRE_DISABLE_COPY(StringBlock);
		StringBlock();
		~StringBlock();
	};
private:
	std::size_t initial_string_len;
	int alloc_size;
	ContainerList<StringBlock*> string_blocks;
public:
	RRE_DISABLE_COPY(StringBlockAllocator);
	StringBlockAllocator(std::size_t _initial_string_len, int _alloc_size);
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