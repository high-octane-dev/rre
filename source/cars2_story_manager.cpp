#include "cars2_story_manager.hpp"
#include "util/rsstring_util.hpp"

Cars2StoryManager* lpGlobalStoryManager = nullptr;

// OFFSET: 0x00475a40, STATUS: COMPLETE
Cars2StoryManager::Cars2StoryManager() : VNObject(0), vehicles(), explore_hubs(), bumper_stickers(), story_database(nullptr) {
	RSStringUtil::Ssnprintf(vehicles[0].vehicle_name, sizeof(VehicleInfo::vehicle_name), "mcq");
	vehicles[0].SetData("mcq_a", "mcq_wheelset_a");
	RSStringUtil::Ssnprintf(vehicles[1].vehicle_name, sizeof(VehicleInfo::vehicle_name), "mcqm");
	RSStringUtil::Ssnprintf(vehicles[1].character_paint_job, sizeof(VehicleInfo::character_paint_job), "%s", "mcqm_a");
	RSStringUtil::Ssnprintf(vehicles[1].character_wheel_set, sizeof(VehicleInfo::character_wheel_set), "");
	current_vehicle = 0;
	bumper_stickers.CLNonMacroCreate(7, 4, std::numeric_limits<std::int32_t>::max());
	time_of_day = 0;
	current_story_event = nullptr;
	current_story_event_name[0] = 0;
	enable_mcq_horn = 0;
	mcq_hq_stage = 1;
	lpGlobalStoryManager = this;
}

// OFFSET: 0x004e96c0, STATUS: TODO
void Cars2StoryManager::Create() {
}

// OFFSET: 0x00475be0, STATUS: COMPLETE
Cars2StoryManager::~Cars2StoryManager() {
	if (story_database != nullptr) {
		delete story_database;
		story_database = nullptr;
	}
	for (auto& bumper_sticker : bumper_stickers) {
		delete bumper_sticker;
	}
	bumper_stickers.Clear();
	if (lpGlobalStoryManager == this) {
		lpGlobalStoryManager = nullptr;
	}
}

// OFFSET: 0x00475df0, STATUS: TODO
int Cars2StoryManager::Serialize(void* buffer, int len) {
	return 0;
}

// OFFSET: 0x00475f00, STATUS: TODO
int Cars2StoryManager::DeSerialize(void* buffer, int len) {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void Cars2StoryManager::ReceiveVNOData(VNObject*, void*) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void Cars2StoryManager::ReceiveVNOEvent(VNObject*, VNMessage*) {
}
