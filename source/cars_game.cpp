#include "cars_game.hpp"
#include "data_access.hpp"
#include "globals.hpp"
#include "gfx/renderer.hpp"
#include "gfx/x360_tex_map.hpp"
#include "gfx/x360_render_target.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_full_screen_texture_render.hpp"
#include "util/rsstring_util.hpp"
#include "util/resource_setup.hpp"

// In the original game, this just gets leaked. For now, we'll use std::unique_ptr for portability.
std::unique_ptr<CarsGame> lpCarsGame = std::make_unique<CarsGame>();

char g_UILocalizedTextureContentDirectory[260] = "E\\UI\\Tex\\";
char g_LocalizedUIContentDirectory[260] = "E\\UI\\";
char g_UITextureContentDirectory[260] = "C\\UI\\Tex\\";
char g_BaseUIContentDirectory[260] = "C\\UI\\";
char g_LocalizationContentDirectory[260] = "E\\Loc\\";
char g_FontTextureContentDirectory[260] = "E\\Font\\";
char g_AudioDialogueDirectory[260] = "E\\Audio\\";
char g_DebugDirectory[260] = "C\\Debug\\";

int g_GetActivityTypeFromActivityFile = FALSE;
int g_FixMcqueensHeadquartersStage = FALSE;
int g_McqueensHeadquartersStage = TRUE;
int g_StartOnTitleScreenOnFirstBoot = TRUE;
int g_Boost_Lvl = 1;
float g_SuspensionLoadType = 1.0f;
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

// OFFSET: 0x0043faf0, STATUS: COMPLETE
CarsGame::CarsGame() : X360Game() {
    for (std::size_t i = 0; i < sizeof(cheat_codes) / sizeof(CheatCode); i++) {
        cheat_codes[i].index = -1;
        cheat_codes[i].name[0] = 0;
        cheat_codes[i].active = 0;
    }
    profile = nullptr;
    brake_when_resting_threshold = 5.0;
    hall_of_fame = nullptr;
    personal_records = nullptr;
    audio_manager = nullptr;
    world = nullptr;
    ui = nullptr;
    video_fx_manager = nullptr;
    settings = nullptr;
    bolt_manager = nullptr;
    pickup_manager = nullptr;
    scavenger_hunt_manager = nullptr;
    bonus_point_manager = nullptr;
    achievement_manager = nullptr;
    scene_database = nullptr;
    activity_database = nullptr;
    event_database = nullptr;
    vehicle_database = nullptr;
    story_manager = nullptr;
    event_join_point_manager = nullptr;
    ui_resource_manager = nullptr;
    bonus_content_manager = nullptr;
    scene[0] = 0;
    scene_name[0] = 0;
    use_vehicle_engine_sound = 0;
    play_music = 0;
    brake_when_resting = 0;
    screen_text_font_manager = nullptr;
    char_number = 0;
    char_name[0] = 0;
    char_paint_type = 0;
    char_color1 = Vector4(0, 0, 0, 1);
    char_color2 = Vector4(0, 0, 0, 1);
    vehicle_manual_transmission = 0;
    unused24 = nullptr;
    total_laps = 3;
    no_321 = 0;
    build_spline_mode = 0;
    unused28 = 0;
    analog_gas = 0;
    show_coords = 1;
    dump_results = 0;
    unused29 = 0;
    world_paused = 0;
    unlock_all = 0;
    unlock_all_abilities = 0;
    unused30 = 0;
    enforce_free_camera_speed = 1;
    unused31 = 0;
    cinema_mode = 0;
    loop_cutscenes = 0;
    loading_tip_preview = 0;
    unused25[0] = 0;
    splash_screen[0] = '\0';
    RSStringUtil::Ssnprintf(splash_screen, sizeof(splash_screen), "load_logo");
    loading_icon = nullptr;
    loading_screen_name[0] = 0;
    loading_icon_name[0] = 0;
    unlock_all_bonus_content = 0;
    cheat_codes[0].index = 0;
    RSStringUtil::Ssnprintf(cheat_codes[0].name, sizeof(CheatCode::name), "MATTEL07");
    cheat_codes[1].index = 1;
    RSStringUtil::Ssnprintf(cheat_codes[1].name, sizeof(CheatCode::name), "PAINTIT");
    cheat_codes[2].index = 2;
    RSStringUtil::Ssnprintf(cheat_codes[2].name, sizeof(CheatCode::name), "NCEDUDZ");
    cheat_codes[3].index = 3;
    RSStringUtil::Ssnprintf(cheat_codes[3].name, sizeof(CheatCode::name), "INSTYLE");
    cheat_codes[4].index = 4;
    RSStringUtil::Ssnprintf(cheat_codes[4].name, sizeof(CheatCode::name), "PLAYALL");
    cheat_codes[5].index = 5;
    RSStringUtil::Ssnprintf(cheat_codes[5].name, sizeof(CheatCode::name), "SCENZ4U");
    cheat_codes[6].index = 6;
    RSStringUtil::Ssnprintf(cheat_codes[6].name, sizeof(CheatCode::name), "BUYTALL");
    cheat_codes[7].index = 7;
    RSStringUtil::Ssnprintf(cheat_codes[7].name, sizeof(CheatCode::name), "ALLYORS");
    cheat_codes[8].index = 8;
    RSStringUtil::Ssnprintf(cheat_codes[8].name, sizeof(CheatCode::name), "VRYFAST");
    cheat_codes[9].index = 9;
    RSStringUtil::Ssnprintf(cheat_codes[9].name, sizeof(CheatCode::name), "ZZOOOOM");
    cheat_codes[10].index = 10;
    RSStringUtil::Ssnprintf(cheat_codes[10].name, sizeof(CheatCode::name), "0TO200X");

    // This does basically nothing, since the return value from DataAccess::FileExists gets discarded.
    char dont_share_me[260]{};
    RSStringUtil::Ssnprintf(dont_share_me, sizeof(dont_share_me), "%sDontShareMe.txt", g_DebugDirectory);
    lpDataAccess->FileExists(dont_share_me);

    ai_names[0] = 0;
    difficulty = 0;
    time_of_day = 0;
    starting_bolt_banners = 0;
    starting_bonus_points = 0;
}

