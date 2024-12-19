#define NOMINMAX

#include "game.hpp"
#include <BASS.h>

Game* lpGame = nullptr;

LanguageConfiguration* Game::GetLanguageConfiguration(char* name)
{
    for (auto& elem : lang_confs) {
        if (_stricmp(elem->Name, name) == 0) {
            return elem;
        }
    }
    return nullptr;
}

void Game::SetLanguageConfiguration(char* name)
{
    auto conf = GetLanguageConfiguration(name);
    if (conf != nullptr && selected_language_configuration != conf) {
        selected_language_configuration = conf;
        UpdateLocalizedPaths();
    }
}

int Game::GetLanguageDefinitionIndex(char* name)
{
    return 0;
}

Game::Game()
{
    lpGame = this;
    this->unk = 0;
    this->screen_fade_complete = 0;
    this->world_ticks = 0;

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

Game::~Game()
{
    
}

int Game::InitializeRenderer(const char*)
{
    return 0;
}

int Game::Initialize() {}

void Game::CreateStringTables() {}

void Game::AddTimeIntervalCallback(void (**callback)(int, float, void*), void* time_interval) {
    TimeIntervalCallback* time_interval_callback = new TimeIntervalCallback{callback, time_interval};
    time_interval_callbacks.Append(time_interval_callback);
}

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

int Game::PreWindowInitialize() {}

int Game::PreGameInitialize(struct DisplayMode*) {}

int Game::LoadResources() {}

void Game::LoadConfigFile(ParameterBlock*) {}

void Game::BeginAutoTest() {}

void Game::EndAutoTest(int) {
    std::abort();
}

int Game::CreateMaterialPropertyManager() {}

int Game::Activate() {}

int Game::Deactivate() {
    BASS_Pause();
    return 1;
}

int Game::SetBasicRenderStates() {}

void Game::PrepareFrame() {}

int Game::UpdateScreen() {}

int Game::Tick() {}

int Game::PauseGame(int) {}

int Game::EventHandler(int event) {
    return parent_game_object->EventHandler(event) != 0;
}

int Game::KeyUpHandler(ProcessNode* node, KeyInfo* key) {
    if ((this->flags & 8) != 0) {
        return 1;
    }
    return parent_game_object->KeyUpHandler(node, key) != 0;
}

int Game::KeyDownHandler(ProcessNode* node, KeyInfo* key) {
    if ((this->flags & 8) != 0) {
        return 1;
    }
    return parent_game_object->KeyDownHandler(node, key) != 0;
}

void Game::LoadMaterials(int, int) {}

void Game::ReloadMaterials(int) {}

void Game::ReloadMaterialsAndTextures(int) {}

void Game::LoadShaders() {}

void Game::SetCameras(Camera*, Camera*, Camera*, Camera*) {}

void Game::GetCameras(Camera* dest[4]) {
    dest[0] = this->cameras[0];
    dest[1] = this->cameras[1];
    dest[2] = this->cameras[2];
    dest[3] = this->cameras[3];
}

int Game::Terminate() {}

int Game::GetFullPathToContentFile(const char*, char*) {}

int Game::SetDisplayMode(struct DisplayMode*) {}

void Game::InitPlatformGraphicsCallback() {}

int Game::GetResourceString(const char*, char* dest) {
    static char Default[] = "No Strings Available";
    std::memcpy(dest, Default, 21);
}

int Game::Timeout(int) {}

void Game::ReportMemoryUsage(char*) {}

void Game::UpdateLocalizedPaths() {}

void Game::UpdateTextureContentDirectories() {}

RenderTarget* CreateRenderTarget() {}

int Game::PostDisplayModeChange() {}
