#pragma once
#include <memory>
#include "x360_game.hpp"
#include "gfx/x360_full_screen_render_pass.hpp"

enum class XboxLocale {
    None = 0,
    Australia = 1, // UNUSED
    Austria = 2, // UNUSED
    Belgium = 3, // UNUSED
    Brazil = 4, // UNUSED
    Canada = 5, // UNUSED
    Chile = 6, // UNUSED
    China = 7, // UNUSED
    Colombia = 8, // UNUSED
    CzechRepublic = 9, // UNUSED
    Denmark = 10,
    Finland = 11, // UNUSED
    France = 12, // UNUSED
    Germany = 13, // UNUSED
    Greece = 14, // UNUSED
    HongKong = 15, // UNUSED
    Hungary = 16, // UNUSED
    India = 17, // UNUSED
    Ireland = 18, // UNUSED
    Italy = 19, // UNUSED
    Japan = 20, // UNUSED
    Korea = 21, // UNUSED
    Mexico = 22, // UNUSED
    Netherlands = 23,
    NewZealand = 24, // UNUSED
    Norway = 25,
    Poland = 26, // UNUSED
    Portugal = 27, // UNUSED
    Singapore = 28, // UNUSED
    SlovakRepublic = 29, // UNUSED
    SouthAfrica = 30, // UNUSED
    Spain = 31, // UNUSED
    Sweden = 32,
    Switzerland = 33, // UNUSED
    Taiwan = 34, // UNUSED
    GreatBritain = 35, // UNUSED
    UnitedStates = 36, // UNUSED
    RussianFederation = 37 // UNUSED
};

enum class XboxLanguage {
    None,
    English = 1,
    Japanese = 2, // UNUSED
    German = 3,
    French = 4,
    Spanish = 5,
    Italian = 6,
    Korean = 7, // UNUSED
    TraditionalChinese = 8, // UNUSED
    Portuguese = 9, // UNUSED
    SimplifiedChinese = 10, // UNUSED
    Polish = 11, // UNUSED
    Russian = 12, // UNUSED
};

// OFFSET: 0x00427420
inline XboxLanguage TryGetLanguage(const std::string_view& str) {
    if (str == "English") return XboxLanguage::English;
    if (str == "German") return XboxLanguage::German;
    if (str == "French") return XboxLanguage::French;
    if (str == "Spanish") return XboxLanguage::Spanish;
    if (str == "Italian") return XboxLanguage::Italian;
    return XboxLanguage::None;
}

// OFFSET: 0x004274b0
inline XboxLocale TryGetLocale(const std::string_view& str) {
    if (str == "Danish") return XboxLocale::Denmark;
    if (str == "Dutch") return XboxLocale::Netherlands;
    if (str == "Norwegian") return XboxLocale::Norway;
    if (str == "Swedish") return XboxLocale::Sweden;
    return XboxLocale::None;
}

#ifdef _WIN32
#define GetSystemLanguage() TryGetLanguage(g_LanguageName)
#define GetSystemLocale() TryGetLocale(g_LanguageName)
#endif // _WIN32

enum DeferredLoad {
    Unk0,
    Unk1,
    Unk2,
    Unk3,
    Unk4
};

struct CheatCode {
    int index;
    char name[20];
    int active;
};

class CarsGame : public X360Game {
public:
    char unk[128];
    struct CarsProfile* profile;
    struct CarsGlobalProfile* global_profile;
    struct CarsRecordLibrary* hall_of_fame;
    struct CarsRecordLibrary* personal_records;
    struct CarsAudioManager* audio_manager;
    struct CarsActivityManager* activity_manager;
    struct CarsWorld* world;
    struct CarsUI* ui;
    struct VideoFXManager* video_fx_manager;
    struct CarsBonusContentManager* bonus_content_manager;
    struct CarsControllerSettings* controller_settings;
    struct CarsSettings* settings;
    struct CarsBoltManager* bolt_manager;
    struct Cars2PickupManager* pickup_manager;
    struct CarsScavengerHuntManager* scavenger_hunt_manager;
    struct Cars2BonusPointManager* bonus_point_manager;
    struct CarsAchievementManager* achievement_manager;
    struct Cars2SceneDatabase* scene_database;
    struct Cars2ActivityDatabase* activity_database;
    struct Cars2EventDatabase* event_database;
    struct Cars2VehicleDatabase* vehicle_database;
    struct Cars2StoryManager* story_manager;
    struct Cars2EventJoinPointManager* event_join_point_manager;
    struct Cars2UIResourceManager* ui_resource_manager;
    GameObject* unused22;
    char scene[64];
    char scene_name[64];
    char activity[64];
    char start[64];
    char unused23[64];
    char splash_screen[64];
    char player_start[64];
    char ai_names[64];
    int unused24;
    int unused25;
    int no_321;
    int total_laps;
    int use_vehicle_engine_sound;
    int play_music;
    int build_spline_mode;
    int brake_when_resting;
    int loop_cutscenes;
    int loading_tip_preview;
    float brake_when_resting_threshold;
    char char_name[32];
    char char2_Name[32];
    int char_number;
    unsigned int char_paint_type;
    int vehicle_manual_transmission;
    int number_of_players;
    int number_of_ai;
    int unused26;
    int load_ai;
    unsigned int viewport_type;
    int analog_gas;
    int show_coords;
    int dump_results;
    int unused27;
    unsigned int control_type;
    unsigned int camera_type;
    DeferredLoad deferred_load;
    int world_paused;
    int unlock_all;
    int force_stub_mode;
    int unlock_all_abilities;
    int unlock_all_bonus_content;
    int unused28;
    int enforce_free_camera_speed;
    int unused29;
    int cinema_mode;
    struct X360LoadingIcon* loading_icon;
    char loading_screen_name[260];
    char loading_icon_name[260];
    Vector4 char_color1;
    Vector4 char_color2;
    int game_mode;
    unsigned int difficulty;
    unsigned int time_of_day;
    int starting_bolt_banners;
    int starting_bonus_points;
    int always_show_gps;
    int show_lap_timer;
    CheatCode cheat_codes[11];
    int unused30;
public:
    CarsGame();