// OFFSET: 0x0051a420, STATUS: TODO
CarsGame::~CarsGame() {
    /*
    if (screen_text_font_manager != nullptr) {
        delete screen_text_font_manager;
        screen_text_font_manager = nullptr;
    }
    if (settings != nullptr) {
        delete settings;
        settings = nullptr;
    }
    if (bolt_manager != nullptr) {
        delete bolt_manager;
        bolt_manager = nullptr;
    }
    if (pickup_manager != nullptr) {
        delete pickup_manager;
        pickup_manager = nullptr;
    }
    if (scavenger_hunt_manager != nullptr) {
        delete scavenger_hunt_manager;
        scavenger_hunt_manager = nullptr;
    }
    if (bonus_point_manager != nullptr) {
        delete bonus_point_manager;
        bonus_point_manager = nullptr;
    }
    if (event_join_point_manager != nullptr) {
        delete event_join_point_manager;
        event_join_point_manager = nullptr;
    }
    if (ui_resource_manager != nullptr) {
        delete ui_resource_manager;
        ui_resource_manager = nullptr;
    }
    if (event_database != nullptr) {
        delete event_database;
        event_database = nullptr;
    }
    if (activity_database != nullptr) {
        delete activity_database;
        activity_database = nullptr;
    }
    if (scene_database != nullptr) {
        delete scene_database;
        scene_database = nullptr;
    }
    if (vehicle_database != nullptr) {
        delete vehicle_database;
        vehicle_database = nullptr;
    }
    if (story_manager != nullptr) {
        delete story_manager;
        story_manager = nullptr;
    }
    if (hall_of_fame != nullptr) {
        delete hall_of_fame;
        hall_of_fame = nullptr;
    }
    if (personal_records != nullptr) {
        delete personal_records;
        personal_records = nullptr;
    }
    if (g_lpVirtualNetwork != nullptr) {
        g_lpVirtualNetwork->Release();
        g_lpVirtualNetwork = nullptr;
    }
    */
}

