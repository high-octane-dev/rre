#pragma once
#include "cars2_event_set.hpp"
#include "string_block_allocator.hpp"

class Cars2EventDatabase : public SerializableInterface {
public:
	Cars2EventSet* event_sets;
	int event_sets_len;
	StringBlockAllocator* string_block_allocator;
public:
	Cars2EventDatabase();
	Cars2EventDatabase(const Cars2EventDatabase&) = delete;
	Cars2EventDatabase& operator=(const Cars2EventDatabase&) = delete;
	~Cars2EventDatabase();
	void Create();
	Cars2EventInfo* GetEventInfo(Cars2ActivityInfo* activity);
	Cars2EventInfo* GetEventInfo(char* name);
	Cars2EventSet* GetEventSet(Cars2EventInfo* info);
	Cars2EventSet* GetEventSet(char* name);
	void Reset();

	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int Serialize(void* buffer, int len) override {
		int written = 0;
		for (int i = 0; i < event_sets_len; i++) {
			written += event_sets[i].Serialize(reinterpret_cast<std::uint8_t*>(buffer) + written, len - written);
		}
		return written;
	}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int DeSerialize(void* buffer, int len) override {
		int read = 0;
		for (int i = 0; i < event_sets_len; i++) {
			read += event_sets[i].DeSerialize(reinterpret_cast<std::uint8_t*>(buffer) + read, len - read);
		}
		return read;
	}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
};

extern Cars2EventDatabase* lpGlobalEventDatabase;