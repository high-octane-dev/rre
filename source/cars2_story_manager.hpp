#pragma once
#include "virtual_network.hpp"
#include "serializable_interface.hpp"
#include "cars2_story_event.hpp"
#include "cars2_bumper_sticker.hpp"
#include "cars2_story_database.hpp"
#include "util/rsstring_util.hpp"
#include "util/macros.hpp"

class Cars2StoryManager : public VNObject, SerializableInterface {
public:
	class VehicleInfo : public SerializableInterface {
	public:
		char vehicle_name[40];
		char character_paint_job[40];
		char character_wheel_set[40];
	public:
		// OFFSET: INLINE, STATUS: COMPLETE
		inline VehicleInfo() {
			vehicle_name[0] = 0;
			character_paint_job[0] = 0;
			character_wheel_set[0] = 0;
		}

		// OFFSET: 0x0042b000, STATUS: COMPLETE
		void SetData(const char* paint_job, const char* wheel_set) {
			if (paint_job != nullptr) {
				RSStringUtil::Ssnprintf(character_paint_job, sizeof(character_paint_job), "%s", paint_job);
			}
			if (wheel_set != nullptr) {
				RSStringUtil::Ssnprintf(character_wheel_set, sizeof(character_wheel_set), "%s", wheel_set);
			}
		}

		// OFFSET: 0x0042b070, STATUS: COMPLETE
		inline virtual int Serialize(void* buffer, int len) override {
			std::memcpy(buffer, &vehicle_name, sizeof(vehicle_name));
			std::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(vehicle_name)), &character_paint_job, sizeof(character_paint_job));
			std::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(vehicle_name) + sizeof(character_paint_job)), &character_wheel_set, sizeof(character_wheel_set));
			return sizeof(vehicle_name) + sizeof(character_paint_job) + sizeof(character_wheel_set);
		}

		// OFFSET: 0x0042b0b0, STATUS: COMPLETE
		inline virtual int DeSerialize(void* buffer, int len) override {
			std::memcpy(&vehicle_name, buffer, sizeof(vehicle_name));
			std::memcpy(&character_paint_job, reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(vehicle_name)), sizeof(character_paint_job));
			std::memcpy(&character_wheel_set, reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + sizeof(vehicle_name) + sizeof(character_paint_job)), sizeof(character_wheel_set));
			return sizeof(vehicle_name) + sizeof(character_paint_job) + sizeof(character_wheel_set);
		}

		// OFFSET: INLINE, STATUS: COMPLETE
		inline virtual int GetSerializedDataSize() override {
			return 0;
		}
	};
	class ExploreHub : public SerializableInterface {
	public:
		char explore_hub_name[40];
		char current_event[40];
		char ambient_vehicle_group[40];
	public:
		// OFFSET: INLINE, STATUS: COMPLETE
		inline ExploreHub() {
			explore_hub_name[0] = 0;
			current_event[0] = 0;
			ambient_vehicle_group[0] = 0;
		}

		// OFFSET: 0x0042afa0, STATUS: COMPLETE
		inline virtual int Serialize(void* buffer, int len) override {
			std::memcpy(buffer, &current_event, 40);
			std::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 40), &ambient_vehicle_group, 40);
			return 80;
		}

		// OFFSET: 0x0042afd0, STATUS: COMPLETE
		inline virtual int DeSerialize(void* buffer, int len) override {
			std::memcpy(&current_event, buffer, 40);
			std::memcpy(&ambient_vehicle_group, reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 40), 40);
			return 80;
		}

		// OFFSET: INLINE, STATUS: COMPLETE
		inline virtual int GetSerializedDataSize() override {
			return 0;
		}
	};
private:
	Cars2StoryDatabase* story_database;
	VehicleInfo vehicles[2];
	int current_vehicle;
	ExploreHub explore_hubs[4];
	int current_explore_hub;
	ContainerList<Cars2BumperSticker*> bumper_stickers;
	int time_of_day;
	char current_story_event_name[40];
	int enable_mcq_horn;
	int mcq_hq_stage;
	Cars2StoryEvent* current_story_event;
public:
	RRE_DISABLE_COPY(Cars2StoryManager);
	Cars2StoryManager();
	void Create();

	virtual ~Cars2StoryManager() override;
	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}

	virtual void ReceiveVNOData(VNObject*, void*) override;
	virtual void ReceiveVNOEvent(VNObject*, VNMessage*) override;

};

extern Cars2StoryManager* lpGlobalStoryManager;

static_assert(sizeof(Cars2StoryManager::VehicleInfo) == 124);
static_assert(sizeof(Cars2StoryManager::ExploreHub) == 124);
static_assert(sizeof(Cars2StoryManager) == 852);