// OFFSET: 0x004f50a0, STATUS: WIP
int CarsGame::InitializeRenderer(char (&quit_message)[260]) {
    SetLanguageToDefault();
    int renderer_initialized = X360Game::InitializeRenderer(quit_message);
    if (renderer_initialized == 0) {
        // The original game does not populate the quit message, but we do.
        RSStringUtil::Ssnprintf(quit_message, sizeof(quit_message), "Failed to initialize renderer!");
    }
    /*
    Activity::Registry->Create();
    Actor::Registry->Create();
    ActorInterface::Registry->Create();
    if (g_lpVirtualNetwork == nullptr) {
        CreateVirtualNetwork();
    }
    if (g_lpGlobalStreamManager == nullptr && g_lpGlobalCarsUIStreamManager == nullptr) {
        CreateStreamManager();
    }
    */
    g_VideoCard->initialized = 1;
    flags |= 32;
    MapAllGameKeys();
    /*
    screen_text_font_manager = new ScreenTextFontManager();
    screen_text_font_manager = screen_text_font_manager->Create(); // Does nothing but return itself
    */
    unk_game_state = 1;
    /*
    managers->AddChild(FlowChartEngine::GetInstance(), 2);
    managers->AddChild(TRCEngine::GetInstance(), 2);
    */
    return renderer_initialized;
}

// OFFSET: 0x0051c130, STATUS: WIP
int CarsGame::Initialize() {
    if (splash_screen[0] != '\0') {
        ShowLoadingScreen(splash_screen, "ld_mcq", 1);
    }

    char game_info_path[260];
    RSStringUtil::Ssnprintf(game_info_path, sizeof(game_info_path), "%s%sGameInfo.%s.res", "Res\\Data\\", "C\\Appstart\\", "x360");
    // FIXME: Until we implement RES file loading, we should keep any ResourceSetup stuff commented out.
    ResourceSetup::ResourceSetup(game_info_path, -1, 1, 8, 0, 0, -1, 1, 0);
    /*
    cars_settings = new CarsSettings();
    cars_settings->Create();
    */
    SetConfigArguments();
    return 1;
}

// OFFSET: 0x0043fe80, STATUS: WIP
int CarsGame::PreGameInitialize(DisplayMode*) {
    // FIXME: Not completely implemented
    char material_file_content[260]{};
    char debug_str_file[260]{};
    char material_template_file[260]{};

    X360Game::PreGameInitialize(nullptr);

    /*
    _timeGameInit = timeGetTime();
    */
    RSStringUtil::Ssnprintf(debug_str_file,sizeof(debug_str_file),"%sDebugStr.mst", g_DebugDirectory);
    // lpMaterialStringTable->Load(debug_str_file);
    RSStringUtil::Ssnprintf(material_template_file, sizeof(material_template_file), "%s%sMaterialTemplate.%s.res", "Res\\Data\\", "C\\Appstart\\", "x360");

    if (g_ResBuilding == 0) {
        // FIXME: Until we implement RES file loading, we should keep any ResourceSetup stuff commented out.
        ResourceSetup::ResourceSetup(material_template_file,0xffffffff,1,8,0,0,-1,1,0);
    }

    RSStringUtil::Ssnprintf(material_file_content, sizeof(material_file_content), "%sDfltMT", "C\\AppStart\\");
    lpSceneObjectMaterialTemplate->LoadFromFile(material_file_content);
    RSStringUtil::Ssnprintf(material_file_content, sizeof(material_file_content), "%sIconMT", "C\\AppStart\\");
    lpIconMaterialTemplate->LoadFromFile(material_file_content);
	
    if (g_ResBuilding == 0) {
        // FIXME: Until we implement RES file loading, we should keep any ResourceSetup stuff commented out.
        ResourceSetup::ResourceFinish(material_template_file,1);
    }
    
    return 1;
}

