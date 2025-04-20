#include "cars2_activity_info.hpp"
#include "cars2_activity_database.hpp"

// OFFSET: 0x00427270, STATUS: COMPLETE
Cars2ActivityInfo::Cars2ActivityInfo() {
	type = ActivityType::Invalid;
	directory = nullptr;
	activity_name = nullptr;
	scene = nullptr;
	time_of_day = 0;
	can_run_during_day_or_night = 0;
	unk = 0;
}

// OFFSET: 0x00427290, STATUS: COMPLETE
Cars2ActivityInfo::~Cars2ActivityInfo() {
	if (directory != nullptr) {
		lpGlobalActivityDatabase->string_block_allocator->FreeString(directory);
	}
	if (activity_name != nullptr) {
		lpGlobalActivityDatabase->string_block_allocator->FreeString(activity_name);
	}
	if (scene != nullptr) {
		lpGlobalActivityDatabase->string_block_allocator->FreeString(scene);
	}
}

// OFFSET: 0x00472e80, STATUS: TODO
void Cars2ActivityInfo::Create(ParameterBlock* file) {
	/*
	char read_parameter[64]{};
	file->ReadParameterBlock(directory);

	file->GetParameter("Directory", directory, read_parameter, sizeof(read_parameter));
	activity = lpGlobalActivityDatabase->string_block_alloc->AllocStringByString(read_parameter, 0);
	strncpy(activity, read_parameter, strlen(read_parameter) + 1);

	file->GetParameter("Scene", scene, read_parameter, sizeof(read_parameter));
	scene = lpGlobalActivityDatabase->string_block_alloc->AllocStringByString(read_parameter, 0);
	strncpy(scene, read_parameter, strlen(read_parameter) + 1);

	lpGlobalSceneDatabase->GetSceneInfo(scene);
	*/
}
