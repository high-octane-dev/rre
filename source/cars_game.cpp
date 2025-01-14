#include "cars_game.hpp"
#include <globals.hpp>
#include "gfx/x360_tex_map.hpp"

// Since I'm not sure how this gets freed in the original game, I'm just going to use a std::unique_ptr.
std::unique_ptr<CarsGame> lpCarsGame = std::make_unique<CarsGame>();

char g_UILocalizedTextureContentDirectory[260] = "E\\UI\\Tex\\";
char g_LocalizedUIContentDirectory[260] = "E\\UI\\";
char g_UITextureContentDirectory[260] = "C\\UI\\Tex\\";
char g_BaseUIContentDirectory[260] = "C\\UI\\";
char g_LocalizationContentDirectory[260] = "E\\Loc\\";
char g_FontTextureContentDirectory[260] = "E\\Font\\";
char g_AudioDialogueDirectory[260] = "E\\Audio\\";

// OFFSET: 0x004f50a0
int CarsGame::InitializeRenderer(char* message) {
    SetLanguageToDefault();
    if (X360Game::InitializeRenderer(message) == 0) {
        snprintf(message, 260, "Failed to initialize renderer!");
        return 0;
    }
    unk_game_state = 1;
    return 1;
}

// OFFSET: 0x0051c130
int CarsGame::Initialize() {
    if (splash_screen[0] != '\0') {
        ShowLoadingScreen(splash_screen, "ld_mcq", 1);
    }
    /*
    char game_info_path[260];
    snprintf(game_info_path, sizeof(game_info_path), "%s%sGameInfo.%s.res", "Res\\Data\\", "C\\Appstart\\", "x360");
    ResourceSetup(game_info_path, 0xffffffff, 1, 8, 0, 0, -1, 1, 0);
    cars_settings = new CarsSettings();
    cars_settings->Create();
    */
    SetConfigArguments();
    return 1;
}

// OFFSET: 0x0043fe80
int CarsGame::PreGameInitialize(DisplayMode*) {
    X360Game::PreGameInitialize(nullptr);
    /*
    _timeGameInit = timeGetTime();
    RSStringUtil::Ssnprintf(%sDebugStr.mst,0x104,"%sDebugStr.mst",s_C\Debug\_006f4970);
    (*((lpMaterialStringTable->StringTable).vtable)->Load)(%sDebugStr.mst);
    RSStringUtil::Ssnprintf(%s%sMaterialTemplate.%s.res,0x104,"%s%sMaterialTemplate.%s.res",s_Res\Data\_006f73d8,s_C\AppStart\_006f6da8,"x360");
    bVar2 = g_ResBuilding == 0;
    if (bVar2) {
        ResourceSetup::ResourceSetup(%s%sMaterialTemplate.%s.res,0xffffffff,1,8,0,0,-1,1,0);
    }
    RSStringUtil::Ssnprintf(&stack0xfffffdf0,0x104,"%sDfltMT",s_C\AppStart\_006f6da8);
    X360MaterialTemplate::LoadFromFile(lpSceneObjectMaterialTemplate,&stack0xfffffdf0);
    RSStringUtil::Ssnprintf(&stack0xfffffdf0,0x104,"%sIconMT",s_C\AppStart\_006f6da8);
    X360MaterialTemplate::LoadFromFile(lpIconMaterialTemplate,&stack0xfffffdf0);
    if (bVar2) {
        ResourceSetup::ResourceFinish(%s%sMaterialTemplate.%s.res,1);
    }
    */
    return 1;
}

// OFFSET: 0x00440360
void CarsGame::LoadConfigFile(ParameterBlock*) {
}

// OFFSET: 0x00440e00
void CarsGame::BeginAutoTest() {
}

// OFFSET: 0x004412b0
void CarsGame::EndAutoTest(int) {
}

// OFFSET: 0x004f51d0
int CarsGame::Tick() {
    return 0;
}

// OFFSET: 0x00440320
int CarsGame::KeyUpHandler(ProcessNode*, KeyInfo*) {
    return 0;
}

// OFFSET: 0x00440340
int CarsGame::KeyDownHandler(ProcessNode*, KeyInfo*) {
    return 0;
}

// OFFSET: 0x0051c730
int CarsGame::Terminate() {
    return 0;
}

// OFFSET: 0x00441b70
void CarsGame::ReportMemoryUsage(char*) {
}

// OFFSET: 0x00487b80
void CarsGame::UpdateLocalizedPaths() {
    char* prefix = language_prefixes[lang_defs[selected_language_configuration->TextLanguageIndex]->prefix_index];
    g_LocalizationContentDirectory[0] = *prefix;
    g_FontTextureContentDirectory[0] = *prefix;
    g_LocalizedUIContentDirectory[0] = *prefix;
    g_AudioDialogueDirectory[0] = *prefix;
    UpdateTextureContentDirectories();
}

