#include "cars2_event_set.hpp"
#include "string_block_allocator.hpp"
#include "cars2_event_database.hpp"

// OFFSET: 0x00428e70, STATUS: COMPLETE
Cars2EventSet::Cars2EventSet() {
	events = nullptr;
	events_len = 0;
	playable_vehicles = nullptr;
	playable_vehicles_len = 0;
	computer_vehicles = nullptr;
	computer_vehicles_len = 0;
	event_set_name = nullptr;
	activity_type = ActivityType::Invalid;
	name_string_id = nullptr;
	unk = 0;
}

// OFFSET: 0x00473980, STATUS: COMPLETE
Cars2EventSet::~Cars2EventSet() {
	if (events != nullptr) {
		delete[] events;
		events = nullptr;
	}

	if (playable_vehicles != nullptr) {
		for (int i = 0; i < playable_vehicles_len; i++) {
			lpGlobalEventDatabase->string_block_allocator->FreeString(playable_vehicles[i]);
		}
		delete[] playable_vehicles;
	}
	if (computer_vehicles != nullptr) {
		for (int i = 0; i < computer_vehicles_len; i++) {
			lpGlobalEventDatabase->string_block_allocator->FreeString(computer_vehicles[i]);
		}
		delete[] computer_vehicles;
	}

	if (event_set_name != nullptr) {
		lpGlobalEventDatabase->string_block_allocator->FreeString(event_set_name);
	}

	if (name_string_id != nullptr) {
		lpGlobalEventDatabase->string_block_allocator->FreeString(name_string_id);
	}
}

// OFFSET: 0x00429310, STATUS: COMPLETE
int Cars2EventSet::Serialize(void* buffer, int len) {
	int written = 0;
	for (int i = 0; i < events_len; i++) {
		written += events[i].Serialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + written), len - written);
	}
	return written;
}

// OFFSET: 0x00429360, STATUS: COMPLETE
int Cars2EventSet::DeSerialize(void* buffer, int len) {
	int read = 0;
	for (int i = 0; i < events_len; i++) {
		read += events[i].DeSerialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + read), len - read);
	}
	return read;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Cars2EventSet::GetSerializedDataSize() {
	return 0;
}
