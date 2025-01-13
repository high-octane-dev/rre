#include "cars_game.hpp"
#include <globals.hpp>

// Since I'm not sure how this gets freed in the original game, I'm just going to use a std::unique_ptr.
std::unique_ptr<CarsGame> lpCarsGame = std::make_unique<CarsGame>();

// OFFSET: 0x004f50a0
int CarsGame::InitializeRenderer(char* message)
{
    SetLanguageToDefault();
    if (X360Game::InitializeRenderer(message) == 0) {
        snprintf(message, 260, "Failed to initialize renderer!");
        return 0;
    }
    unk_game_state = 1;
    return 1;
}

// OFFSET: 0x0051c130
int CarsGame::Initialize()
{
    if (splash_screen[0] != '\0')
    {
        ShowLoadingScreen(splash_screen, "ld_mcq", 1);
    }

    SetConfigArguments();
    return 0;
}

// OFFSET: 0x0043fe80
int CarsGame::PreGameInitialize(DisplayMode*)
{
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
void CarsGame::LoadConfigFile(ParameterBlock*)
{
}

// OFFSET: 0x00440e00
void CarsGame::BeginAutoTest()
{
}

// OFFSET: 0x004412b0
void CarsGame::EndAutoTest(int)
{
}

// OFFSET: 0x004f51d0
int CarsGame::Tick()
{
    return 0;
}

// OFFSET: 0x00440320
int CarsGame::KeyUpHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

// OFFSET: 0x00440340
int CarsGame::KeyDownHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

// OFFSET: 0x0051c730
int CarsGame::Terminate()
{
    return 0;
}

// OFFSET: 0x00441b70
void CarsGame::ReportMemoryUsage(char*)
{
}

// OFFSET: 0x00487b80
void CarsGame::UpdateLocalizedPaths()
{
}

// OFFSET: 0x00441d80
void CarsGame::UpdateTextureContentDirectories()
{
}

// OFFSET: 0x00441840
void CarsGame::ReloadMaterials()
{
    Game::ReloadMaterials(0);
}

// OFFSET: 0x00441850
void CarsGame::ReloadMaterialsAndTextures()
{
    Game::ReloadMaterialsAndTextures(0);
}

// OFFSET: 0x004237a0
void CarsGame::PresentFrame(int)
{
}

void CarsGame::SetConfigArguments()
{
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

int CarsGame::ShowLoadingScreen(char* a1, const char* a2, int a3)
{
    if ((((a3 == 0) && (a1 != NULL)) && (_stricmp(a1, loading_screen_name) == 0)) && ((a2 != NULL && (_stricmp(a2, loading_icon_name) == 0))))
    {
        return 1;
    }
    if (a2 == NULL)
    {
        loading_icon_name[0] = '\0';
        return 1;
    }
    snprintf(loading_icon_name, 260, "%s", a2);
    return 1;
}
