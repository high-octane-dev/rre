#include "cars2_event_join_point_manager.hpp"
#include "cars2_event_database.hpp"
#include "cars_game.hpp"
#include "util/rsstring_util.hpp"

Cars2EventJoinPointManager* lpGlobalEventJoinPointManager = nullptr;

static constexpr Vector4 g_DefaultPosition = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
static constexpr Vector4 g_DefaultLookVector = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
static constexpr Vector4 g_DefaultUpVector = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

// OFFSET: 0x00429560, STATUS: COMPLETE
Cars2EventJoinPoint::MenuItem::MenuItem() {
	name[0] = 0;
	cutscene = nullptr;
	event = nullptr;
	unk_name[0] = 0;
	unk_index = -1;
	parent = nullptr;
}

// OFFSET: 0x00429580, STATUS: COMPLETE
Cars2EventJoinPoint::MenuItem::~MenuItem() {
	if (cutscene != nullptr) {
		delete cutscene;
		cutscene = nullptr;
	}
	if (event != nullptr) {
		delete event;
		event = nullptr;
	}
}

// OFFSET: 0x004295c0, STATUS: COMPLETE
void Cars2EventJoinPoint::MenuItem::Create(ParameterBlock* file) {
	char event_name[40]{};
	file->ReadParameterBlock(name);
	if (file->GetParameter("Cutscene", "", event_name, sizeof(event_name)) != 0) {
		cutscene = new EventInfoStartPair();
		cutscene->event = nullptr;
		cutscene->event_start[0] = 0;
		cutscene->event = lpGlobalEventDatabase->GetEventInfo(event_name);
		file->GetParameter("CutsceneStart", "Player1Start", cutscene->event_start, sizeof(cutscene->event_start));
	}
	if (file->GetParameter("Event", "", event_name, sizeof(event_name)) != 0) {
		event = new EventInfoStartPair();
		event->event = nullptr;
		event->event_start[0] = 0;
		event->event = lpGlobalEventDatabase->GetEventInfo(event_name);
		file->GetParameter("EventStart", "", event->event_start, sizeof(event->event_start));
	}
}

// OFFSET: 0x00429770, STATUS: COMPLETE
Cars2EventJoinPoint::Cars2EventJoinPoint() {
	music_radius = 0.0f;
	name[0] = 0;
	type = EventJoinPointType::None;
	music_track_name[0] = 0;
	explore_hub_event = nullptr;
	position = g_DefaultPosition;
	look_vector = g_DefaultLookVector;
	up_vector = g_DefaultUpVector;
	zone = 0;
	menu_items = nullptr;
	menu_items_len = 0;
}

// OFFSET: 0x00473f70, STATUS: COMPLETE
Cars2EventJoinPoint::~Cars2EventJoinPoint() {
	if (menu_items != nullptr) {
		delete[] menu_items;
		menu_items = nullptr;
	}
}

