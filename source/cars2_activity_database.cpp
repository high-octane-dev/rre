#include "cars2_activity_database.hpp"

Cars2ActivityDatabase* lpGlobalActivityDatabase = nullptr;

// OFFSET: INLINE, STATUS: COMPLETE
Cars2ActivityDatabase::Cars2ActivityDatabase() {
    activity_sets = nullptr;
    activity_sets_len = 0;
    string_block_allocator = nullptr;
    lpGlobalActivityDatabase = this;
}

// OFFSET: 0x004b0830, STATUS: COMPLETE
Cars2ActivityDatabase::~Cars2ActivityDatabase() {
    if (activity_sets != nullptr) {
        delete[] activity_sets;
        activity_sets = nullptr;
    }
    if (string_block_allocator != nullptr) {
        delete string_block_allocator;
        string_block_allocator = nullptr;
    }
    lpGlobalActivityDatabase = nullptr;
}

// OFFSET: 0x004731d0, STATUS: TODO
void Cars2ActivityDatabase::Create() {
}

// OFFSET: 0x004273e0, STATUS: COMPLETE
Cars2ActivityInfo* Cars2ActivityDatabase::GetActivityInfo(char* name) {
    for (int i = 0; i < activity_sets_len; i++) {
        Cars2ActivityInfo* activity = activity_sets[i].GetActivityInfo(name);
        if (activity != nullptr) {
            return activity;
        }
    }
	return nullptr;
}