#include "cars_ui_menu_manager.hpp"
#include "cars_game.hpp"
#include "cars_ui.hpp"

// The original implementation of this function does not play the intended sound effects, so our version will more closely match the Xbox 360 version to fix this.
/*
void CarsUIMenuManager::HandleOk() {
    Cars2Launcher* launcher = Cars2Launcher::GetInstance();

	switch (current) {
	case MenuState::Activity_Instructions:
		if (lpCarsGame->settings->game_mode != 0) {
			unk_network_payload = 0;
			GoBack(-1);
			lpVirtualNetwork->UpdateVNObjectEvent(this, 6, 0x26, &unk_network_payload, 0);
			return;
		}
		if (SetGenericMenu(Activity_Payout, -1) == 0) {
			unk_network_payload = 0;
			GoBack(-1);
			lpVirtualNetwork->UpdateVNObjectEvent(this, 6, 0x26, &unk_network_payload, 0);
			return;
		}
		break;
	case MenuState::Activity_Payout:
		unk_network_payload = current_page->GetAdvancedOrRetracted();
		GoBack(-1);
		lpVirtualNetwork->UpdateVNObjectEvent(this, 6, 0x26, &unk_network_payload, 0);
		return;
	case MenuState::Activity_Results:
        if (lpCarsGame->settings->game_mode == 0) {
            if ((ExitTutorialLayer() == 0) && (FUN_004a45e0() == 0)) {
                if (SetGenericMenu(Activity_Rewards, -1) == 0) {
                    SetGenericMenu(Activity_ProgressMenu, -1);
                    return;
                }
                break;
            }
        }
        else {
            if ((ExitTutorialLayer() == 0) && (FUN_004a45e0() == 0)) {
                SetMenu("Arcade Event Over Menu", ArcadeEventOverMenu, -1);
                return;
            }
        }
        break;
    case MenuState::Activity_Records:
        if (lpCarsGame->settings->game_mode != 0) {
            SetMenu("Arcade Event Over Menu", ArcadeEventOverMenu, -1);
            return;
        }
        if (SetGenericMenu(Activity_Rewards, -1) == 0) {
            SetGenericMenu(Activity_ProgressMenu, -1);
            return;
        }
        break;
    case MenuState::Activity_Rewards:
        SetGenericMenu(Activity_ProgressMenu, -1);
        return;
    case MenuState::Activity_SwitchEvent:
    case MenuState::Activity_SaveGamePopup:
    case MenuState::LastLaughHUD:
    case MenuState::LastLaughHUDVS:
    case MenuState::Unknown:
        break;
    case MenuState::Activity_TryAgainDlg:
        unk_network_payload = current_page->GetAdvancedOrRetracted();
        if (current_page->GetAdvancedOrRetracted() == 6) {
            current_page->deferred_load = 1;
            GoBack(-1);
            return;
        }
        launcher->incoming_unk_type = UnkType::FrontEndUI;
        launcher->activity_type = ActivityType::Invalid;
        launcher->SetActivity("FrontEndUI");
        current_page->deferred_load = 0;
        GoBack(-1);
        return;
    case MenuState::Activity_ProgressMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (reinterpret_cast<CarsUIPagesPopup*>(current_page)->field1_0xb4 == 0) {
                GameText* in_game_ui = lpCarsUI->manager->GetGameText("InGameUI");
                ShowPopup(in_game_ui->GetString("STR_RETRY_LEVEL"), in_game_ui->GetString("STR_RETRY_CONFIRM"), in_game_ui->GetString("STR_YES"), in_game_ui->GetString("STR_NO"), this, 1);
                popup_type = 0;
                popup_parent_menu = Activity_ProgressMenu;
                return;
            }
            if (reinterpret_cast<CarsUIPagesPopup*>(current_page)->field1_0xb4 == 1) {
                GameText* in_game_ui = lpCarsUI->manager->GetGameText("TRC");
                ShowPopup(in_game_ui->GetString("STR_NEW_TITLE_SWITCH_EVENT"), in_game_ui->GetString("STR_NEW_MESSAGE_SWITCH_EVENT"), in_game_ui->GetString("STR_YES"), in_game_ui->GetString("STR_NO"), this, 1);
                popup_type = 1;
                popup_parent_menu = Activity_ProgressMenu;
                return;
            }
            if (reinterpret_cast<CarsUIPagesPopup*>(current_page)->field1_0xb4 == 2 && lpCarsGame->settings->game_mode == 0) {
                *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(save_profile_flow_chart) + 0x34) = 2;
                auto func = reinterpret_cast<void(**__thiscall)(void*)>((*reinterpret_cast<std::uintptr_t*>(save_profile_flow_chart)) + 9 * sizeof(void*));
                (*func)(save_profile_flow_chart);
                finished_saving = 1;
                flow_chart_parent_menu = Activity_ProgressMenu;
                return;
            }
        }
        else if (current_page->GetAdvancedOrRetracted() == 10) {
            FUN_004dfe70("ProgressPopup", 9);
            return;
        }
        break;
    case MenuState::Activity_ArcadeEventOverMenu:
        GoBack(-1);
        return;
    case MenuState::ArcadeEventOverMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (_stricmp(current_page->GetSelectedOption(), "Retry") == 0) {
                current_page->deferred_load = 1;
                GoBack(-1);
                return;
            }
            if (_stricmp(current_page->GetSelectedOption(), "SelectLevel") == 0) {
                CarsUIArcadeEventOverMenu* arcade_menu = reinterpret_cast<CarsUIArcadeEventOverMenu*>(lpCarsUI->manager->GetLayer("ArcadeMenu"));
                Cars2EventInfo* event = arcade_menu->GetSelectedEvent();
                launcher->incoming_unk_type = (lpCarsGame->settings->game_mode != 1) ? UnkType::VersusRace : UnkType::Race;
                launcher->activity_type = event->activity_info->type;
                launcher->SetActivity(event->activity_info->directory);
                current_page->deferred_load = 0;
                GoBack(-1);
                return;
            }
            if (_stricmp(current_page->GetSelectedOption(), "Quit") == 0) {
                GameText* trc = lpCarsUI->manager->GetGameText("TRC");
                *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(save_profile_flow_chart) + 0x34) = 2;
                auto func = reinterpret_cast<void(**__thiscall)(void*)>((*reinterpret_cast<std::uintptr_t*>(save_profile_flow_chart)) + 9 * sizeof(void*));
                (*func)(save_profile_flow_chart);
                finished_saving = 1;
                flow_chart_parent_menu = ArcadeEventOverMenu;
                return;
            }
        }
        break;
    case MenuState::VideoLogoScreen:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("LegalScreen", MenuState::LegalScreen, 2);
            return;
        }
        break;
    case MenuState::LegalScreen:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("TitleScreen", MenuState::TitleScreen, 2);
        }
        // WTFRB: Is this intentntional? Pressing enter on load_legal taking you straight to profile select seems wrong.
    case MenuState::TitleScreen:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("InitialBootProfileMenu", MenuState::InitialBootProfileMenu, 2);
            return;
        }
        break;
    case MenuState::InitialBootProfileMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("ManageProfileMenu", MenuState::ManageProfileMenu, 3);
            return;
        }
        break;
    case MenuState::MainMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (_stricmp("Arcade Mode", current_page->GetSelectedOption()) == 0) {
                launcher->incoming_unk_type = UnkType::Race;
                launcher->player_count = 1;
                SetMenu("EventSelectMenu", EventSelectMenu, -1);
                return;
            }
            if (_stricmp("Versus", current_page->GetSelectedOption()) == 0) {
                launcher->incoming_unk_type = UnkType::VersusRace;
                launcher->player_count = 2;
                SetMenu("EventSelectMenu", EventSelectMenu, -1);
                return;
            }
            if (_stricmp("Options", current_page->GetSelectedOption()) == 0) {
                SetMenu("OptionMenu", OptionMenu, -1);
                return;
            }
            if (_stricmp("Profile", current_page->GetSelectedOption()) == 0) {
                SetMenu("ProfileMenu", ProfileMenu, -1);
                return;
            }
            if (_stricmp("Story Mode", current_page->GetSelectedOption()) == 0) {
                Cars2StoryEvent* event = lpGlobalStoryManager->GetStoryDatabase()->GetStoryEvent("StoryEvent_CutScene1");
                if (event != nullptr) {
                    launcher->incoming_unk_type = UnkType::StoryMode;
                    launcher->player_count = 1;
                    if (event->status == Cars2StoryEvent::StoryEventStatusType::CutsceneFinished) {
                        current_page->Unk_ExitMenu(nullptr, 0);
                        unk_network_payload = 0;
                        current_page = nullptr;
                        current = Invalid;
                        launcher->LaunchActivity();
                        return;
                    }
                    SetMenu("DifficultyMenu", DifficultyMenu2, -1);
                    return;
                }
            }
            else {
                if (_stricmp("BonusContent", current_page->GetSelectedOption()) == 0) {
                    SetMenu("BonusContentMenu", BonusContentMenu, -1);
                    return;
                }
                if (_stricmp("Quit Game", current_page->GetSelectedOption()) == 0) {
                    GameText* pc_front_end_ui = lpCarsUI->manager->GetGameText("TRC");
                    ShowPopup(pc_front_end_ui->GetString("STR_TITLE_EXITGAME"), pc_front_end_ui->GetString("STR_MESSAGE_EXITGAME"), pc_front_end_ui->GetString("STR_YES"), pc_front_end_ui->GetString("STR_NO"), this, 1);
                    exit_game_popup_active = 1;
                    popup_type = 4;
                    return;
                }
            }
        }
        break;
    case MenuState::OptionMenu:
        if (current_page->GetAdvancedOrRetracted() != 1) {
            return;
        }
        if (_stricmp("Game Options", current_page->GetSelectedOption()) == 0) {
            SetMenu("GameOptionMenu", GameOptionMenu, -1);
            return;
        }
        if (_stricmp("Sound Options", current_page->GetSelectedOption()) == 0) {
            SetMenu("SoundOptionMenu", SoundOptionMenu, -1);
            return;
        }
        if (_stricmp("Camera Options", current_page->GetSelectedOption()) == 0) {
            SetMenu("CameraOptionMenu", CameraOptionMenu, -1);
            return;
        }
        if (_stricmp("launcherayer One Controller Settings", current_page->GetSelectedOption()) == 0) {
            *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetLayer("ControllerSettingsMenuLayer")) + 0x6c) = 0;
            SetMenu("ControllerSettingsMenu", ControllerSettingsMenu, -1);
            return;
        }
        if (_stricmp("launcherayer Two Controller Settings", current_page->GetSelectedOption()) == 0) {
            *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetLayer("ControllerSettingsMenuLayer")) + 0x6c) = 1;
            SetMenu("ControllerSettingsMenu", ControllerSettingsMenu, -1);
            return;
        }
        if (_stricmp("Records", current_page->GetSelectedOption()) == 0) {
            SetMenu("ChooseRecordsMenu", ChooseRecordsMenu, -1);
            return;
        }
        if (_stricmp("Cheat Codes", current_page->GetSelectedOption()) == 0) {
            SetMenu("CheatCodeMenu", CheatCodeMenu, -1);
            return;
        }
        if (_stricmp("Credits", current_page->GetSelectedOption()) == 0) {
            SetMenu("CreditsScreen", CreditsScreen, 2);
            return;
        }
        break;
    case MenuState::GameOptionMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("OptionMenu", OptionMenu, -1);
            return;
        }
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("OptionMenu", OptionMenu, -1);
            return;
        }
        break;
    case MenuState::CenterScreenMenu:
        if (current_page->GetAdvancedOrRetracted() < 3) {
            SetMenu("GameOptionMenu", GameOptionMenu, -1);
            return;
        }
        break;
    case MenuState::SoundOptionMenu:
        if (current_page->GetAdvancedOrRetracted() < 3) {
            SetMenu("OptionMenu", OptionMenu, -1);
            return;
        }
        break;
    case MenuState::CameraOptionMenu:
        if (current_page->GetAdvancedOrRetracted() < 3) {
            SetMenu("OptionMenu", OptionMenu, -1);
            return;
        }
        break;
    case MenuState::ControllerSettingsMenu:
    case MenuState::CheatCodeMenu:
        if (current_page->GetAdvancedOrRetracted() < 3) {
            SetMenu("OptionMenu", OptionMenu, -1);
            return;
        }
        break;
    case MenuState::ChooseRecordsMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("ViewRecordsMenu", ViewRecordsMenu, -1);
            return;
        }
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("OptionMenu", OptionMenu, -1);
            return;
        }
        break;
    case MenuState::ViewRecordsMenu:
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("ChooseRecordsMenu", ChooseRecordsMenu, -1);
            return;
        }
        break;
    case MenuState::CreditsScreen:
        if (current_page->GetAdvancedOrRetracted() > 0) {
            if (current_page->GetAdvancedOrRetracted() < 3) {
                SetMenu("OptionMenu", OptionMenu, 2);
                return;
            }
            if (current_page->GetAdvancedOrRetracted() == 3 && (lpCarsGame->settings->game_mode == 0)) {
                current_page->deferred_load = 3;
                GoBack(-1);
                return;
            }
        }
        break;
    case MenuState::ProfileMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("ManageProfileMenu", ManageProfileMenu, -1);
            return;
        }
        break;
    case MenuState::ManageProfileMenu:
        if (current_page->GetAdvancedOrRetracted() != 1) {
            if (current_page->GetAdvancedOrRetracted() == 2) {
                SetMenu("ProfileMenu", ProfileMenu, -1);
                return;
            }
            if (current_page->GetAdvancedOrRetracted() == 8) {
                SetMenu("InitialBootProfileMenu", InitialBootProfileMenu, 2);
                return;
            }
            break;
        }
        if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xc0) == 0 || *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xc0) == 3) {
            SetMenu("EnterNameMenu", EnterNameMenu, -1);
            return;
        }
        if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xc0) == 2 || *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xc0) == 4) {
            SetMenu("MainMenu", MainMenu, 3);
            return;
        }
        break;
    case MenuState::EnterNameMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("MainMenu", MainMenu, 3);
            return;
        }
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("ManageProfileMenu", ManageProfileMenu, -1);
            return;
        }
        if (current_page->GetAdvancedOrRetracted() == 8) {
            SetMenu("InitialBootProfileMenu", InitialBootProfileMenu, 2);
            return;
        }
        break;
    case MenuState::EventSelectMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("ActivitySelectMenu", ActivitySelectMenu, -1);
            return;
        }
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("MainMenu", MainMenu, -1);
            return;
        }
        break;
    case MenuState::ActivitySelectMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (launcher->activity_type != ActivityType::TractorTipping && !(launcher->incoming_unk_type == UnkType::VersusRace && (launcher->activity_type == ActivityType::RaceFuelFrenzy || launcher->activity_type == ActivityType::RaceAndChase || launcher->activity_type == ActivityType::LastLaugh))) {
                SetMenu("DifficultyMenu", DifficultyMenu, -1);
                return;
            }
            launcher->needs_character_select_screen = 0;
            SetMenu("CharacterSelectMenu", CharacterSelectMenu, -1);
            return;
        }
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("EventSelectMenu", EventSelectMenu, -1);
            return;
        }
        break;
    case MenuState::CharacterSelectMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (launcher->incoming_unk_type == UnkType::Race) {
                current_page->Unk_ExitMenu(nullptr, 0);
                unk_network_payload = 0;
                current_page = nullptr;
                current = MenuState::Invalid;
                launcher->LaunchActivity();
                return;
            }
            if (launcher->incoming_unk_type == UnkType::VersusRace) {
                if (launcher->needs_character_select_screen != 0) {
                    current_page->Unk_ExitMenu(nullptr, 0);
                    unk_network_payload = 0;
                    current_page = nullptr;
                    current = MenuState::Invalid;
                    launcher->LaunchActivity();
                    return;
                }
                launcher->needs_character_select_screen = 1;
                SetMenu("CharacterSelectMenu", CharacterSelectMenu, -1);
                return;
            }
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            if (launcher->incoming_unk_type == UnkType::Race) {
                if (launcher->activity_type == ActivityType::TractorTipping) {
                    SetMenu("ActivitySelectMenu", ActivitySelectMenu, -1);
                    return;
                }
                SetMenu("DifficultyMenu", DifficultyMenu, -1);
                return;
            }
            if (launcher->incoming_unk_type == UnkType::VersusRace) {
                if (launcher->needs_character_select_screen == 0) {
                    if (launcher->activity_type == ActivityType::RaceFuelFrenzy || launcher->activity_type == ActivityType::RaceAndChase || launcher->activity_type == ActivityType::LastLaugh || launcher->activity_type == ActivityType::TractorTipping) {
                        SetMenu("ActivitySelectMenu", ActivitySelectMenu, -1);
                        return;
                    }
                    SetMenu("DifficultyMenu", DifficultyMenu, -1);
                    return;
                }
                launcher->needs_character_select_screen = 0;
                SetMenu("CharacterSelectMenu", CharacterSelectMenu, -1);
                return;
            }
        }
        else if (current_page->GetAdvancedOrRetracted() == 3 && lpCarsGame->settings->game_mode == 0) {
            current_page->deferred_load = 3;
            GoBack(-1);
            return;
        }
        break;
    case MenuState::RelayRaceTeamSelectMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (launcher->incoming_unk_type == UnkType::Race) {
                current_page->Unk_ExitMenu(nullptr, 0);
                unk_network_payload = 0;
                current_page = nullptr;
                current = MenuState::Invalid;
                launcher->LaunchActivity();
            }
            else if (launcher->incoming_unk_type == UnkType::VersusRace) {
                if (launcher->needs_character_select_screen != 0) {
                    current_page->Unk_ExitMenu(nullptr, 0);
                    unk_network_payload = 0;
                    current_page = nullptr;
                    current = MenuState::Invalid;
                    launcher->LaunchActivity();
                }
                else {
                    launcher->needs_character_select_screen = 1;
                    SetMenu("RelayRaceTeamSelectMenu", RelayRaceTeamSelectMenu, -1);
                }
            }
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            if (launcher->incoming_unk_type == UnkType::Race) {
                SetMenu("DifficultyMenu", DifficultyMenu, -1);
            }
            else if (launcher->incoming_unk_type == UnkType::VersusRace) {
                if (launcher->needs_character_select_screen != 0) {
                    launcher->needs_character_select_screen = 0;
                    SetMenu("RelayRaceTeamSelectMenu", RelayRaceTeamSelectMenu, -1);
                }
                else {
                    SetMenu("DifficultyMenu", DifficultyMenu, -1);
                }
            }
        }
        break;
    case MenuState::DifficultyMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            launcher->needs_character_select_screen = 0;
            if (launcher->incoming_unk_type == UnkType::Race) {
                if (launcher->activity_type == ActivityType::RaceRelay) {
                    SetMenu("RelayRaceTeamSelectMenu", RelayRaceTeamSelectMenu, -1);
                }
                else if ((launcher->activity_type == ActivityType::GhostingMater) || (launcher->activity_type == ActivityType::RythmicRumble)) {
                    current_page->Unk_ExitMenu(nullptr, 0);
                    current = Invalid;
                    unk_network_payload = 0;
                    current_page = (CarsUIMenuPage*)0x0;
                    launcher->LaunchActivity();
                }
                else {
                    SetMenu("CharacterSelectMenu", CharacterSelectMenu, -1);
                }
            }
            if (launcher->incoming_unk_type == UnkType::VersusRace) {
                if (launcher->activity_type != ActivityType::RaceRelay) {
                    SetMenu("CharacterSelectMenu", CharacterSelectMenu, -1);
                }
                else {
                    SetMenu("RelayRaceTeamSelectMenu", RelayRaceTeamSelectMenu, -1);
                }
            }
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("ActivitySelectMenu", ActivitySelectMenu, -1);
        }
        break;
    case MenuState::DifficultyMenu2:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("StoryModeInstructionsMenu", StoryModeInstructionsMenu, -1);
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("MainMenu", MainMenu, -1);
        }
        break;
    case MenuState::StoryModeInstructionsMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            current_page->Unk_ExitMenu(nullptr, 0);
            current = Invalid;
            unk_network_payload = 0;
            current_page = nullptr;
            launcher->LaunchActivity();
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("DifficultyMenu", DifficultyMenu2, -1);
        }
        else if (current_page->GetAdvancedOrRetracted() == 0xb) {
            SetMenu("StoryModeDrivingControlsMenu", StoryModeDrivingControlsMenu, -1);
        }
        break;
    case MenuState::StoryModeDrivingControlsMenu:
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("StoryModeInstructionsMenu", StoryModeInstructionsMenu, -1);
        }
        break;
    case MenuState::BonusContentMenu:
        if (current_page->GetAdvancedOrRetracted() != 1) {
            if (current_page->GetAdvancedOrRetracted() == 2) {
                SetMenu("MainMenu", MainMenu, -1);
            }
            break;
        }
        if (_stricmp("BonusCharacter", current_page->GetSelectedOption()) == 0) {
            std::uintptr_t page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("CharacterSelectMenu"));
            *reinterpret_cast<std::uint32_t*>(page + 0xec) = 1;
            launcher->needs_character_select_screen = 0;
            SetMenu("CharacterSelectMenu", CharacterSelectMenu2, -1);
        }
        else if (_stricmp("BonusArt", current_page->GetSelectedOption()) == 0) {
            SetMenu("BonusContentArtMenu", BonusContentArtMenu, -1);
        }
        else if (_stricmp("BonusMovie", current_page->GetSelectedOption()) == 0) {
            std::uintptr_t page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("BonusContentItemMenu"));
            *reinterpret_cast<std::uint32_t*>(page + 0xcc) = 2;
            SetMenu("BonusContentItemMenu", BonusContentItemMenu2, -1);
        }
        break;
    case MenuState::CharacterSelectMenu2:
        if (current_page->GetAdvancedOrRetracted() == 2) {
            std::uintptr_t page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("CharacterSelectMenu"));
            *reinterpret_cast<std::uint32_t*>(page + 0xec) = 0;
            SetMenu("BonusContentMenu", BonusContentMenu, -1);
        }
        break;
    case MenuState::BonusContentItemMenu:
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("BonusContentArtMenu", BonusContentArtMenu, -1);
        }
        break;
    case MenuState::BonusContentArtMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            std::uintptr_t item_page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("BonusContentItemMenu"));
            std::uintptr_t art_page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("BonusContentArtMenu"));
            *reinterpret_cast<std::uint32_t*>(item_page + 0xcc) = 1;
            *reinterpret_cast<std::uint32_t*>(item_page + 0xd0) = *(unsigned int*)(*(int*)(*(int*)(art_page + 0x78) + 0x5c) + 0x134);
            SetMenu("BonusContentItemMenu", BonusContentItemMenu, -1);
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("BonusContentMenu", BonusContentMenu, -1);
        }
        break;
    case MenuState::BonusContentItemMenu2:
        if (current_page->GetAdvancedOrRetracted() == 2) {
            SetMenu("BonusContentMenu", BonusContentMenu, -1);
        }
        break;
    case MenuState::PauseMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xBC) == 0) {
                GoBack(-1);
                user_index = -1;
            }
            else if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xBC) == 1) {
                GameText* in_game_ui = lpCarsUI->manager->GetGameText("InGameUI");
                ShowPopup(in_game_ui->GetString("STR_RETRY_LEVEL"), in_game_ui->GetString("STR_RETRY_CONFIRM"), in_game_ui->GetString("STR_YES"), in_game_ui->GetString("STR_NO"), this, 1);
                popup_type = 0;
                popup_parent_menu = PauseMenu;
            }
            else if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xBC) == 2) {
                GameText* trc = lpCarsUI->manager->GetGameText("TRC");
                ShowPopup(trc->GetString("STR_NEW_TITLE_SWITCH_EVENT"), trc->GetString("STR_NEW_MESSAGE_SWITCH_EVENT"), trc->GetString("STR_NEW_YES"), trc->GetString("STR_NEW_NO"), this, 1);
                popup_type = 1;
                popup_parent_menu = PauseMenu;
            }
            else if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xBC) == 3) {
                SetMenu("PauseMenuOptions", PauseMenuOptions, -1);
            }
            else if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xBC) == 4) {
                GameText* in_game_ui = lpCarsUI->manager->GetGameText("InGameUI");
                ShowPopup(in_game_ui->GetString("STR_EXIT_EVENT"), in_game_ui->GetString("STR_QUIT_CONFIRM"), in_game_ui->GetString("STR_YES"), in_game_ui->GetString("STR_NO"), this, 1);
                popup_type = 2;
                popup_parent_menu = PauseMenu;
            }
            else if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xBC) == 5) {
                SetMenu("PauseProgressMenu", PauseProgressMenu, -1);
            }
        }
        break;
    case MenuState::PauseMenuOptions:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (_stricmp(current_page->GetSelectedOption(), "SoundOptions") == 0) {
                SetMenu("PauseMenuSoundOptions", PauseMenuSoundOptions, -1);
            }
            else if (_stricmp(current_page->GetSelectedOption(), "GameOptions") == 0) {
                SetMenu("PauseGameOptions", PauseGameOptions, -1);
            }
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            ShowPauseMenu();
        }
        break;
    case MenuState::PauseProgressMenu:
    case MenuState::PauseMenuSoundOptions:
    case MenuState::PauseGameOptions:
        ShowPauseMenu();
        break;
    case MenuState::EventJointPointSelectEventMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            SetMenu("EventJointPointSelectActivityMenu", EventJointPointSelectActivityMenu, -1);
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            std::uintptr_t page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("EventJointPointSelectEventMenu"));
            *reinterpret_cast<std::uint32_t*>(page + 0xb4) = 1;
            GoBack(-1);
        }
        break;
    case MenuState::EventJointPointSelectActivityMenu:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            std::uintptr_t page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("EventJointPointSelectActivityMenu"));
            Cars2EventInfo* iVar3 = FUN_00473f00(*(void**)(*(int*)(*(int*)(page + 0xb4) + 0x88) + *(int*)(*(int*)(page + 0xb4) + 0x98) * 4));
            launcher->incoming_unk_type = UnkType::Unk;
            launcher->activity_type = iVar3->activity_info->type;
            launcher->SetActivity(iVar3->activity_info->directory);
            strncpy(lpGlobalStoryManager->GetCurrentExploreHub()->current_event, launcher->current_event_in_explore_hub, sizeof(Cars2StoryManager::ExploreHub::current_event));
            current_page->deferred_load = 0;
            *reinterpret_cast<std::uint32_t*>(page + 0xbc) = 1;
            GoBack(-1);
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            std::uintptr_t page = reinterpret_cast<std::uintptr_t>(lpCarsUI->manager->GetPage("EventJointPointSelectActivityMenu"));
            if (*reinterpret_cast<std::uint32_t*>(page + 0xb8) != 0) {
                *reinterpret_cast<std::uint32_t*>(page + 0xbc) = 1;
                GoBack(-1);
                *reinterpret_cast<std::uint32_t*>(page + 0xb8) = 0;
            }
            else {
                SetMenu("EventJointPointSelectEventMenu", EventJointPointSelectEventMenu, -1);
                *reinterpret_cast<std::uint32_t*>(page + 0xb8) = 0;
            }
        }
        break;
    case MenuState::StoryModeMap:
        int iVar6 = 0;
        if (current_page->GetAdvancedOrRetracted() == 1) {
            if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xb4) == 0) {
            LAB_005133ce:
                iVar6 = -1;
            }
            else {
                if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xb4) == 1) {
                    CarsUIStoryModeMapLayer* story_mode_layer = reinterpret_cast<CarsUIStoryModeMapLayer*>(lpCarsUI->manager->GetLayer("StoryModeMap"));
                    char* chosen_activity = story_mode_layer->GetChosenActivity();
                    if (chosen_activity != nullptr) {
                        reinterpret_cast<CarsActivityExplore*>(lpCarsGame->activity_manager->activity)->JumpToActivity(chosen_activity);
                    }
                    else {
                        GoBack(-1);
                        break;
                    }
                }
                else if (*reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(current_page) + 0xb4) == 2) {
                    CarsUIStoryModeMapLayer* story_mode_layer = reinterpret_cast<CarsUIStoryModeMapLayer*>(lpCarsUI->manager->GetLayer("StoryModeMap"));
                    if (story_mode_layer->event_info == nullptr) {
                        GoBack(-1);
                        break;
                    }
                    launcher->incoming_unk_type = UnkType::Unk;
                    launcher->activity_type = story_mode_layer->event_info->activity_info->type;
                    launcher->SetActivity(story_mode_layer->event_info->activity_info->directory);
                    current_page->deferred_load = 0;
                    if ((story_mode_layer->field_0x78 != '\0') && (story_mode_layer->field_0xb8 != '\0')) {
                        pcVar16 = CarsUICommon::MAKE_STRING("StoryEvent_Explore_%s", &iVar4->field_0xb8);
                        lpGlobalStoryManager->SetCurrentExploreHub();
                        strncpy(lpGlobalStoryManager->GetCurrentExploreHub()->current_event, &iVar4->field_0x78, sizeof(Cars2StoryManager::ExploreHub::current_event));
                    }
                    GoBack(2);
                    break;
                }
                else {
                    break;
                }

            }
            GoBack(iVar6);
        }
        else if (current_page->GetAdvancedOrRetracted() == 2) {
            GoBack(-1);
        }
        break;
    case MenuState::Tutorial:
        if (FUN_004a45e0() == 0) {
            if (lpCarsGame->settings->game_mode != 0) {
                SetMenu("Arcade Event Over Menu", ArcadeEventOverMenu, 0);
            }
            else if (SetGenericMenu(Activity_Rewards, 0) == 0) {
                SetGenericMenu(Activity_ProgressMenu, 0);
            }
        }
        break;
    case MenuState::CutsceneOverlay:
        if (current_page->GetAdvancedOrRetracted() == 1) {
            GoBack(-1);
        }
        break;
    default:
        return;
	}

    int sound_event = 0;
    if (current_page != nullptr) {
        switch (current_page->GetAdvancedOrRetracted()) {
        case 1:
        case 10:
        case 11:
            if (current_page->is_not_top_of_stack != 0) {
                return;
            }
            sound_event = 4;
            break;
        case 2:
        case 8:
            if (current_page->is_not_top_of_stack != 0) {
                return;
            }
            sound_event = 3;
            break;
        default:
            return;
        }
        current_page->is_not_top_of_stack = 1;
        lpCarsUI->manager->PlaySoundEvent(sound_event, 1, 1);
    }
}
*/