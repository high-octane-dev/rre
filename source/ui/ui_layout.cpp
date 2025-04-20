#include "ui_layout.hpp"

// OFFSET: 0x00610340, STATUS: COMPLETE
UILayout::~UILayout() {
	for (auto& component_layout : component_layouts) {
		delete component_layout;
	}
}

// OFFSET: 0x006103f0, STATUS: COMPLETE
void UILayout::Create() {
	component_layouts.CLNonMacroCreate(16, 16, (std::numeric_limits<int32_t>::max)());
}

// OFFSET: 0x00610420, STATUS: COMPLETE
UIComponentLayout* UILayout::GetComponentLayout(char* name) {
	for (const auto& layout : component_layouts) {
		if (_stricmp(name, layout->name) == 0) {
			return layout;
		}
	}
	return nullptr;
}