// OFFSET: 0x00429810, STATUS: COMPLETE
void Cars2EventJoinPoint::Create(ParameterBlock* file) {
	char type_name[20]{};
	char explore_hub_name[40]{};

	file->ReadParameterBlock(name);
	file->GetParameter("Type", "None", type_name, sizeof(type_name));
	if (_stricmp("Race", type_name) == 0) {
		type = EventJoinPointType::Race;
	}
	else if (_stricmp("Minigame", type_name) == 0) {
		type = EventJoinPointType::Minigame;
	}
	else if (_stricmp("RaceAndMinigame", type_name) == 0) {
		type = EventJoinPointType::RaceAndMinigame;
	}
	else if (_stricmp("RamoneShop", type_name) == 0) {
		type = EventJoinPointType::RamoneShop;
	}
	else if (_stricmp("LuigiGarage", type_name) == 0) {
		type = EventJoinPointType::LuigiGarage;
	}

	file->GetParameter("ExploreHubName", "", explore_hub_name, sizeof(explore_hub_name));
	explore_hub_event = lpGlobalEventDatabase->GetEventInfo(explore_hub_name);
	if (explore_hub_event != nullptr && explore_hub_event->activity_info->type != ActivityType::EXplorer) {
		explore_hub_event = nullptr;
	}

	file->GetParameter("MusicTrackName", "", music_track_name, sizeof(music_track_name));
	file->GetParameter("MusicRadius", 0.0f, &music_radius);

	menu_items_len = file->GetNumberOfParameterValues("MenuItem");
	if (menu_items_len <= 0) {
		return;
	}
	menu_items = new MenuItem[menu_items_len];

	file->search.SetResetParameterSearch(0);
	for (int i = 0; i < menu_items_len; i++) {
		char menu_item_name[40]{};
		file->GetParameter("MenuItem", "", menu_item_name, sizeof(menu_item_name));
		RSStringUtil::Ssnprintf(menu_items[i].name, sizeof(menu_items[i].name), "%s", menu_item_name);
	}
	file->search.SetResetParameterSearch(1);

	for (int i = 0; i < menu_items_len; i++) {
		menu_items[i].parent = this;
		menu_items[i].Create(file);
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
Cars2EventJoinPointManager::Cars2EventJoinPointManager() {
	event_join_points = nullptr;
	event_join_points_len = 0;
	lpGlobalEventJoinPointManager = this;
}

// OFFSET: INLINE, STATUS: COMPLETE
Cars2EventJoinPointManager::~Cars2EventJoinPointManager() {
	if (event_join_points != nullptr) {
		delete[] event_join_points;
		event_join_points = nullptr;
	}
	lpGlobalEventJoinPointManager = nullptr;
}

// OFFSET: 0x004b10d0, STATUS: COMPLETE
void Cars2EventJoinPointManager::Create() {
	ParameterBlock file{};
	char path[260]{};
	RSStringUtil::Ssnprintf(path, sizeof(path), "%sEventJoinPointInfo.dat", g_ActivityContentDirectory);
	if (file.OpenFile(path, 0, -1, nullptr, -1) != 0) {
		file.ReadParameterBlock("General");
		event_join_points_len = file.GetNumberOfParameterValues("EventJoinPoint");
		if (event_join_points_len > 0) {
			event_join_points = new Cars2EventJoinPoint[event_join_points_len];

			file.search.SetResetParameterSearch(0);
			for (int i = 0; i < event_join_points_len; i++) {
				char event_join_point_name[40]{};
				file.GetParameter("EventJoinPoint", "", event_join_point_name, sizeof(event_join_point_name));
				RSStringUtil::Ssnprintf(event_join_points[i].name, sizeof(event_join_points[i].name), "%s", event_join_point_name);
			}
			file.search.SetResetParameterSearch(1);

			for (int i = 0; i < event_join_points_len; i++) {
				event_join_points[i].Create(&file);
			}
		}
	}

	RSStringUtil::Ssnprintf(path, sizeof(path), "%sRS_Hub\\RS_Hub_EventJoinPoints.toy", g_ActivityContentDirectory);
	ProcessEventJoinPointToyFile(path);
	RSStringUtil::Ssnprintf(path, sizeof(path), "%sOV_Hub\\OV_Hub_EventJoinPoints.toy", g_ActivityContentDirectory);
	ProcessEventJoinPointToyFile(path);
	RSStringUtil::Ssnprintf(path, sizeof(path), "%sTF_Hub\\TF_Hub_EventJoinPoints.toy", g_ActivityContentDirectory);
	ProcessEventJoinPointToyFile(path);
}

// OFFSET: 0x00429aa0, STATUS: COMPLETE
Cars2EventJoinPoint* Cars2EventJoinPointManager::GetEventJoinPoint(const char* name) {
	for (int i = 0; i < event_join_points_len; i++) {
		if (_stricmp(event_join_points[i].name, name) == 0) {
			return &event_join_points[i];
		}
	}
	return nullptr;
}

// OFFSET: 0x00474330, STATUS: COMPLETE
void Cars2EventJoinPointManager::ProcessEventJoinPointToyFile(const char* path) {
	ParameterBlock file{};
	if (file.OpenFile(path, 0, -1, nullptr, -1) == 0 || file.ReadParameterBlock("ActJoinPoints") == 0) {
		return;
	}

	int act_join_points_len = 0;
	file.GetParameter("NumberOfActJoinPoints", 0, &act_join_points_len);
	for (int i = 1; i <= act_join_points_len; i++) {
		char block_name[40]{};
		RSStringUtil::Ssnprintf(block_name, sizeof(block_name), "ActJoinPoint%d", i);
		if (file.ReadParameterBlock(block_name) == 0) {
			continue;
		}

		char event_join_point_name[40]{};
		file.GetParameter("EventJoinPointName", "", event_join_point_name, sizeof(event_join_point_name));
		Cars2EventJoinPoint* event_join_point = GetEventJoinPoint(event_join_point_name);
		if (event_join_point == nullptr) {
			continue;
		}

		Vector4 position{};
		Vector4 look_vector{};
		Vector4 up_vector{};
		int zone = 0;
		file.GetParameter("Position", &g_DefaultPosition, &position);
		file.GetParameter("LookVector", &g_DefaultLookVector, &look_vector);
		file.GetParameter("UpVector", &g_DefaultUpVector, &up_vector);
		file.GetParameter("Zone", 1, &zone);
		event_join_point->position = position;
		event_join_point->look_vector = look_vector;
		event_join_point->up_vector = up_vector;
		// Zones are 1-indexed in the .toy files.
		event_join_point->zone = zone - 1;
	}
}
