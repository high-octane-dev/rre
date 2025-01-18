#include "cars_game.hpp"
#include <globals.hpp>
#include "gfx/x360_tex_map.hpp"
#include "util/rsstring_util.hpp"

// In the original game, this just gets leaked. For now, we'll use std::unique_ptr for portability.
std::unique_ptr<CarsGame> lpCarsGame = std::make_unique<CarsGame>();

char g_UILocalizedTextureContentDirectory[260] = "E\\UI\\Tex\\";
char g_LocalizedUIContentDirectory[260] = "E\\UI\\";
char g_UITextureContentDirectory[260] = "C\\UI\\Tex\\";
char g_BaseUIContentDirectory[260] = "C\\UI\\";
char g_LocalizationContentDirectory[260] = "E\\Loc\\";
char g_FontTextureContentDirectory[260] = "E\\Font\\";
char g_AudioDialogueDirectory[260] = "E\\Audio\\";

int g_GetActivityTypeFromActivityFile = FALSE;
int g_FixMcqueensHeadquartersStage = FALSE;
int g_McqueensHeadquartersStage = TRUE;
int g_StartOnTitleScreenOnFirstBoot = TRUE;
int g_Boost_Lvl = 1;
int g_SuspensionLoadType = 1.0;
int g_EnableVehicleAudio = TRUE;
int g_EnableVehicleMotionBlurOverlay = TRUE;
int g_EnableAllResFiles = TRUE;
int g_EnableActivityStreaming = FALSE;
int g_EnableDialogueMotionStreaming = FALSE;
int g_EnableNonStreamingResFiles = TRUE;
int g_EnableResourceBuilding = FALSE;
int g_ResBuilding = 0;
int g_BuildNewResourcesOnly = TRUE;
char g_IniFileOutputDirectory[260] = "Res\\Ini\\";
int g_NotInAnimViewer = TRUE;
int g_UseConstantAfterEventTutorial = FALSE;
char g_ConstantAfterEventTutorial[40] = "";
int g_ShowMetalDetectorOverlayInExploreHub = FALSE;
int g_ShowScavengerHuntOverlayInExploreHub = FALSE;
int g_ScavengerHuntPartGroupInExploreHub = FALSE;
int g_CheckForXbox360TextureMipMaps = FALSE;
int g_OnlyLoadXbox360LightMapsFromResourceFile = TRUE;

