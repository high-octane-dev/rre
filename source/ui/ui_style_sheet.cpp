#include "ui_style_sheet.hpp"

// OFFSET: 0x006100c0, STATUS: COMPLETE
void UIStyleSheet::InitializeLists() {
	color_styles.CLNonMacroCreate(20, 16, std::numeric_limits<std::int32_t>::max());
	font_styles.CLNonMacroCreate(20, 16, std::numeric_limits<std::int32_t>::max());
	text_styles.CLNonMacroCreate(20, 16, std::numeric_limits<std::int32_t>::max());
}

// OFFSET: 0x0060ff00, STATUS: COMPLETE
UIStyleSheet::UIStyleSheet() : color_styles(), font_styles(), text_styles() {
}

// OFFSET: 0x0060ff50, STATUS: TODO
UIStyleSheet::~UIStyleSheet() {
}

// OFFSET: 0x00611270, STATUS: TODO
void UIStyleSheet::CreateFromFile(char*) {
}

// OFFSET: 0x00610cc0, STATUS: TODO
void UIStyleSheet::AddColorStyle(char*, unsigned int) {
}

// OFFSET: 0x00610d50, STATUS: TODO
void UIStyleSheet::AddFontStyle(char*, char*, char*) {
}

// OFFSET: 0x00610df0, STATUS: TODO
void UIStyleSheet::AddTextStyle(char*, char*) {
}

// OFFSET: 0x00610140, STATUS: TODO
void* UIStyleSheet::GetColorStyle(char*) {
	return nullptr;
}

// OFFSET: 0x00610190, STATUS: TODO
void* UIStyleSheet::GetFontStyle(char*) {
	return nullptr;
}

// OFFSET: 0x006101e0, STATUS: TODO
void* UIStyleSheet::GetTextStyle(char*) {
	return nullptr;
}