// OFFSET: 0x00441d80
void CarsGame::UpdateTextureContentDirectories() {
    if (g_ScreenMode == 2) {
        snprintf(g_UILocalizedTextureContentDirectory, 260, "%s%s", g_LocalizedUIContentDirectory, "Tex_HD\\");
        snprintf(g_UITextureContentDirectory, 260, "%s%s", g_BaseUIContentDirectory, "Tex_HD\\");
    }
    else {
        snprintf(g_UILocalizedTextureContentDirectory, 260, "%s%s", g_LocalizedUIContentDirectory, "Tex\\");
        snprintf(g_UITextureContentDirectory, 260, "%s%s", g_BaseUIContentDirectory, "Tex\\");
    }
}

// OFFSET: 0x00441840
void CarsGame::ReloadMaterials() {
    Game::ReloadMaterials(0);
}

// OFFSET: 0x00441850
void CarsGame::ReloadMaterialsAndTextures() {
    Game::ReloadMaterialsAndTextures(0);
}

// OFFSET: 0x0043fa70
void CarsGame::AddVisibilitySearchList(ContainerList<GameObject*>*) {
}

// OFFSET: 0x00441cb0
void CarsGame::ClearCheatCodes() {
    for (std::size_t i = 0; i < sizeof(cheat_codes) / sizeof(CheatCode); i++) {
        cheat_codes[i].active = 0;
    }
}

// OFFSET: 0x00487ac0
void CarsGame::CreateActivityDatabase() {
}

// OFFSET: 0x004873c0
void CarsGame::CreateActivityManager() {
}

// OFFSET: 0x00487870
void CarsGame::CreateBoltManager(int) {
}

// OFFSET: 0x004c3b70
void CarsGame::CreateBonusContentManager() {
}

// OFFSET: 0x004c3a10
void CarsGame::CreateBonusPointManager() {
}

// OFFSET: 0x004c3c40
void CarsGame::CreateControllerSettings() {
}

// OFFSET: 0x00487b20
void CarsGame::CreateEventDatabase() {
}

// OFFSET: 0x00441470
void CarsGame::CreateLoadingIcon(const char* name) {
}

// OFFSET: 0x00441370
void CarsGame::CreateLoadingScreen(const char* name) {
    /*
    char package_name[260]{};
    snprintf(package_name,0x104,"%s%s%s.%s.res","Res\\Data\\", g_UILocalizedTextureContentDirectory, name,"x360");
    ResourceSetup::ResourceSetup(local_20c,0xffffffff,1,8,0,0,-1,1,0);
    */
    char texture_name[260]{};
    snprintf(texture_name, 0x104,"%s%s", g_UILocalizedTextureContentDirectory, name);
    TextureMap* texture_map = X360TexMap::GetTextureMapFromResourceName(texture_name, 555, 0);
    if (texture_map != nullptr) {
        /*
        // always zero
        iVar2 = FUN_00413600();
        
        FUN_00413620(iVar2);
        X360FullScreenTextureRender::FUN_00413460(&g_FullscreenRender,iVar2,texture_map);
        X360FullScreenTextureRender_ARRAY_006fd378[iVar2 * 4 + 3].unk = &g_FullscreenRender;
        FUN_00413670(iVar2);
        
        texture_map->Release();
        */
    }
}

// OFFSET: 0x00487990
void CarsGame::CreatePickupManager() {
}

// OFFSET: 0x0051c030
void CarsGame::CreateProfileManager() {
}

// OFFSET: 0x004c3960
void CarsGame::CreateRecordLibraries() {
}

// OFFSET: 0x004f54e0
void CarsGame::CreateScavengerHuntManager() {
}

// OFFSET: 0x004f5560
void CarsGame::CreateStoryManager() {
}

// OFFSET: 0x004c3aa0
void CarsGame::CreateStreamManager() {
}

// OFFSET: 0x004877d0
void CarsGame::CreateUI() {
}

// OFFSET: 0x004c3bd0
void CarsGame::CreateUIResourceManager() {
}

// OFFSET: 0x004419e0
void CarsGame::CreateVideoFXManager(int) {
}

// OFFSET: 0x00441970
void CarsGame::CreateVirtualNetwork() {
}

// OFFSET: 0x00422c80
void CarsGame::UNK_00422c80(int, int) {
}

// OFFSET: 0x00487470
void CarsGame::UNK_00487470() {
}

// OFFSET: 0x0050b890
void CarsGame::LoadWorld() {
}

// OFFSET: 0x0043ffc0
void CarsGame::MapAllGameKeys() {
}

