#include "cars2_activity_set.hpp"
#include <cars2_activity_database.hpp>

// OFFSET: 0x004272e0, STATUS: COMPLETE
Cars2ActivitySet::Cars2ActivitySet() {
	activities = nullptr;
	activities_len = 0;
	activity_type = ActivityType::Invalid;
	activity_type_str = nullptr;
}

// OFFSET: 0x00472ff0, STATUS: COMPLETE
Cars2ActivitySet::~Cars2ActivitySet() {
	if (activities != nullptr) {
		delete[] activities;
		activities = nullptr;
	}
	if (activity_type_str != nullptr) {
		lpGlobalActivityDatabase->string_block_allocator->FreeString(this->activity_type_str);
		activity_type_str = nullptr;
	}
}

// OFFSET: 0x00427380, STATUS: COMPLETE
Cars2ActivityInfo* Cars2ActivitySet::GetActivityInfo(char* name) {
	for (int i = 0; i < activities_len; i++) {
		if (_stricmp(activities[i].directory, name) == 0) {
			return &activities[i];
		}
	}
	return nullptr;
}
