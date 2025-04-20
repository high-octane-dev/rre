#include <memory>
#include "cars2_launcher.hpp"
#include "util/rsstring_util.hpp"

// WTFRB: As far as I'm aware, they leak this in the original game. So we'll use std::unique_ptr here.
static std::unique_ptr<Cars2Launcher> lpGlobalLauncher = nullptr;

// OFFSET: 0x004745d0, STATUS: COMPLETE
Cars2Launcher::Cars2Launcher() : vehicle_infos(), vehicle_infos2() {
	incoming_selected_event_type = SelectedEventType::Race;
	activity_type = ActivityType::RaceRoad;
	activity_name[0] = '\0';
	player_count = 1;
	needs_character_select_screen = 0;
	current_event_in_explore_hub = nullptr;
	incoming_unk_type = UnkType::Race;
}

// OFFSET: 0x00429b10, STATUS: COMPLETE
void Cars2Launcher::SetVehicleInfo(int index, char* name, char* _paint_job, char* _wheel_set) {
	char paint_job[40]{};
	char wheel_set[40]{};
	if (_paint_job != nullptr) {
		RSStringUtil::Ssnprintf(paint_job, sizeof(paint_job), "%s", _paint_job);
	}
	if (_wheel_set != nullptr) {
		RSStringUtil::Ssnprintf(wheel_set, sizeof(wheel_set), "%s", _wheel_set);
	}
	RSStringUtil::Ssnprintf(vehicle_infos[index].vehicle_name, sizeof(VehicleInformation::vehicle_name), "%s", name);
	RSStringUtil::Ssnprintf(vehicle_infos[index].character_paint_job, sizeof(VehicleInformation::character_paint_job), "%s", paint_job);
	RSStringUtil::Ssnprintf(vehicle_infos[index].character_wheel_set, sizeof(VehicleInformation::character_wheel_set), "%s", wheel_set);
}

// OFFSET: 0x004e91f0, STATUS: TODO
void Cars2Launcher::LaunchActivity() {
}

// OFFSET: 0x004b15c0, STATUS: TODO
void Cars2Launcher::LaunchFrontEndUI() {
}

// OFFSET: 0x004b1690, STATUS: TODO
void Cars2Launcher::LaunchRace() {
}

// OFFSET: 0x004b1900, STATUS: TODO
void Cars2Launcher::LaunchStoryMode() {
}

// OFFSET: 0x004b17a0, STATUS: TODO
void Cars2Launcher::LaunchVersusRace() {
}

// OFFSET: 0x00429af0, STATUS: COMPLETE
void Cars2Launcher::SetActivity(char* activity) {
	RSStringUtil::Ssnprintf(activity_name, sizeof(activity_name), "%s", activity);
}

// OFFSET: 0x004b1550, STATUS: COMPLETE
Cars2Launcher* Cars2Launcher::GetInstance() {
	if (lpGlobalLauncher == nullptr) {
		lpGlobalLauncher = std::make_unique<Cars2Launcher>();
	}
	return lpGlobalLauncher.get();
}
