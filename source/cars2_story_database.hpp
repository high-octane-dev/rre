#pragma once
#include "serializable_interface.hpp"
#include "cars2_story_event.hpp"
#include "allocators/string_block_allocator.hpp"
#include "util/macros.hpp"

class Cars2StoryDatabase : public SerializableInterface {
public:
	Cars2StoryEvent* story_events;
	int story_events_len;
	int bb_unlock_1;
	int bb_unlock_2;
	int bb_unlock_3;
	float percent_completion_denominator;
	int starting_bolt_banners;
	StringBlockAllocator* string_block_allocator;
public:
	RRE_DISABLE_COPY(Cars2StoryDatabase);
	Cars2StoryDatabase();

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	virtual int GetSerializedDataSize() override;
	virtual ~Cars2StoryDatabase();
};