    virtual int InitializeRenderer(char(&quit_message)[260]) override;
    virtual int Initialize() override;
    virtual int PreGameInitialize(DisplayMode*) override;
    virtual void LoadConfigFile(ParameterBlock*) override;
    virtual void BeginAutoTest() override;
    virtual void EndAutoTest(int) override;
    virtual int Tick() override;
    virtual int KeyUpHandler(ProcessNode*, KeyInfo*) override;
    virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;
    virtual int Terminate() override;
    virtual void ReportMemoryUsage(char*) override;
    virtual void UpdateLocalizedPaths() override;
    virtual void UpdateTextureContentDirectories() override;
    virtual void ReloadMaterials();
    virtual void ReloadMaterialsAndTextures();
	
    void AddVisibilitySearchList(ContainerList<GameObject*>*);
    void ClearCheatCodes();
    void CreateActivityDatabase();
    void CreateActivityManager();
    void CreateBoltManager(int);
    void CreateBonusContentManager();
    void CreateBonusPointManager();
    void CreateControllerSettings();
    void CreateEventDatabase();
    void CreateLoadingIcon(const char* name);
    void CreateLoadingScreen(const char* name);
    void CreatePickupManager();
    void CreateProfileManager();
    void CreateRecordLibraries();
    void CreateScavengerHuntManager();
    void CreateStoryManager();
    void CreateStreamManager();
    void CreateUI();
    void CreateUIResourceManager();
    void CreateVideoFXManager(int);
    void CreateVirtualNetwork();

    void UNK_00422c80(int, int);
    void UNK_00487470();
    void LoadWorld();
    void MapAllGameKeys();
    void PrepareDeferredLoad(DeferredLoad);
    void PresentFrame(int);
    bool ReleaseVideoFXManager();
    int ResetLoadingScreenAndIcon();
    void SetConfigArguments();
    void SetLanguageToDefault();
    int ShowLoadingScreen(const char*, const char*, int);
    void UnkRegistryGetValue(char*, unsigned int);
    void UnloadStringTables();
    void UnloadWorld();
};


extern std::unique_ptr<CarsGame> lpCarsGame;

// E\UI\Tex_HD
extern char g_UILocalizedTextureContentDirectory[260];

// E\UI
extern char g_LocalizedUIContentDirectory[260];

// C\UI\Tex_HD
extern char g_UITextureContentDirectory[260];

// C\UI
extern char g_BaseUIContentDirectory[260];

// E\Loc
extern char g_LocalizationContentDirectory[260];

// E\Font
extern char g_FontTextureContentDirectory[260];

// E\Audio
extern char g_AudioDialogueDirectory[260];

// C\Debug
extern char g_DebugDirectory[260];

extern int g_GetActivityTypeFromActivityFile;
extern int g_FixMcqueensHeadquartersStage;
extern int g_McqueensHeadquartersStage;
extern int g_StartOnTitleScreenOnFirstBoot;
extern int g_Boost_Lvl;
extern float g_SuspensionLoadType;
extern int g_EnableVehicleAudio;
extern int g_EnableVehicleMotionBlurOverlay;
extern int g_EnableAllResFiles;
extern int g_EnableActivityStreaming;
extern int g_EnableDialogueMotionStreaming;
extern int g_EnableNonStreamingResFiles;
extern int g_EnableResourceBuilding;
extern int g_ResBuilding;
extern int g_BuildNewResourcesOnly;
extern char g_IniFileOutputDirectory[260];
extern int g_NotInAnimViewer;
extern int g_UseConstantAfterEventTutorial;
extern char g_ConstantAfterEventTutorial[40];
extern int g_ShowMetalDetectorOverlayInExploreHub;
extern int g_ShowScavengerHuntOverlayInExploreHub;
extern int g_ScavengerHuntPartGroupInExploreHub;
extern int g_CheckForXbox360TextureMipMaps;
extern int g_OnlyLoadXbox360LightMapsFromResourceFile;

// FIXME: So, in the original game, this is actually a 1-sized array of X360FullScreenRenderPass* es;
// Every function that accesses that array does so by retrieving an index from some function, but
// those functions always return 0. There is a decent amount of code sprinkled throughout the
// rendering classes in the game that use 1-sized arrays like this, but at the moment we do not
// know enough about the game to declare them as such.
extern X360FullScreenRenderPass* g_CurrentFSRP;
