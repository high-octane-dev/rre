#pragma once
#include "cars2_event_info.hpp"
#include "parameter_block.hpp"
#include "math/vector4.hpp"
#include "util/macros.hpp"

enum class EventJoinPointType : std::uint32_t {
	None = 0,
	Race = 1,
	Minigame = 2,
	RaceAndMinigame = 3,
	RamoneShop = 4,
	LuigiGarage = 5,
};

struct EventInfoStartPair {
	Cars2EventInfo* event;
	char event_start[40];
};

class Cars2EventJoinPoint {
public:
	class MenuItem {
	public:
		char name[40];
		EventInfoStartPair* cutscene;
		EventInfoStartPair* event;
		char unk_name[40];
		int unk_index;
		Cars2EventJoinPoint* parent;
	public:
		RRE_DISABLE_COPY(MenuItem);
		MenuItem();
		~MenuItem();
		void Create(ParameterBlock* file);
	};
public:
	char name[40];
	MenuItem* menu_items;
	int menu_items_len;
	EventJoinPointType type;
	char music_track_name[30];
	float music_radius;
	Cars2EventInfo* explore_hub_event;
	Vector4 position;
	Vector4 look_vector;
	Vector4 up_vector;
	int zone;
public:
	RRE_DISABLE_COPY(Cars2EventJoinPoint);
	Cars2EventJoinPoint();
	~Cars2EventJoinPoint();
	void Create(ParameterBlock* file);
};

class Cars2EventJoinPointManager {
public:
	Cars2EventJoinPoint* event_join_points;
	int event_join_points_len;
public:
	RRE_DISABLE_COPY(Cars2EventJoinPointManager);
	Cars2EventJoinPointManager();
	~Cars2EventJoinPointManager();
	void Create();
	Cars2EventJoinPoint* GetEventJoinPoint(const char* name);
	void ProcessEventJoinPointToyFile(const char* path);
};

extern Cars2EventJoinPointManager* lpGlobalEventJoinPointManager;

#ifdef _M_IX86
static_assert(sizeof(EventInfoStartPair) == 0x2c);
static_assert(sizeof(Cars2EventJoinPoint::MenuItem) == 0x60);
static_assert(sizeof(Cars2EventJoinPoint) == 0x90);
static_assert(sizeof(Cars2EventJoinPointManager) == 0x8);
#endif