// OFFSET: 0x00440360, STATUS: COMPLETE
void CarsGame::LoadConfigFile(ParameterBlock* parameter_block) {
    char game_mode_parameter[64]{};
    char difficulty_parameter[64]{};
    char story_type_parameter[64]{};
    char time_of_day_parameter[64]{};
    char char_paint_type_parameter[260]{};
    constexpr auto empty_vector = Vector4(0, 0, 0, 0);
    char viewport_type_parameter[260]{};
    char control_type_parameter[260]{};
    char camera_type_parameter[260]{};
    char language_parameter[64]{};
    int build_resource_files_parameter = 0;
    int build_ini_files_parameter = 0;

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

	    parameter_block->GetParameter("SuspensionLoadType", 1.0f, &g_SuspensionLoadType);
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

// OFFSET: 0x00440e00, STATUS: TODO
void CarsGame::BeginAutoTest() {
}

// OFFSET: 0x004412b0, STATUS: TODO
void CarsGame::EndAutoTest(int) {
}

// OFFSET: 0x004f51d0, STATUS: TODO
int CarsGame::Tick() {
    return 0;
}

// OFFSET: 0x00440320, STATUS: TODO
int CarsGame::KeyUpHandler(ProcessNode*, KeyInfo*) {
    return 0;
}

// OFFSET: 0x00440340, STATUS: TODO
int CarsGame::KeyDownHandler(ProcessNode*, KeyInfo*) {
    return 0;
}

// OFFSET: 0x0051c730, STATUS: TODO
int CarsGame::Terminate() {
    return 0;
}

// OFFSET: 0x00441b70, STATUS: TODO
void CarsGame::ReportMemoryUsage(char*) {
}

// OFFSET: 0x00487b80, STATUS: COMPLETE
void CarsGame::UpdateLocalizedPaths() {
    char* prefix = language_prefixes[lang_defs[selected_language_configuration->text_language_index]->prefix_index];
    g_LocalizationContentDirectory[0] = *prefix;
    g_FontTextureContentDirectory[0] = *prefix;
    g_LocalizedUIContentDirectory[0] = *prefix;
    g_AudioDialogueDirectory[0] = *prefix;
    UpdateTextureContentDirectories();
}

// OFFSET: 0x00441d80, STATUS: COMPLETE
void CarsGame::UpdateTextureContentDirectories() {
    if (g_ScreenMode == 2) {
        RSStringUtil::Ssnprintf(g_UILocalizedTextureContentDirectory, sizeof(g_UILocalizedTextureContentDirectory), "%s%s", g_LocalizedUIContentDirectory, "Tex_HD\\");
        RSStringUtil::Ssnprintf(g_UITextureContentDirectory, sizeof(g_UITextureContentDirectory), "%s%s", g_BaseUIContentDirectory, "Tex_HD\\");
    }
    else {
        RSStringUtil::Ssnprintf(g_UILocalizedTextureContentDirectory, sizeof(g_UILocalizedTextureContentDirectory), "%s%s", g_LocalizedUIContentDirectory, "Tex\\");
        RSStringUtil::Ssnprintf(g_UITextureContentDirectory, sizeof(g_UITextureContentDirectory), "%s%s", g_BaseUIContentDirectory, "Tex\\");
    }
}

// OFFSET: 0x00441840, STATUS: COMPLETE
void CarsGame::ReloadMaterials() {
    Game::ReloadMaterials(0);
}

// OFFSET: 0x00441850, STATUS: COMPLETE
void CarsGame::ReloadMaterialsAndTextures() {
    Game::ReloadMaterialsAndTextures(0);
}

// OFFSET: 0x0043fa70, STATUS: TODO
void CarsGame::AddVisibilitySearchList(ContainerList<GameObject*>*) {
}

// OFFSET: 0x00441cb0, STATUS: COMPLETE
void CarsGame::ClearCheatCodes() {
    for (std::size_t i = 0; i < sizeof(cheat_codes) / sizeof(CheatCode); i++) {
        cheat_codes[i].active = 0;
    }
}

// OFFSET: 0x00487ac0, STATUS: TODO
void CarsGame::CreateActivityDatabase() {
}

// OFFSET: 0x004873c0, STATUS: TODO
void CarsGame::CreateActivityManager() {
}

// OFFSET: 0x00487870, STATUS: TODO
void CarsGame::CreateBoltManager(int) {
}

// OFFSET: 0x004c3b70, STATUS: TODO
void CarsGame::CreateBonusContentManager() {
}

// OFFSET: 0x004c3a10, STATUS: TODO
void CarsGame::CreateBonusPointManager() {
}

// OFFSET: 0x004c3c40, STATUS: TODO
void CarsGame::CreateControllerSettings() {
}

// OFFSET: 0x00487b20, STATUS: TODO
void CarsGame::CreateEventDatabase() {
}

// OFFSET: 0x00441470, STATUS: TODO
void CarsGame::CreateLoadingIcon(const char* name) {
    if (loading_icon == nullptr) {
        loading_icon = new X360LoadingIcon();
    }
    char icon_lsi[260]{};
    RSStringUtil::Ssnprintf(icon_lsi, sizeof(icon_lsi), "%s%s", g_UILocalizedTextureContentDirectory, name);
    loading_icon->Disable();
    loading_icon->Create(icon_lsi);
    loading_icon->Enable();
}

// OFFSET: 0x00441370, STATUS: WIP
void CarsGame::CreateLoadingScreen(const char* name) {

    char package_name[260]{};
    RSStringUtil::Ssnprintf(package_name, sizeof(package_name), "%s%s%s.%s.res", "Res\\Data\\", g_UILocalizedTextureContentDirectory, name, "x360");
    // FIXME: Until we implement RES file loading, we should keep any ResourceSetup stuff commented out.
    ResourceSetup::ResourceSetup(package_name,-1,1,8,0,0,-1,1,0);

    char texture_name[260]{};
    RSStringUtil::Ssnprintf(texture_name, sizeof(texture_name), "%s%s", g_UILocalizedTextureContentDirectory, name);
    TextureMap* texture_map = X360TexMap::GetTextureMapFromResourceName(texture_name, 555, 0);
    if (texture_map != nullptr) {
        int index = Renderer::PrepareNextRenderFrame();
        Renderer::PrepareNextRenderDrawList(index);
        g_FullscreenRender.SetTexture(0, texture_map);
        Renderer::g_RenderFrameData[index].fullscreen_render_pass = static_cast<X360FullScreenRenderPass*>(&g_FullscreenRender);
        Renderer::CompleteNextRenderFrame();
        texture_map->Release();
    }
    // FIXME: Until we implement RES file loading, we should keep any ResourceSetup stuff commented out.
    ResourceSetup::ResourceFinish(package_name,1);
}

// OFFSET: 0x00487990, STATUS: TODO
void CarsGame::CreatePickupManager() {
}

// OFFSET: 0x0051c030, STATUS: TODO
void CarsGame::CreateProfileManager() {
}

// OFFSET: 0x004c3960, STATUS: TODO
void CarsGame::CreateRecordLibraries() {
}

// OFFSET: 0x004f54e0, STATUS: TODO
void CarsGame::CreateScavengerHuntManager() {
}

// OFFSET: 0x004f5560, STATUS: TODO
void CarsGame::CreateStoryManager() {
}

// OFFSET: 0x004c3aa0, STATUS: TODO
void CarsGame::CreateStreamManager() {
}

// OFFSET: 0x004877d0, STATUS: TODO
void CarsGame::CreateUI() {
}

// OFFSET: 0x004c3bd0, STATUS: TODO
void CarsGame::CreateUIResourceManager() {
}

// OFFSET: 0x004419e0, STATUS: TODO
void CarsGame::CreateVideoFXManager(int) {
}

// OFFSET: 0x00441970, STATUS: TODO
void CarsGame::CreateVirtualNetwork() {
}

// OFFSET: 0x00422c80, STATUS: TODO
void CarsGame::UNK_00422c80(int render_frame_index, int camera_index) {
    g_RenderTarget->SetViewport(&Renderer::g_Viewports[camera_index]);
    // g_RenderTarget->used_camera = &g_Cameras[camera_index];
    g_RenderTarget->unk_filter_index = 0;
    if (Renderer::g_RenderFrameData[render_frame_index].should_unk != 0) {
        if (g_RenderTarget->used_camera->enable_perspective == 0) {
            int should_draw_cursor = SetBasicRenderStates();
            g_RenderTarget->ApplyViewportImpl(1, 1, 1, 1);
            if (should_draw_cursor != 0) {
                // SurfaceShaderList::FUN_0040cd00(lpSurfaceShaderList, camera_index, 0);
                g_RenderTarget->DrawCursorImpl();
            }
        }
        else {
            if (g_VideoCard->IsFXEnabled()) {
                SetBasicRenderStates();
                int should_end = g_RenderTarget->DrawFullscreenEffects();
                if (should_end != 0) {
                    // SurfaceShaderList::FUN_0040cd00(lpSurfaceShaderList, camera_index, 1);
                    g_RenderTarget->EndFrame();
                }
            }
        }
    }
    g_RenderTarget->Clear();
    unused11 += g_RenderTarget->unk_increment;
    unk_game_state = 0;
    return;
}

// OFFSET: 0x00487470, STATUS: TODO
void CarsGame::UNK_00487470() {
}

// OFFSET: 0x0050b890, STATUS: TODO
void CarsGame::LoadWorld() {
}

// OFFSET: 0x0043ffc0, STATUS: TODO
void CarsGame::MapAllGameKeys() {
}

// OFFSET: 0x00441910, STATUS: TODO
void CarsGame::PrepareDeferredLoad(DeferredLoad) {
}

// OFFSET: 0x004237a0, STATUS: COMPLETE
void CarsGame::PresentFrame(int draw_fullscreen_render_pass) {
    constexpr auto render_frame_index = 0;
    if (Renderer::g_RenderFrameData[render_frame_index].fullscreen_render_pass == nullptr) {
        int next_camera_index = Renderer::g_RenderFrameData[render_frame_index].next_camera;
        if (Renderer::g_RenderFrameData[render_frame_index].next_camera == 0) {
            g_VideoCard->DisplayToScreen(1);
            return;
        }
        for (int i = 0; i < Renderer::g_RenderFrameData[render_frame_index].next_camera; i++) {
            UNK_00422c80(render_frame_index, Renderer::g_RenderFrameData[render_frame_index].camera_indices[i]);
        }
    }
    else {
        D3DVIEWPORT9 fullscreen_viewport{
            .X = 0,
            .Y = 0,
            .Width = g_ViewportWidth,
            .Height = g_ViewportHeight,
            .MinZ = 0.0f,
            .MaxZ = 1.0f,
        };
        g_RenderTarget->SetViewport(&fullscreen_viewport);
        g_RenderTarget->ApplyViewportImpl(1, 1, 1, draw_fullscreen_render_pass);
        if (draw_fullscreen_render_pass != 0) {
            Renderer::g_RenderFrameData->fullscreen_render_pass->Draw(0);
            g_RenderTarget->DrawCursor();
        }
    }
    g_VideoCard->DisplayToScreen(0);
}

// OFFSET: 0x00441b40, STATUS: TODO
bool CarsGame::ReleaseVideoFXManager() {
    /*
    if (video_fx_manager != nullptr) {
        video_fx_manager->Release();
        video_fx_manager = nullptr;
    }
    */
    return true;
}

// OFFSET: 0x00441660, STATUS: TODO
int CarsGame::ResetLoadingScreenAndIcon() {
    return 0;
}

// OFFSET: 0x004416b0, STATUS: COMPLETE
void CarsGame::SetConfigArguments() {
    strncpy(activity, "FrontEndUI", 11);
    start[0] = '\0';
    scene[0] = '\0';
    scene_name[0] = '\0';
    RSStringUtil::Ssnprintf(splash_screen, sizeof(splash_screen), "load_logo");
    unused26 = 0;
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

// OFFSET: 0x00440c90, STATUS: COMPLETE
void CarsGame::SetLanguageToDefault() {
    XboxLanguage selected_dash_lang = XGetLanguage();
    if (selected_dash_lang == XboxLanguage::None) {
        XboxLocale selected_locale = XGetLocale();
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

// OFFSET: 0x00441570, STATUS: COMPLETE
int CarsGame::ShowLoadingScreen(const char* _loading_screen, const char* _loading_icon, int is_first) {
    if (is_first == 0 && _loading_icon != nullptr && _loading_screen != nullptr) {
        if (_stricmp(_loading_screen, loading_screen_name) == 0 && _stricmp(_loading_icon, loading_icon_name) == 0) {
            return 1;
        }
    }
    if (loading_icon != nullptr) {
        loading_icon->Disable();
        delete loading_icon;
        loading_icon = nullptr;
    }
    if (_loading_screen == nullptr) {
        loading_screen_name[0] = '\0';
    }
    else {
        CreateLoadingScreen(_loading_screen);
        RSStringUtil::Ssnprintf(loading_screen_name, sizeof(loading_screen_name), "%s", _loading_screen);
    }
    if (_loading_icon == nullptr) {
        loading_icon_name[0] = '\0';
        return 1;
    }
    CreateLoadingIcon(_loading_icon);
    RSStringUtil::Ssnprintf(loading_icon_name, sizeof(loading_screen_name), "%s", _loading_icon);
    return 1;
}

// OFFSET: 0x00422db0, STATUS: TODO
void CarsGame::UnkRegistryGetValue(char*, unsigned int) {
}

// OFFSET: 0x00441870, STATUS: TODO
void CarsGame::UnloadStringTables() {
}

// OFFSET: 0x00487700, STATUS: TODO
void CarsGame::UnloadWorld() {
}
