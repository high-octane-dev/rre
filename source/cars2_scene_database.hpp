#pragma once
#include "util/macros.hpp"

class Cars2SceneDatabase {
public:
	class SceneInfo {
	public:
		char name[16];
		char directory[16];
		char page_file[24];
		char loading_screen_name[24];
		int has_versus_mode_streampack_file;
	public:
		RRE_DISABLE_COPY(SceneInfo);
		// OFFSET: 0x0042a4a0, STATUS: COMPLETE
		inline SceneInfo() {
			name[0] = 0;
			directory[0] = 0;
			page_file[0] = 0;
			loading_screen_name[0] = 0;
			has_versus_mode_streampack_file = 0;
		}
	};
public:
	SceneInfo* scene_infos;
	int scene_infos_len;
public:
	RRE_DISABLE_COPY(Cars2SceneDatabase);
	Cars2SceneDatabase();
	~Cars2SceneDatabase();
	void Create();
	SceneInfo* GetSceneInfo(const char* name);
};

extern Cars2SceneDatabase* lpGlobalSceneDatabase;

#ifdef _M_IX86
static_assert(sizeof(Cars2SceneDatabase::SceneInfo) == 0x54);
static_assert(sizeof(Cars2SceneDatabase) == 0x8);
#endif