// OFFSET: 0x004f50a0
int CarsGame::InitializeRenderer(char (&quit_message)[260]) {
    SetLanguageToDefault();
    if (X360Game::InitializeRenderer(quit_message) == 0) {
        snprintf(quit_message, sizeof(quit_message), "Failed to initialize renderer!");
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
    // TODO: Not completely implemented
    char material_file_content[260]{};

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
	*/

    RSStringUtil::Ssnprintf(material_file_content, sizeof(material_file_content), "%sDfltMT", "C\\AppStart\\");
    lpSceneObjectMaterialTemplate->LoadFromFile(material_file_content);
    RSStringUtil::Ssnprintf(material_file_content, sizeof(material_file_content), "%sIconMT", "C\\AppStart\\");
    lpIconMaterialTemplate->LoadFromFile(material_file_content);

	/*
    if (bVar2) {
        ResourceSetup::ResourceFinish(%s%sMaterialTemplate.%s.res,1);
    }
    */
    return 1;
}

// OFFSET: 0x00440360
void CarsGame::LoadConfigFile(ParameterBlock* parameter_block) {
    char game_mode_parameter[64];
    char difficulty_parameter[64];
    char story_type_parameter[64];
    char time_of_day_parameter[64];
    char char_paint_type_parameter[260];
    constexpr auto empty_vector = Vector4(0, 0, 0, 0);
    char viewport_type_parameter[260];
    char control_type_parameter[260];
    char camera_type_parameter[260];
    char language_parameter[64];
    int build_resource_files_parameter;
    int build_ini_files_parameter;

    if (parameter_block->ReadParameterBlock("Cars") != 0) {
	    parameter_block->GetParameter("GameMode", "", game_mode_parameter, sizeof(game_mode_parameter));
	    parameter_block->GetParameter("Difficulty", "", difficulty_parameter, sizeof(difficulty_parameter));
	    parameter_block->GetParameter("StoryType", "", story_type_parameter, sizeof(story_type_parameter));
	    parameter_block->GetParameter("TimeOfDay", "", time_of_day_parameter, sizeof(time_of_day_parameter));
	    parameter_block->GetParameter("Activity", "", activity, sizeof(activity));
	    parameter_block->GetParameter("Start", "Player1Start", start, sizeof(start));
	    parameter_block->GetParameter("CharNumber", char_number, &char_number);
	    parameter_block->GetParameter("CharName", "*NONE*", char_name, sizeof(char_name));
	    parameter_block->GetParameter("Char2Name", "*NONE*", char2_Name, sizeof(char2_Name));
	    parameter_block->GetParameter("CharPaintType", "SOLID", char_paint_type_parameter, sizeof(char_paint_type_parameter));
	    parameter_block->GetParameter("CharColor1", &empty_vector, &char_color1);
	    parameter_block->GetParameter("CharColor2", &empty_vector, &char_color2);
	    parameter_block->GetParameter("VehicleManualTransmission", &vehicle_manual_transmission);
	    parameter_block->GetParameter("ViewportType", "", viewport_type_parameter, sizeof(viewport_type_parameter));
	    parameter_block->GetParameter("LoadAI", FALSE, &load_ai);
	    parameter_block->GetParameter("No321", FALSE, &no_321);
	    parameter_block->GetParameter("UseVehicleEngineSound", FALSE, &use_vehicle_engine_sound);
	    parameter_block->GetParameter("PlayMusic", FALSE, &play_music);
	    parameter_block->GetParameter("BuildSplineMode", FALSE, &build_spline_mode);
	    parameter_block->GetParameter("BrakeWhenResting", FALSE, &brake_when_resting);
	    parameter_block->GetParameter("BrakeWhenRestingThreshold", 5.0, &brake_when_resting_threshold);
	    brake_when_resting_threshold *= 1.46666f;
	    parameter_block->GetParameter("AnalogGas", FALSE, &analog_gas);
	    parameter_block->GetParameter("TotalLaps", 3, &total_laps);
	    parameter_block->GetParameter("ShowCoords", TRUE, &show_coords);
	    parameter_block->GetParameter("DumpResults", FALSE, &dump_results);
	    parameter_block->GetParameter("ControlType", "VehicleRelative", control_type_parameter, sizeof(control_type_parameter));
	    parameter_block->GetParameter("CameraType", "Follow", camera_type_parameter, sizeof(camera_type_parameter));
	    parameter_block->GetParameter("UnlockAll", FALSE, &unlock_all);
	    parameter_block->GetParameter("UnlockAllAbilities", FALSE, &unlock_all_abilities);
	    parameter_block->GetParameter("ForceStubMode", FALSE, &force_stub_mode);
	    parameter_block->GetParameter("EnforceFreeCameraSpeed", TRUE, &enforce_free_camera_speed);
	    parameter_block->GetParameter("CinemaMode", FALSE, &cinema_mode);
	    parameter_block->GetParameter("AlwaysShowGPS", FALSE, &always_show_gps);
	    parameter_block->GetParameter("ShowLapTimer", FALSE, &show_lap_timer);
	    parameter_block->GetParameter("LoopCutscenes", FALSE, &loop_cutscenes);
	    parameter_block->GetParameter("SplashScreen", "load_logo", splash_screen, sizeof(splash_screen));
	    parameter_block->GetParameter("StartingBoltBanners", 0, &starting_bolt_banners);
	    parameter_block->GetParameter("StartingBonusPoints", 0, &starting_bonus_points);
	    parameter_block->GetParameter("UnlockAllBonusContent", FALSE, &unlock_all_bonus_content);
	    parameter_block->GetParameter("GetActivityTypeFromActivityFile", FALSE, &g_GetActivityTypeFromActivityFile);
	    parameter_block->GetParameter("LoadingTipPreview", FALSE, &loading_tip_preview);
	    parameter_block->GetParameter("FixMcqueensHeadquartersStage", FALSE, &g_FixMcqueensHeadquartersStage);
	    parameter_block->GetParameter("McqueensHeadquartersStage", TRUE, &g_McqueensHeadquartersStage);
	    parameter_block->GetParameter("StartOnTitleScreenOnFirstBoot", TRUE, &g_StartOnTitleScreenOnFirstBoot);

	    if (_stricmp(game_mode_parameter, "Arcade") == 0) {
		    number_of_players = 1;
		    game_mode = 1;
	    }
	    else if (_stricmp(game_mode_parameter, "VS") == 0 || _stricmp(game_mode_parameter, "Versus") == 0) {
		    number_of_players = 2;
		    game_mode = 2;
	    }
	    else if (_stricmp(game_mode_parameter, "Test") == 0) {
		    number_of_players = 1;
		    game_mode = 3;
	    }
	    else {
		    number_of_players = 1;
		    game_mode = 0;
	    }

	    if (_stricmp(difficulty_parameter, "Easy") == 0)
		    difficulty = 0;
	    else if (_stricmp(difficulty_parameter, "Hard") == 0)
		    difficulty = 2;
	    else if (_stricmp(difficulty_parameter, "Expert") == 0)
		    difficulty = 3;
	    else if (_stricmp(difficulty_parameter, "Practice") == 0)
		    difficulty = 4;
	    else
		    difficulty = 1;

	    time_of_day = _stricmp(time_of_day_parameter, "Night") == 0;

	    parameter_block->GetParameter("Language", "English", language_parameter, sizeof(language_parameter));
	    SetLanguageConfiguration(language_parameter);

	    if (_stricmp(char_paint_type_parameter, "Solid") == 0)
		    char_paint_type = 0;
	    else
		    char_paint_type = _stricmp(char_paint_type_parameter, "Chameleon") == 0;

	    camera_type = _stricmp(camera_type_parameter, "Follow") != 0;

	    if (_stricmp(control_type_parameter, "VehicleRelative") == 0)
		    control_type = 1;
	    else if (_stricmp(control_type_parameter, "CameraRelativeWithPhysics") == 0)
		    control_type = 2;
	    else
		    control_type = 3;

	    if (_stricmp(viewport_type_parameter, "ViewportCodeTwoPlayerHoriz") == 0)
		    viewport_type = 1;
	    else if (_stricmp(viewport_type_parameter, "ViewportCodeTwoPlayerVertical") == 0)
		    viewport_type = 2;
	    else if (_stricmp(viewport_type_parameter, "ViewportCodeOnePlayer") == 0 || game_mode != 2)
		    viewport_type = 0;
	    else
		    viewport_type = RSStringUtil::stristr(activity, "MG_RRR") == nullptr;

	    parameter_block->GetParameter("Boost_Lvl", 1, &g_Boost_Lvl);
	    g_Boost_Lvl = min(g_Boost_Lvl, 3);

	    parameter_block->GetParameter("SuspensionLoadType", 1.0, &g_SuspensionLoadType);
	    parameter_block->GetParameter("EnableVehicleAudio", TRUE, &g_EnableVehicleAudio);
	    parameter_block->GetParameter("EnableVehicleMotionBlurOverlay", TRUE, &g_EnableVehicleMotionBlurOverlay);
	    parameter_block->GetParameter("EnableAllResFiles", FALSE, &g_EnableAllResFiles);
	    parameter_block->GetParameter("EnableActivityStreaming", FALSE, &g_EnableActivityStreaming);
	    parameter_block->GetParameter("EnableDialogueMotionStreaming", FALSE, &g_EnableDialogueMotionStreaming);
	    parameter_block->GetParameter("EnableNonStreamingResFiles", FALSE, &g_EnableNonStreamingResFiles);
	    parameter_block->GetParameter("EnableResourceBuilding", g_EnableResourceBuilding, &g_EnableResourceBuilding);
	    parameter_block->GetParameter("BuildResourceFiles", FALSE, &build_resource_files_parameter);
	    parameter_block->GetParameter("BuildIniFiles", FALSE, &build_ini_files_parameter);

	    if (!build_ini_files_parameter) {
		    if (build_resource_files_parameter)
			    g_ResBuilding = 2;
	    }
	    else
		    g_ResBuilding = 1;

	    parameter_block->GetParameter("BuildNewResourcesOnly", g_BuildNewResourcesOnly, &g_BuildNewResourcesOnly);
	    parameter_block->GetParameter("IniFileOutputDirectory", g_IniFileOutputDirectory, g_IniFileOutputDirectory, sizeof(g_IniFileOutputDirectory));

	    if (_stricmp(activity, "AnimViewer") == 0)
		    g_NotInAnimViewer = FALSE;

	    parameter_block->GetParameter("UseConstantAfterEventTutorial", FALSE, &g_UseConstantAfterEventTutorial);
	    parameter_block->GetParameter("ConstantAfterEventTutorial", "", g_ConstantAfterEventTutorial, sizeof(g_ConstantAfterEventTutorial));
	    parameter_block->GetParameter("ShowMetalDetectorOverlayInExploreHub", FALSE, &g_ShowMetalDetectorOverlayInExploreHub);
	    parameter_block->GetParameter("ShowScavengerHuntOverlayInExploreHub", FALSE, &g_ShowScavengerHuntOverlayInExploreHub);
	    parameter_block->GetParameter("ScavengerHuntPartGroupInExploreHub", FALSE, &g_ScavengerHuntPartGroupInExploreHub);
	    parameter_block->GetParameter("CheckForXbox360TextureMipMaps", FALSE, &g_CheckForXbox360TextureMipMaps);
	    parameter_block->GetParameter("OnlyLoadXbox360LightMapsFromResourceFile", FALSE, &g_OnlyLoadXbox360LightMapsFromResourceFile);
    }

    if (game_mode == 1 && parameter_block->ReadParameterBlock("Arcade") != 0) {
	    parameter_block->GetParameter("NumberOfAI", 0, &number_of_ai);
	    parameter_block->GetParameter("AINames", "", ai_names, sizeof(ai_names));
    }

    Game::LoadConfigFile(parameter_block);
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
        snprintf(g_UILocalizedTextureContentDirectory, sizeof(g_UILocalizedTextureContentDirectory), "%s%s", g_LocalizedUIContentDirectory, "Tex_HD\\");
        snprintf(g_UITextureContentDirectory, sizeof(g_UITextureContentDirectory), "%s%s", g_BaseUIContentDirectory, "Tex_HD\\");
    }
    else {
        snprintf(g_UILocalizedTextureContentDirectory, sizeof(g_UILocalizedTextureContentDirectory), "%s%s", g_LocalizedUIContentDirectory, "Tex\\");
        snprintf(g_UITextureContentDirectory, sizeof(g_UITextureContentDirectory), "%s%s", g_BaseUIContentDirectory, "Tex\\");
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
    snprintf(texture_name, sizeof(texture_name), "%s%s", g_UILocalizedTextureContentDirectory, name);
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
    snprintf(splash_screen, sizeof(splash_screen), "load_logo");
    unused24 = 0;
    char_number = 0;
    strncpy(char_name, "McQ", 4);
    char_paint_type = 1;
    char_color1.x = 0.0f;
    char_color1.y = 0.0f;
    char_color1.z = 0.0f;
    char_color1.w = 1.0f;
    char_color2.x = 0.0f;
    char_color2.y = 0.0f;
    char_color2.z = 0.0f;
    char_color2.w = 1.0f;
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
        snprintf(loading_screen_name, sizeof(loading_screen_name), "%s", _loading_screen);
    }
    if (_loading_icon == nullptr) {
        loading_icon_name[0] = '\0';
        return 1;
    }
    CreateLoadingIcon(_loading_icon);
    snprintf(loading_icon_name, sizeof(loading_screen_name), "%s", _loading_icon);
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
