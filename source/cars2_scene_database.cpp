#include "cars2_scene_database.hpp"
#include "cars_game.hpp"
#include "parameter_block.hpp"
#include "util/rsstring_util.hpp"

Cars2SceneDatabase* lpGlobalSceneDatabase = nullptr;

// OFFSET: INLINE, STATUS: COMPLETE
Cars2SceneDatabase::Cars2SceneDatabase() {
	scene_infos = nullptr;
	scene_infos_len = 0;
	lpGlobalSceneDatabase = this;
}

// OFFSET: INLINE, STATUS: COMPLETE
Cars2SceneDatabase::~Cars2SceneDatabase() {
	if (scene_infos != nullptr) {
		delete[] scene_infos;
		scene_infos = nullptr;
	}
	lpGlobalSceneDatabase = nullptr;
}

// OFFSET: 0x0042a4c0, STATUS: COMPLETE
void Cars2SceneDatabase::Create() {
	ParameterBlock file{};
	char path[260]{};
	RSStringUtil::Ssnprintf(path, sizeof(path), "%sSceneInfo.dat", g_SceneContentDirectory);
	if (file.OpenFile(path, 0, -1, nullptr, -1) == 0) {
		return;
	}

	file.ReadParameterBlock("SceneInfo");
	scene_infos_len = file.GetNumberOfParameterValues("Scene");
	scene_infos = new SceneInfo[scene_infos_len];

	// The scene names are read first, and then each scene's block is read afterwards.
	file.search.SetResetParameterSearch(0);
	for (int i = 0; i < scene_infos_len; i++) {
		char scene_name[16]{};
		file.GetParameter("Scene", "", scene_name, sizeof(scene_name));
		RSStringUtil::Ssnprintf(scene_infos[i].name, sizeof(scene_infos[i].name), "%s", scene_name);
	}
	file.search.SetResetParameterSearch(1);

	for (int i = 0; i < scene_infos_len; i++) {
		SceneInfo* info = &scene_infos[i];
		file.ReadParameterBlock(info->name);
		file.GetParameter("Directory", info->name, info->directory, sizeof(info->directory));
		file.GetParameter("PageFile", "", info->page_file, sizeof(info->page_file));
		file.GetParameter("LoadingScreenName", "load_logo", info->loading_screen_name, sizeof(info->loading_screen_name));
		file.GetParameter("HasVersusModeStreampackFile", 0, &info->has_versus_mode_streampack_file);
	}
}

// OFFSET: 0x0042a6f0, STATUS: COMPLETE
Cars2SceneDatabase::SceneInfo* Cars2SceneDatabase::GetSceneInfo(const char* name) {
	for (int i = 0; i < scene_infos_len; i++) {
		if (_stricmp(name, scene_infos[i].name) == 0) {
			return &scene_infos[i];
		}
	}
	return nullptr;
}
