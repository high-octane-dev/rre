#define NOMINMAX

#include <BASS.h>
#include "game.hpp"
#include "data_access.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_render_target.hpp"

Game* lpGame = nullptr;
X360MaterialTemplate* lpSceneObjectMaterialTemplate = nullptr;
X360MaterialTemplate* lpIconMaterialTemplate = nullptr;
StringTable* lpMaterialStringTable = nullptr;

// OFFSET: 0x00618220, STATUS: TODO
void Game::DestroyStatisticsLog() {
}

// OFFSET: 0x006182c0, STATUS: COMPLETE
LanguageConfiguration* Game::GetLanguageConfiguration(const char* name) {
    for (auto& elem : lang_confs) {
        if (_stricmp(elem->name, name) == 0) {
            return elem;
        }
    }
    return nullptr;
}

// OFFSET: 0x00618310, STATUS: COMPLETE
void Game::SetLanguageConfiguration(const char* name) {
    LanguageConfiguration* conf = GetLanguageConfiguration(name);
    if (conf != nullptr && selected_language_configuration != conf) {
        selected_language_configuration = conf;
        UpdateLocalizedPaths();
    }
}

// OFFSET: 0x00618270, STATUS: COMPLETE
int Game::GetLanguageDefinitionIndex(char* name) {
    for (std::size_t i = 0; i < lang_defs.Length(); i++) {
        if (_stricmp(lang_defs[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// OFFSET: 0x00618ce0, STATUS: TODO
void Game::HandleClick(unsigned int u_msg, unsigned int button, int x, int y) {
}

// OFFSET: 0x00618580, STATUS: WIP
Game::Game() {
    lpGame = this;
    this->unk = 0;
    this->screen_fade_complete = 0;
    this->world_ticks = 0;
    
    lpDataAccess = new DataAccess();
    lpVirtualFileAllocator = new BlockAllocator(sizeof(VirtualDataFile), 1024 * sizeof(VirtualDataFile));

    lpSceneObjectMaterialTemplate = new X360MaterialTemplate();
    lpIconMaterialTemplate = new X360MaterialTemplate();

    /* 
    lpMotionLibrary = new MotionLibrary();
    */
    g_RenderTarget = nullptr;
    g_VideoCard = nullptr;


    this->flags = this->flags & 0xf9 | 0x40;
    this->control_interface = nullptr;
    this->material_property_manager = nullptr;
    this->managers = nullptr;
    this->parent_game_object = nullptr;
    this->unk2 = 1;
    this->camera_index = 0;
    // DAT_0071af90 = 0;
    this->debug_flags = this->debug_flags & 0xe7;
    this->flags = this->flags & 0xe7 | 0x20;
    this->game_time_sec_unpaused = 0;
    this->game_time_sec = 0;
    this->prev_time_ms = 0;
    this->blank_timestamp_2 = 0;
    this->curr_time_ms = 0;
    this->blank_timestamp = 0;
    SetCameras(nullptr, nullptr, nullptr, nullptr);
    this->debug_flags = this->debug_flags & 0xfb;
    this->camera_count = 1;
    this->viewport_index = 0;
    this->dt_seconds = 0.0;
    this->paused_frame_count = 0;
    this->dt_seconds_unpaused = 0.0;
    this->rendered_objects = 0;
    /*
    pbVar4 = &DAT_006fa9d8;
    bVar2 = DAT_006fa9d8;
    while (bVar2 != 0) {
        *pbVar4 = *pbVar4 ^ 0x5b;
        pbVar4 = pbVar4 + 1;
        bVar2 = *pbVar4;
    }
    pbVar4 = &DAT_006fa9f4;
    bVar2 = DAT_006fa9f4;
    while (bVar2 != 0) {
        *pbVar4 = *pbVar4 ^ 0x5b;
        pbVar4 = pbVar4 + 1;
        bVar2 = *pbVar4;
    }
    */
    this->frames_per_second = 0;
    this->flags = this->flags | 0x80;
    this->smoothed_fractional_sec_game_time = 0.0;
    this->min_fractional_sec_game_time = 10000.0;
    this->debug_flags = this->debug_flags & 0x3c | 3;
    this->max_fractional_sec_game_time = 0.0;
    this->frames_in_sec = 0;
    this->should_calc_time_within_sec = 0;
    this->statistics_log = (StatisticsLog*)0x0;
    this->enable_statistics_logging = 0;
    this->language_prefixes.CLNonMacroCreate(8, 8, std::numeric_limits<int>::max());
    this->lang_defs.CLNonMacroCreate(8, 8, std::numeric_limits<int>::max());
    this->lang_confs.CLNonMacroCreate(8, 8, std::numeric_limits<int>::max());
    this->selected_language_configuration_name[0] = '\0';
    this->selected_language_configuration = nullptr;
    this->use_dashboard_locale = 0;
}

// OFFSET: 0x00618a50, STATUS: WIP
Game::~Game() {
    if (statistics_log != nullptr) {
        DestroyStatisticsLog();
    }
    if (g_RenderTarget != nullptr) {
        delete g_RenderTarget;
        g_RenderTarget = nullptr;
    }
    if (g_VideoCard != nullptr) {
        g_VideoCard->FreeCursor();
        delete g_VideoCard;
        g_VideoCard = nullptr;
    }
    /*
    if (DAT_0071af90 != nullptr) {
        fclose(DAT_0071af90);
    }
    */
    lpDataAccess->Release();
    lpVirtualFileAllocator->Release();

    for (auto& elem : time_interval_callbacks) {
        if (elem != nullptr) {
            delete elem;
        }
    }

    for (auto& elem : lang_confs) {
        if (elem != nullptr) {
            delete elem;
        }
    }
    
    for (auto& elem : lang_defs) {
        if (elem != nullptr) {
            delete elem;
        }
    }
}

// OFFSET: 0x00617d30, STATUS: TODO
int Game::Initialize() {
    return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Game::InitializeRenderer(char(&quit_message)[260]) {
    return 0;
}

// OFFSET: 0x00616ee0, STATUS: TODO
void Game::CreateStringTables() {
}

// OFFSET: 0x00618980, STATUS: COMPLETE
void Game::AddTimeIntervalCallback(void (**callback)(int, float, void*), void* time_interval) {
    TimeIntervalCallback* time_interval_callback = new TimeIntervalCallback{callback, time_interval};
    time_interval_callbacks.Append(time_interval_callback);
}

// OFFSET: 0x006189c0, STATUS: COMPLETE
void Game::RemoveTimeIntervalCallback(void (**callback)(int, float, void*), void* time_interval) {
    TimeIntervalCallback* data = nullptr;
    for (auto& elem : time_interval_callbacks) {
        if (elem->callback == callback && elem->time_interval == time_interval) {
            data = elem;
        }
    }
    time_interval_callbacks.RemoveElement(data);
    delete data;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Game::PreWindowInitialize() {
    return 1;
}

// OFFSET: 0x006170c0, STATUS: WIP
int Game::PreGameInitialize(struct DisplayMode*) {
    /*
    if (control_interface == nullptr) {
        control_interface = new X360ControlInterface();
        if (control_interface->Create() == 0) {
            return 0;
        }
    }
    */
    CreateStringTables();
    return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int Game::LoadResources() {
    return 0;
}

// OFFSET: 0x00617ac0, STATUS: TODO
void Game::LoadConfigFile(ParameterBlock*) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void Game::BeginAutoTest() {
}

// OFFSET: 0x00617990, STATUS: COMPLETE
void Game::EndAutoTest(int) {
    std::abort();
}

// OFFSET: 0x00617180, STATUS: TODO
int Game::CreateMaterialPropertyManager() {
    return 0;
}

// OFFSET: 0x006171b0, STATUS: COMPLETE
int Game::Activate() {
    BASS_Start();
    return 1;
}

// OFFSET: 0x006171c0, STATUS: COMPLETE
int Game::Deactivate() {
    BASS_Pause();
    return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
void Game::PrepareFrame() {
}

// OFFSET: 0x00617370, STATUS: TODO
int Game::UpdateScreen() {
    return 0;
}

// OFFSET: 0x006173c0, STATUS: TODO
int Game::Tick() {
    return 0;
}

// OFFSET: 0x006177d0, STATUS: TODO
int Game::PauseGame(int) {
    return 0;
}

// OFFSET: 0x00617810, STATUS: COMPLETE
int Game::EventHandler(int event) {
    return parent_game_object->EventHandler(event) != 0;
}

// OFFSET: 0x00617830, STATUS: COMPLETE
int Game::KeyUpHandler(ProcessNode* node, KeyInfo* key) {
    if ((this->flags & 8) != 0) {
        return 1;
    }
    return parent_game_object->KeyUpHandler(node, key) != 0;
}

// OFFSET: 0x00617870, STATUS: COMPLETE
int Game::KeyDownHandler(ProcessNode* node, KeyInfo* key) {
    if ((this->flags & 8) != 0) {
        return 1;
    }
    return parent_game_object->KeyDownHandler(node, key) != 0;
}

// OFFSET: 0x006178b0, STATUS: TODO
void Game::LoadMaterials(int, int) {
}

// OFFSET: 0x00617900, STATUS: TODO
void Game::ReloadMaterials(int) {
}

// OFFSET: 0x00617930, STATUS: TODO
void Game::ReloadMaterialsAndTextures(int) {
}

// OFFSET: 0x00617970, STATUS: TODO
void Game::LoadShaders() {
}

// OFFSET: 0x00617200, STATUS: TODO
void Game::SetCameras(Camera*, Camera*, Camera*, Camera*) {
}

// OFFSET: 0x00617280, STATUS: COMPLETE
void Game::GetCameras(Camera* dest[4]) {
    dest[0] = this->cameras[0];
    dest[1] = this->cameras[1];
    dest[2] = this->cameras[2];
    dest[3] = this->cameras[3];
}

// OFFSET: 0x00617ff0, STATUS: WIP
int Game::Terminate() {
    // TODO: Not completely implemented
    if (g_RenderTarget != nullptr) {
        delete g_RenderTarget;
        g_RenderTarget = nullptr;
    }
    if (g_VideoCard != nullptr) {
        g_VideoCard->FreeCursor();
        delete g_VideoCard;
        g_VideoCard = nullptr;
    }
    delete lpSceneObjectMaterialTemplate;
    delete lpIconMaterialTemplate;
    return 0;
}

// OFFSET: 0x006179a0, STATUS: TODO
int Game::GetFullPathToContentFile(const char*, char*) {
    return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void Game::InitPlatformGraphicsCallback(DisplayMode*) {
}

// OFFSET: 0x00617a20, STATUS: COMPLETE
int Game::GetResourceString(const char*, char* dest) {
    static char Default[] = "No Strings Available";
    std::memcpy(dest, Default, 21);
    return 0;
}

// OFFSET: 0x00617a60, STATUS: TODO
int Game::Timeout(int) {
    return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void Game::ReportMemoryUsage(char*) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void Game::UpdateLocalizedPaths() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void Game::UpdateTextureContentDirectories() {
}

// OFFSET: 0x006171d0, STATUS: TODO
int Game::PostDisplayModeChange() {
    return 0;
}
