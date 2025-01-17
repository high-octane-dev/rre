#pragma once
#include "language_definition.hpp"
#include "language_configuration.hpp"
#include "containers/container_list.hpp"
#include "game_object.hpp"
#include "gfx/x360_material_template.hpp"
#include "containers/container_linked_list.hpp"
#include "containers/string_table.hpp"

struct TimeIntervalCallback {
    void (**callback)(int, float, void*);
    void* time_interval;
};

struct Camera {};
struct X360Camera : public Camera {};
struct RenderTarget;
struct DisplayMode {
    int field0_0x0;
    int screen_mode;
    float fps;
};

class Game {
public:
    int unused;
    int use_sound_bank_pack;
    ContainerList<void**> sound_bank_pack;
    int unused1;
    int unk;
    int screen_fade_complete;
    int world_ticks;
    struct X360ControlInterface* control_interface;
    struct MaterialPropertyManager* material_property_manager;
    int unk2;
    int camera_index;
    GameObject* managers;
    int unused2;
    char unused3[260];
    char install_dir[260];
    std::uint8_t flags;
    std::uint8_t debug_flags;
    std::uint8_t shadow_map_flags;
    std::uint8_t padding;
    int unused4;
    int enable_ui;
    int unk_game_state;
    int game_time_sec;
    int game_time_sec_unpaused;
    int unused5;
    int curr_time_ms;
    int blank_timestamp; /* always 0 */
    int prev_time_ms;
    int blank_timestamp_2; /* always 0 */
    int frames_per_second;
    float dt_seconds_unpaused;
    float one_over_dt_seconds_unpaused;
    float unused6;
    struct X360Sound* unused7; /* technically not unused */
    int prev_snd_play_position;
    float unused8;
    float unused9;
    float dt_seconds;
    X360Camera* cameras[4];
    int camera_count;
    int viewport_index;
    int paused_frame_count;
    GameObject* parent_game_object;
    int unused20;
    int unused21;
    int rendered_objects;
    LinkedList<TimeIntervalCallback*> time_interval_callbacks;
    int enable_statistics_logging;
    struct StatisticsLog* statistics_log;
    int unused10;
    struct ScreenTextFontManager* screen_text_font_manager;
    int unused11;
    int unused12;
    float smoothed_fractional_sec_game_time;
    float min_fractional_sec_game_time;
    float max_fractional_sec_game_time;
    int frames_in_sec;
    float unused13;
    float unused14;
    int unused15;
    int should_calc_time_within_sec;
    ContainerList<char*> language_prefixes;
    ContainerList<LanguageDefinition*> lang_defs;
    ContainerList<LanguageConfiguration*> lang_confs;
    char selected_language_configuration_name[32];
    LanguageConfiguration* selected_language_configuration;
    int use_dashboard_locale;
    int unused16;
public:
    Game();

	virtual ~Game();
	virtual int InitializeRenderer(char*);
	virtual int Initialize();
	virtual void CreateStringTables();
	virtual void AddTimeIntervalCallback(void (**)(int, float, void*), void*);
	virtual void RemoveTimeIntervalCallback(void (**)(int, float, void*), void*);
	virtual int PreWindowInitialize();
	virtual int PreGameInitialize(DisplayMode*);
	virtual int LoadResources();
	virtual void LoadConfigFile(ParameterBlock*);
	virtual void BeginAutoTest();
	virtual void EndAutoTest(int);
	virtual int CreateMaterialPropertyManager();
	virtual int Activate();
	virtual int Deactivate();
	virtual int SetBasicRenderStates();
	virtual void PrepareFrame();
	virtual int UpdateScreen();
	virtual int Tick();
    virtual int PauseGame(int);
    virtual int EventHandler(int);
    virtual int KeyUpHandler(ProcessNode*, KeyInfo*);
    virtual int KeyDownHandler(ProcessNode*, KeyInfo*);
    virtual void LoadMaterials(int, int);
    virtual void ReloadMaterials(int);
    virtual void ReloadMaterialsAndTextures(int);
    virtual void LoadShaders();
    virtual void SetCameras(Camera*, Camera*, Camera*, Camera*);
    virtual void GetCameras(Camera*[4]);
    virtual int Terminate();
    virtual int GetFullPathToContentFile(const char*, char*);
    virtual int SetDisplayMode(DisplayMode*);
    virtual void InitPlatformGraphicsCallback();
    virtual int GetResourceString(const char*, char*);
    virtual int Timeout(int);
    virtual void ReportMemoryUsage(char*);
    virtual void UpdateLocalizedPaths();
    virtual void UpdateTextureContentDirectories();
    virtual RenderTarget* CreateRenderTarget();
    virtual int PostDisplayModeChange();

    void DestroyStatisticsLog();
    LanguageConfiguration* GetLanguageConfiguration(const char* name);
    int GetLanguageDefinitionIndex(char* name);
    void HandleClick(unsigned int u_msg, unsigned int button, int x, int y);
    void SetLanguageConfiguration(const char* name);
};

extern Game* lpGame;
extern X360MaterialTemplate* lpSceneObjectMaterialTemplate;
extern X360MaterialTemplate* lpIconMaterialTemplate;
extern StringTable* lpMaterialStringTable;
