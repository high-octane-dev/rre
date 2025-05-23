#pragma once
#include "cars2_activity_set.hpp"
#include "cars2_activity_info.hpp"
#include "allocators/string_block_allocator.hpp"

class Cars2ActivityDatabase {
public:
	Cars2ActivitySet* activity_sets;
	int activity_sets_len;
	StringBlockAllocator* string_block_allocator;
public:
	RRE_DISABLE_COPY(Cars2ActivityDatabase);
	Cars2ActivityDatabase();
	~Cars2ActivityDatabase();
	void Create();
	Cars2ActivityInfo* GetActivityInfo(char*);
};

extern Cars2ActivityDatabase* lpGlobalActivityDatabase;