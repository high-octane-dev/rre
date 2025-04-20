#include "ui_page.hpp"

// OFFSET: 0x00625de0, STATUS: COMPLETE
UIPage::UIPage() : focus_managers() {
	name = 0;
	layer_count = 0;
	layer_name_indices = nullptr;
	class_name_indices = nullptr;
	should_be_enabled = 0;
	enabled = 0;
	has_registered_event_listeners = 0;
	is_top_of_stack = 0;
	has_top_enabled = 3;
	has_not_top_enabled = 3;
}

// OFFSET: 0x00625e20, STATUS: COMPLETE
UIPage::~UIPage() {
	if (layer_name_indices != nullptr) delete[] layer_name_indices;
	if (class_name_indices != nullptr) delete[] class_name_indices;
	if (name != nullptr) delete[] name;
	for (auto& elem : focus_managers) {
		delete elem;
	}
}

// OFFSET: 0x006279f0, STATUS: TODO
void UIPage::CreateFromFile(char*, ParameterBlock*) {
}

// OFFSET: 0x00625ed0, STATUS: TODO
void UIPage::CreateFromName(char*) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIPage::Initialize() {
}

// OFFSET: 0x006239e0, STATUS: TODO
void UIPage::Render() {
}

// OFFSET: 0x00623a40, STATUS: TODO
void UIPage::Tick(float dt) {
}

// OFFSET: 0x00625fa0, STATUS: TODO
void UIPage::StartPage() {
}

// OFFSET: 0x00626100, STATUS: TODO
void UIPage::EndPage() {
}

// OFFSET: 0x00623aa0, STATUS: TODO
void UIPage::RegisterEventListeners() {
}

// OFFSET: 0x00623b00, STATUS: TODO
void UIPage::DeRegisterEventListeners() {
}

// OFFSET: 0x00625f40, STATUS: TODO
void UIPage::RegisterFocusManagers() {
}

// OFFSET: 0x00625f70, STATUS: TODO
void UIPage::DeRegisterFocusManagers() {
}

// OFFSET: 0x004288b0, STATUS: COMPLETE
void UIPage::SetEnabled(int _enabled) {
	enabled = _enabled;
}

// OFFSET: 0x00623b60, STATUS: TODO
void UIPage::SetPageStackState(PageStackState) {
}

// OFFSET: 0x00623ca0, STATUS: TODO
void UIPage::Repaint() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIPage::ReportToDebugOverlay(int, DebugOverlay*) {
}

// OFFSET: 0x00623bd0, STATUS: TODO
void UIPage::PageStackStateChanged() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIPage::Start() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIPage::End() {
}