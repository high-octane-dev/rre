#pragma once
#include "virtual_network.hpp"
#include "cars_ui_menu_page.hpp"
#include "containers/container_linked_list.hpp"

class CarsUIPagesPopup : public CarsUIMenuPage {
public:
    int field1_0xb4;
};

enum class MenuState {
    Invalid = -1,
    Activity_Instructions = 0,
    Activity_Payout = 1,
    Activity_Results = 2,
    Activity_Records = 3,
    Activity_Rewards = 4,
    Activity_SwitchEvent = 5,
    Activity_TryAgainDlg = 6,
    Activity_SaveGamePopup = 7,
    Activity_ProgressMenu = 8,
    Activity_ArcadeEventOverMenu = 9,
    ArcadeEventOverMenu = 10,
    VideoLogoScreen = 11,
    LegalScreen = 12,
    TitleScreen = 13,
    InitialBootProfileMenu = 14,
    MainMenu = 15,
    OptionMenu = 16,
    GameOptionMenu = 17,
    CenterScreenMenu = 18,
    SoundOptionMenu = 19,
    CameraOptionMenu = 20,
    ControllerSettingsMenu = 21,
    ChooseRecordsMenu = 22,
    ViewRecordsMenu = 23,
    CheatCodeMenu = 24,
    CreditsScreen = 25,
    ProfileMenu = 26,
    ManageProfileMenu = 27,
    EnterNameMenu = 28,
    EventSelectMenu = 29,
    ActivitySelectMenu = 30,
    CharacterSelectMenu = 31,
    RelayRaceTeamSelectMenu = 32,
    DifficultyMenu = 33,
    DifficultyMenu2 = 34,
    StoryModeInstructionsMenu = 35,
    StoryModeDrivingControlsMenu = 36,
    BonusContentMenu = 37,
    CharacterSelectMenu2 = 38,
    BonusContentItemMenu = 39,
    BonusContentArtMenu = 40,
    BonusContentItemMenu2 = 41,
    PauseMenu = 42,
    PauseMenuOptions = 43,
    PauseProgressMenu = 44,
    PauseMenuSoundOptions = 45,
    PauseGameOptions = 46,
    EventJointPointSelectEventMenu = 47,
    EventJointPointSelectActivityMenu = 48,
    StoryModeMap = 49,
    Tutorial = 50,
    LastLaughHUD = 51,
    LastLaughHUDVS = 52,
    CutsceneOverlay = 53,
    Unknown = 54,
};

class CarsPopupListener {
public:
    int unk;
public:
    virtual void Unk1(int);
    virtual void Unk2(int, int);
};

class FlowChartDoneListener {
    virtual void FlowChartDone(int);
};

class CarsUIMenuManager : public VNObject, public CarsPopupListener, public FlowChartDoneListener {
public:
    int unused;
    MenuState previous;
    MenuState current;
    int unk_network_payload;
    CarsUIMenuPage* current_page;
    CarsUIMenuPage* previous_page;
    ContainerLinkedList<UIPage*> page_linked_list;
    int user_index;
    int unk_current_vno_event_data;
    int unk_total_vno_event_data;
    int tutorial_exited;
    int popup_type;
    MenuState popup_parent_menu;
    struct X360SaveProfileFlowChart* save_profile_flow_chart;
    MenuState flow_chart_parent_menu;
    int finished_saving;
    int exit_game_popup_active;
    int unknown1;
public:
    void HandleOk();
    void GoBack(int);
    int SetGenericMenu(MenuState, int);
    void SetMenu(const char*, MenuState, int);
    int ExitTutorialLayer();
    int FUN_004a45e0();
    void FUN_004dfe70(const char*, int);
    void ShowPopup(const char*, const char*, const char*, const char*, CarsPopupListener*, int);
};

#ifdef _M_IX86
static_assert(sizeof(CarsUIMenuManager) == 0x68);
#endif