#pragma once
#include "cars2_activity_set.hpp"
#include "cars2_activity_info.hpp"
#include "string_block_allocator.hpp"

class Cars2ActivityDatabase {
public:
	Cars2ActivitySet* activity_sets;
	int activity_sets_len;
	StringBlockAllocator* string_block_allocator;
public:
	Cars2ActivityDatabase();
	~Cars2ActivityDatabase();
	void Create();
	Cars2ActivityInfo* GetActivityInfo(char*);
};

extern Cars2ActivityDatabase* lpGlobalActivityDatabase;