#include "cars2_story_database.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
Cars2StoryDatabase::Cars2StoryDatabase() {
	bb_unlock_3 = 0;
	bb_unlock_2 = 0;
	bb_unlock_1 = 0;
	percent_completion_denominator = 0;
	starting_bolt_banners = 0;
	story_events = nullptr;
	story_events_len = 0;
	string_block_allocator = nullptr;
}

// OFFSET: 0x0042abc0, STATUS: COMPLETE
int Cars2StoryDatabase::Serialize(void* buffer, int len) {
	int written = 0;
	for (int i = 0; i < story_events_len; i++) {
		written += story_events[i].Serialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + written), len - written);
	}
	return written;
}

// OFFSET: 0x0042ac10, STATUS: COMPLETE
int Cars2StoryDatabase::DeSerialize(void* buffer, int len) {
	int read = 0;
	for (int i = 0; i < story_events_len; i++) {
		read += story_events[i].DeSerialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + read), len - read);
	}
	return read;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Cars2StoryDatabase::GetSerializedDataSize() {
	return 0;
}

// OFFSET: 0x004e9530, STATUS: COMPLETE
Cars2StoryDatabase::~Cars2StoryDatabase() {
	if (story_events != nullptr) {
		delete[] story_events;
		story_events = nullptr;
	}
	if (string_block_allocator != nullptr) {
		delete string_block_allocator;
		string_block_allocator = nullptr;
	}
}
