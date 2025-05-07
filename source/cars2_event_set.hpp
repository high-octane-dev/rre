#pragma once
#include "serializable_interface.hpp"
#include "cars2_event_info.hpp"
#include "util/macros.hpp"

class Cars2EventSet : public SerializableInterface {
public:
	Cars2EventInfo* events;
	int events_len;
	char* event_set_name;
	ActivityType activity_type;
	char* name_string_id;
	int unk;
	char** playable_vehicles;
	int playable_vehicles_len;
	char** computer_vehicles;
	int computer_vehicles_len;
public:
	RRE_DISABLE_COPY(Cars2EventSet);
	Cars2EventSet();
	~Cars2EventSet();

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	virtual int GetSerializedDataSize() override;
};

#ifdef _M_IX86
static_assert(sizeof(Cars2EventSet) == 0x2c);
#endif