// OFFSET: 0x00441910
void CarsGame::PrepareDeferredLoad(DeferredLoad) {
}

// OFFSET: 0x004237a0
void CarsGame::PresentFrame(int) {
}

// OFFSET: 0x00441b40
bool CarsGame::ReleaseVideoFXManager() {
    /*
    if (video_fx_manager != nullptr) {
        video_fx_manager->Release();
        video_fx_manager = nullptr;
    }
    */
    return true;
}

// OFFSET: 0x00441660
int CarsGame::ResetLoadingScreenAndIcon() {
    return 0;
}

// OFFSET: 0x004416b0
void CarsGame::SetConfigArguments() {
    strncpy(activity, "FrontEndUI", 11);
    start[0] = '\0';
    scene[0] = '\0';
    scene_name[0] = '\0';
    snprintf(splash_screen, 260, "load_logo");
    unused24 = 0;
    char_number = 0;
    strncpy(char_name, "McQ", 4);
    char_paint_type = 1;
    char_color1[0] = 0.0f;
    char_color1[1] = 0.0f;
    char_color1[2] = 0.0f;
    char_color1[3] = 1.0f;
    char_color2[0] = 0.0f;
    char_color2[1] = 0.0f;
    char_color2[2] = 0.0f;
    char_color2[3] = 1.0f;
    vehicle_manual_transmission = 0;
    no_321 = 0;
    build_spline_mode = 0;
    brake_when_resting_threshold = 5.0f;
    brake_when_resting = 0;
    analog_gas = 0;
    camera_type = 0;
    unlock_all = 0;
    unlock_all_abilities = 0;
    game_mode = 0;
    time_of_day = 0;
    show_lap_timer = 0;
    starting_bolt_banners = 0;
    starting_bonus_points = 0;
    number_of_players = 1;
    load_ai = 1;
    total_laps = 3;
    use_vehicle_engine_sound = 1;
    play_music = 1;
    control_type = 1;
    difficulty = 1;
    always_show_gps = 1;
}

// OFFSET: 0x00440c90
void CarsGame::SetLanguageToDefault() {
    XboxLanguage selected_dash_lang = GetSystemLanguage();
    if (selected_dash_lang == XboxLanguage::None) {
        XboxLocale selected_locale = GetSystemLocale();
        if (selected_locale == XboxLocale::None) {
            SetLanguageConfiguration("English");
            return;
        }
        switch (selected_locale) {
        case XboxLocale::Denmark:
            SetLanguageConfiguration("Danish");
            return;
        case XboxLocale::Netherlands:
            SetLanguageConfiguration("Dutch");
            return;
        case XboxLocale::Norway:
            SetLanguageConfiguration("Norwegian");
            return;
        case XboxLocale::Sweden:
            SetLanguageConfiguration("Swedish");
            return;
        default:
            SetLanguageConfiguration("English");
            return;
        }
    }
    switch (selected_dash_lang)
    {
    case XboxLanguage::English:
        SetLanguageConfiguration("English");
        return;
    case XboxLanguage::German:
        SetLanguageConfiguration("German");
        return;
    case XboxLanguage::French:
        SetLanguageConfiguration("French");
        return;
    case XboxLanguage::Spanish:
        SetLanguageConfiguration("Spanish");
        return;
    case XboxLanguage::Italian:
        SetLanguageConfiguration("Italian");
        return;
    default:
        SetLanguageConfiguration("English");
        break;
    }
}

// OFFSET: 0x00441570
int CarsGame::ShowLoadingScreen(const char* _loading_screen, const char* _loading_icon, int is_first) {
    if (is_first == 0 && _loading_icon != nullptr && _loading_screen != nullptr) {
        if (_stricmp(_loading_screen, loading_screen_name) == 0 && _stricmp(_loading_icon, loading_icon_name) == 0) {
            return 1;
        }
    }
    /*
    if (loading_icon != nullptr) {
        loading_icon->Disable();
        delete loading_icon;
        loading_icon = nullptr;
    }
    */
    if (_loading_screen == nullptr) {
        loading_screen_name[0] = '\0';
    }
    else {
        CreateLoadingScreen(_loading_screen);
        snprintf(loading_screen_name, 0x104, "%s", _loading_screen);
    }
    if (_loading_icon == nullptr) {
        loading_icon_name[0] = '\0';
        return 1;
    }
    CreateLoadingIcon(_loading_icon);
    snprintf(loading_icon_name, 0x104, "%s", _loading_icon);
    return 1;
}

// OFFSET: 0x00422db0
void CarsGame::UnkRegistryGetValue(char*, unsigned int) {
}

// OFFSET: 0x00441870
void CarsGame::UnloadStringTables() {
}

// OFFSET: 0x00487700
void CarsGame::UnloadWorld() {
}
