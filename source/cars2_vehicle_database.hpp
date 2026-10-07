#pragma once
#include "serializable_interface.hpp"
#include "parameter_block.hpp"
#include "containers/container_list.hpp"
#include "allocators/string_block_allocator.hpp"
#include "util/macros.hpp"

class ActionScript;

class Cars2VehiclePaintJob : public SerializableInterface {
public:
	char* name;
	char** materials;
	int materials_len;
	int default_locked;
	int locked;
	struct BonusContentCharacterItem* bonus_content_item;
public:
	RRE_DISABLE_COPY(Cars2VehiclePaintJob);
	Cars2VehiclePaintJob();
	void Create(ParameterBlock* file);

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
	virtual ~Cars2VehiclePaintJob();
};

class Cars2VehicleWheelSet : public SerializableInterface {
public:
	char* name;
	char** materials;
	int materials_len;
	int default_locked;
	int locked;
	int unk[4];
	struct BonusContentCharacterItem* bonus_content_item;
public:
	RRE_DISABLE_COPY(Cars2VehicleWheelSet);
	Cars2VehicleWheelSet();
	void Create(ParameterBlock* file);

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
	virtual ~Cars2VehicleWheelSet();
};

class Cars2VehicleRecord : public SerializableInterface {
public:
	char* name;
	char* directory;
	char* default_physics_file;
	int animation_enabled;
	char** paint_job_materials;
	int paint_job_materials_len;
	Cars2VehiclePaintJob* paint_jobs;
	int paint_jobs_len;
	char** wheel_set_materials;
	int wheel_set_materials_len;
	Cars2VehicleWheelSet* wheel_sets;
	int wheel_sets_len;
	int boost;
	int acceleration;
	int handling;
	int stability;
	int playable;
	char* name_string_id;
	int default_locked;
	int locked;
	struct BonusContentCharacterItem* bonus_content_item;
public:
	RRE_DISABLE_COPY(Cars2VehicleRecord);
	Cars2VehicleRecord();
	void Create(ParameterBlock* file);
	Cars2VehiclePaintJob* GetPaintJob(const char* name);
	void Reset();

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
	virtual ~Cars2VehicleRecord();
};

// An action script that gets triggered whenever a vehicle, paint job, or wheel set gets unlocked.
struct Cars2UnlockAction {
	char* name;
	ActionScript* action_script;
};

class Cars2VehicleDatabase : public SerializableInterface {
public:
	ContainerList<Cars2VehiclePaintJob*> paint_jobs;
	Cars2UnlockAction* paint_job_unlock_action;
	ContainerList<Cars2VehicleWheelSet*> wheel_sets;
	Cars2UnlockAction* wheel_set_unlock_action;
	Cars2VehicleRecord* vehicles;
	int vehicles_len;
	Cars2UnlockAction* vehicle_unlock_action;
	StringBlockAllocator* string_block_allocator;
public:
	RRE_DISABLE_COPY(Cars2VehicleDatabase);
	Cars2VehicleDatabase();
	void Create();
	Cars2VehicleRecord* GetVehicle(const char* name);
	Cars2VehiclePaintJob* GetPaintJob(const char* name);
	Cars2VehicleWheelSet* GetWheelSet(const char* name);
	void UnlockAllPlayableVehicles();

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
	virtual ~Cars2VehicleDatabase();

	// Allocates a copy of the given string using the vehicle database's string block allocator.
	// OFFSET: INLINE, STATUS: COMPLETE
	inline char* AllocString(char* str) {
		char* allocated = string_block_allocator->StringBlockAllocator_AllocStringByString(str, 0);
		strncpy(allocated, str, strlen(str) + 1);
		return allocated;
	}
};

extern Cars2VehicleDatabase* lpGlobalVehicleDatabase;

#ifdef _M_IX86
static_assert(sizeof(Cars2VehiclePaintJob) == 0x1c);
static_assert(sizeof(Cars2VehicleWheelSet) == 0x2c);
static_assert(sizeof(Cars2VehicleRecord) == 0x58);
static_assert(sizeof(Cars2VehicleDatabase) == 0x4c);
#endif
