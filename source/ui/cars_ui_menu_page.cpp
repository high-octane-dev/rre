#include "cars_ui_menu_page.hpp"

// OFFSET: 0x004ced20, STATUS: COMPLETE
CarsUIMenuPage::CarsUIMenuPage() : CarsUIPage(), UIInputEventListener() {
	title_text_id_index = -1;
	nav_text_id_indices[0] = -1;
	nav_text_id_indices[1] = -1;
	nav_text_id_indices[2] = -1;
	nav_text_id_indices[3] = -1;
	deferred_load = -1;
	pause_when_up = 0;
	pause_music_on_start = 0;
	visible_if_no_animation_present = 0;
	fade_tick_timer = 0;
	key_up = new UIInputEvent();
	key_down = new UIInputEvent();
	is_pages_popup = 0;
}

// OFFSET: 0x004540d0, STATUS: COMPLETE
CarsUIMenuPage::~CarsUIMenuPage() {
	if (key_down != nullptr) {
		delete key_down;
		key_down = nullptr;
	}
	if (key_up != nullptr) {
		delete key_up;
		key_up = nullptr;
	}
}

// OFFSET: 0x00496180, STATUS: TODO
void CarsUIMenuPage::CreateFromFile(char*, ParameterBlock*) {
}

// OFFSET: 0x00454190, STATUS: TODO
void CarsUIMenuPage::Render() {
}

// OFFSET: 0x00454200, STATUS: TODO
void CarsUIMenuPage::Tick(float dt) {
}

// OFFSET: 0x00496340, STATUS: TODO
void CarsUIMenuPage::StartPage() {
}

// OFFSET: 0x004cee60, STATUS: TODO
void CarsUIMenuPage::EndPage() {
}

// OFFSET: 0x00454470, STATUS: TODO
void CarsUIMenuPage::RegisterEventListeners() {
}

// OFFSET: 0x004544e0, STATUS: TODO
void CarsUIMenuPage::DeRegisterEventListeners() {
}

// OFFSET: 0x004542b0, STATUS: TODO
void CarsUIMenuPage::Unk_PlayAnim(int is_full_on_animation) {
}

// OFFSET: 0x00454300, STATUS: TODO
void CarsUIMenuPage::Unk_SetVisible() {
}

// OFFSET: 0x00454340, STATUS: TODO
void CarsUIMenuPage::Unk_EndLayer() {
}

// OFFSET: 0x00454370, STATUS: TODO
void CarsUIMenuPage::Unk_DisableMenu(int) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void CarsUIMenuPage::Unk_EnableMenu() {
}

// OFFSET: 0x004543d0, STATUS: TODO
void CarsUIMenuPage::Unk_ExitMenu(char*, int) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void CarsUIMenuPage::Unk2(int unk) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void CarsUIMenuPage::Unk3(int unk) {
}

// OFFSET: 0x004cee10, STATUS: TODO
void CarsUIMenuPage::Unk_UpdateText() {
}

// OFFSET: 0x00516840, STATUS: TODO
void CarsUIMenuPage::HandleOk(int _advanced_or_retracted) {
}

// OFFSET: 0x004545e0, STATUS: TODO
void CarsUIMenuPage::HandleBack(int _advanced_or_retracted) {
}

// OFFSET: 0x00463030, STATUS: TODO
void CarsUIMenuPage::ProcessKeydown(UIInputEvent*) {
}

// OFFSET: 0x004545b0, STATUS: TODO
void CarsUIMenuPage::ProcessKeyup(UIInputEvent*) {
}
