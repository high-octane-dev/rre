#include "ui_layer.hpp"

// OFFSET: 0x00625bb0, STATUS: COMPLETE
UILayer::UILayer() : components() {
	container = nullptr;
	name = nullptr;
	style_sheet = nullptr;
	layer_unk = 0;
	layer_unk2 = 0x10;
}

// OFFSET: 0x00625be0, STATUS: COMPLETE
UILayer::~UILayer() {
	for (auto& component : components) {
		component->ResetMouseMenu();
	}
	for (auto& component : components) {
		delete component;
	}

	delete style_sheet;
	delete[] name;
}

// OFFSET: 0x00623860, STATUS: WIP
void UILayer::CreateFromFile(char* _name, ParameterBlock* file) {
	CreateFromName(_name);
	file->ReadParameterBlock("General");

	char style_sheet_name[80];
	if (file->GetParameter("StyleSheet", "*none*", style_sheet_name, sizeof(style_sheet_name)) != 0) {
		style_sheet = new UIStyleSheet();
		style_sheet->CreateFromFile(style_sheet_name);
	}
	// lpUIManager->LoadComponent("Root", file, this);
}

// OFFSET: 0x00625ca0, STATUS: COMPLETE
void UILayer::CreateFromName(char* _name) {
	std::size_t len = strlen(_name);
	name = new char[len + 1]();
	if (name != nullptr) {
		memcpy(name, _name, len);
		name[len] = 0;
	}
	components.CLNonMacroCreate(16, 16, (std::numeric_limits<std::int32_t>::max)());
}

// OFFSET: 0x00623950, STATUS: COMPLETE
void UILayer::Tick(float dt) {
	if (container->enabled != 0) {
		container->DoTick(dt);
	}
}

// OFFSET: 0x00623970, STATUS: COMPLETE
void UILayer::Render(int _unk) {
	if (container->enabled != 0) {
		container->DoRender(_unk);
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
void UILayer::StartLayer() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UILayer::EndLayer() {
}

// OFFSET: 0x00627990, STATUS: COMPLETE
void UILayer::StartComponents() {
	GetComponent("Root")->StartComponent();
}

// OFFSET: 0x006279b0, STATUS: COMPLETE
void UILayer::EndComponents() {
	GetComponent("Root")->EndComponent();
}

// OFFSET: 0x006239b0, STATUS: COMPLETE
void UILayer::RegisterEventListeners() {
	container->RegisterEventListeners();
}

// OFFSET: 0x006239c0, STATUS: COMPLETE
void UILayer::DeRegisterEventListeners() {
	container->DeRegisterEventListeners();
}

// OFFSET: INLINE, STATUS: COMPLETE
void UILayer::PageStackStateChanged(PageStackState) {
}

// OFFSET: 0x00623990, STATUS: COMPLETE
void UILayer::SetEnabled(int enabled) {
	container->SetEnabled(enabled);
}

// OFFSET: 0x00453c10, STATUS: COMPLETE
int UILayer::IsEnabled() {
	return container->enabled;
}

// OFFSET: 0x006239a0, STATUS: COMPLETE
void UILayer::SetVisible(int visible) {
	container->SetVisible(visible);
}

// OFFSET: 0x00453c20, STATUS: COMPLETE
int UILayer::IsVisible() {
	return container->visible;
}

// OFFSET: 0x00625d60, STATUS: COMPLETE
void UILayer::SetLayout(UILayout* _layout) {
	layer_unk2 = _layout->unk;
	for (auto& component : components) {
		UIComponentLayout* component_layout = _layout->GetComponentLayout(component->name);
		if (component_layout != nullptr) {
			if (component_layout->condition == UIComponentLayout::ApplyCondition::Once) {
				component->ApplyLayout(component_layout);
				component_layout->condition = UIComponentLayout::ApplyCondition::Never;
			}
			else if (component_layout->condition == UIComponentLayout::ApplyCondition::Always) {
				component->ApplyLayout(component_layout);
			}
		}
	}
}

// OFFSET: 0x004699c0, STATUS: COMPLETE
int UILayer::FUN_004699c0() {
	return 1;
}

// OFFSET: 0x006239d0, STATUS: COMPLETE
void UILayer::Repaint(int) {
	container->Repaint();
}

// OFFSET: 0x00625030, STATUS: COMPLETE
int UILayer::CursorClicked(std::uint32_t u_msg, int button, int x, int y) {
	return 3;
}

// OFFSET: 0x00625040, STATUS: COMPLETE
void UILayer::CursorSelected(std::uint32_t u_msg, int button, int x, int y) {
	return;
}

// OFFSET: 0x00625d10, STATUS: COMPLETE
UIComponent* UILayer::GetComponent(const char* _name) {
	for (const auto& component : components) {
		if (_stricmp(component->name, _name) == 0) {
			return component;
		}
	}
	return nullptr;
}
