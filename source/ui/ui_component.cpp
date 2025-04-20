#include <algorithm>
#include "ui_container.hpp"
#include "ui_component.hpp"
#include "ui_layer.hpp"
#include "parameter_block.hpp"


// OFFSET: 0x0060ef90, STATUS: COMPLETE
void UIComponentLayout::Create(ParameterBlock* file) {
	int read_param = 0;
	visible = 0;
	if (file->GetParameter("Visible", 1, &read_param) != 0) {
		visible = 2 - static_cast<unsigned int>(read_param != 0);
	}
	enabled = 0;
	if (file->GetParameter("Enabled", 1, &read_param) != 0) {
		enabled = 2 - static_cast<unsigned int>(read_param != 0);
	}
	focusable = 0;
	if (file->GetParameter("Focusable", 1, &read_param) != 0) {
		focusable = 2 - static_cast<unsigned int>(read_param != 0);
	}
	focused = 0;
	if (file->GetParameter("Focused", 1, &read_param) != 0) {
		focused = 2 - static_cast<unsigned int>(read_param != 0);
	}
	selectable = 0;
	if (file->GetParameter("Selectable", 1, &read_param) != 0) {
		selectable = 2 - static_cast<unsigned int>(read_param != 0);
	}
	selected = 0;
	if (file->GetParameter("Selected", 1, &read_param) != 0) {
		selected = 2 - static_cast<unsigned int>(read_param != 0);
	}
	char apply_condition_name[260];
	file->GetParameter("Apply", "always", apply_condition_name, sizeof(apply_condition_name));
	if (_stricmp(apply_condition_name, "never") == 0) {
		condition = ApplyCondition::Never;
	}
	else {
		if (_stricmp(apply_condition_name, "once") == 0) {
			condition = ApplyCondition::Once;
		}
		else {
			if (_stricmp(apply_condition_name, "always") == 0) {
				condition = ApplyCondition::Always;
			}
		}
	}
}

// OFFSET: 0x006231a0, STATUS: COMPLETE
UIComponent::UIComponent() {
	parent_container = nullptr;
	D3DXMatrixIdentity(&transform);
	name = nullptr;
	visible = 1;
	enabled = 1;
	focusable = 1;
	focused = 0;
	selectable = 1;
	selected = 0;
	parent_mouse_menu = nullptr;
}

// OFFSET: INLINE, STATUS: COMPLETE
UIComponent::~UIComponent() {
	free(name);
	parent_mouse_menu = nullptr;
}

// OFFSET: 0x00623220, STATUS: COMPLETE
void UIComponent::Create(char* new_name) {
	std::size_t len = strlen(new_name);
	name = reinterpret_cast<char*>(malloc(len + 1));
	if (name != nullptr) {
		memcpy(name, new_name, len);
		name[len] = 0;
	}
	D3DXMatrixIdentity(&transform);
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::StartComponent() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::EndComponent() {
}

// OFFSET: INLINE, STATUS: COMPLETE
int UIComponent::IsContainer() {
	return 0;
}

// OFFSET: 0x006234d0, STATUS: COMPLETE
UIComponent* UIComponent::GetComponent(char* component) {
	if (_stricmp(name, component) != 0) {
		return nullptr;
	}
	return this;
}

// OFFSET: 0x004573e0, STATUS: COMPLETE
void UIComponent::GetDimensions(float* x, float* y) {
	*x = 0;
	*y = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::SetDimensions(float* x, float* y) {}

// OFFSET: 0x00457400, STATUS: COMPLETE
void UIComponent::GetExtents(_RECT* extents) {
	extents->left = 0;
	extents->top = 0;
	extents->right = 0;
	extents->bottom = 0;
}

// OFFSET: 0x006235b0, STATUS: COMPLETE
void UIComponent::GetExtentsRelative(_RECT* extents) {
	this->GetExtents(extents);
	if (parent_container != nullptr) {
		float parent_x = 0.0f;
		float parent_y = 0.0f;
		parent_container->GetAbsolutePosition(&parent_x, &parent_y);
		extents->left -= static_cast<int>(parent_x);
		extents->right -= static_cast<int>(parent_x);
		extents->top -= static_cast<int>(parent_y);
		extents->bottom -= static_cast<int>(parent_y);
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
float UIComponent::GetWidth() {
	return 0.0f;
}

// OFFSET: INLINE, STATUS: COMPLETE
float UIComponent::GetHeight() {
	return 0.0f;
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::GetPosition(float* x, float* y) {
	*x = 0;
	*y = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::SetPosition(float x, float y) {}

// OFFSET: 0x006232a0, STATUS: COMPLETE
void UIComponent::GetAbsolutePosition(float* x, float* y) {
	this->GetPosition(x, y);
	if (parent_container != nullptr) {
		float parent_x = 0.0f;
		float parent_y = 0.0f;
		parent_container->GetAbsolutePosition(&parent_x, &parent_y);
		*x += parent_x;
		*y += parent_y;
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::SetAngle(float angle) {
}

// OFFSET: 0x00623510, STATUS: COMPLETE
void UIComponent::PropogateChange() {
	if (parent_container != nullptr) {
		parent_container->PropogateChange();
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::Repaint() {
}

// OFFSET: 0x00457250, STATUS: COMPLETE
Matrix4x4* UIComponent::GetTransform() {
	return &transform;
}

// OFFSET: 0x00623620, STATUS: COMPLETE
Matrix4x4* UIComponent::GetScreenTransform(Matrix4x4* dest) {
	if (parent_container == nullptr) {
		*dest = transform;
		return dest;
	}
	else {
		Matrix4x4 parent_transform{};
		parent_container->GetScreenTransform(&parent_transform);
		D3DXMatrixMultiply(dest, &parent_transform, &transform);
		return dest;
	}
}

// OFFSET: 0x00623470, STATUS: COMPLETE
void UIComponent::SetTransform(Matrix4x4* mat) {
	transform = *mat;
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::UpdateTransform(Matrix4x4* mat) {
}

// OFFSET: 0x006232f0, STATUS: COMPLETE
void UIComponent::ApplyLayout(UIComponentLayout* layout) {
	if (layout->visible != 0) {
		this->SetVisible(layout->visible != 2);
	}
	if (layout->enabled != 0) {
		this->SetEnabled(layout->enabled != 2);
	}
	if (layout->focusable != 0) {
		this->SetFocusable(layout->focusable != 2);
	}
	if (layout->selectable != 0) {
		this->SetSelectable(layout->selectable != 2);
	}
	if (layout->focused != 0) {
		this->SetFocused(layout->focused != 2);
	}
	if (layout->selected != 0) {
		this->SetSelected(layout->selected != 2);
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
std::uint32_t UIComponent::GetColor() {
	return 0xffffffff;
}

// OFFSET: 0x006233a0, STATUS: COMPLETE
std::uint32_t UIComponent::GetAlpha() {
	return this->GetColor() >> 24;
}

// OFFSET: 0x006233b0, STATUS: COMPLETE
void UIComponent::SetAlpha(int alpha) {
	alpha = std::clamp(alpha, 0, 0xFF);
	std::uint32_t color = this->GetColor();
	this->SetColor(color & 0xffffff | alpha << 0x18);
}

// OFFSET: 0x00623400, STATUS: COMPLETE
void UIComponent::ScaleAlpha(float scale) {
	scale = std::clamp(scale, 0.0f, 1.0f);
	std::uint32_t alpha = static_cast<uint32_t>(this->GetAlpha() * scale);
	this->SetAlpha(alpha);
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::SetColor(std::uint32_t color) {
}

// OFFSET: 0x00457260, STATUS: COMPLETE
void UIComponent::GetObjectColorScale(Vector4* object_color_scale) {
	object_color_scale->x = 1.0;
	object_color_scale->y = 1.0;
	object_color_scale->z = 1.0;
	object_color_scale->w = 1.0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::SetObjectColorScale(Vector4 object_color_scale) {
}

// OFFSET: 0x00457420, STATUS: COMPLETE
void UIComponent::SetEnabled(int _enabled) {
	enabled = _enabled;
}

// OFFSET: 0x006234f0, STATUS: COMPLETE
void UIComponent::SetVisible(int _visible) {
	visible = _visible;
	if (_visible != 0) {
		Repaint();
	}
}

// OFFSET: 0x00457290, STATUS: COMPLETE
void UIComponent::SetFocusable(int _focusable) {
	focusable = _focusable;
}

// OFFSET: 0x004572a0, STATUS: COMPLETE
void UIComponent::SetFocused(UIComponent* other) {
	if (focusable != 0) {
		this->SetFocused(1);
		if (other != nullptr && other != this) {
			other->SetFocused(0);
		}
	}
}

// OFFSET: 0x00457370, STATUS: COMPLETE
void UIComponent::SetFocused(int _focused) {
	if (focusable != 0) {
		focused = _focused;
	}
}

// OFFSET: 0x004572e0, STATUS: COMPLETE
void UIComponent::SetSelectable(int _selectable) {
	selectable = _selectable;
}

// OFFSET: 0x004572f0, STATUS: COMPLETE
void UIComponent::SetSelected(UIComponent* other) {
	if (selectable != 0) {
		this->SetSelected(1);
		if (other != nullptr && other != this) {
			other->SetSelected(0);
		}
	}
}

// OFFSET: 0x00427630, STATUS: COMPLETE
void UIComponent::SetSelected(int _selected) {
	if (selectable != 0) {
		selected = _selected;
		this->Repaint();
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::RegisterEventListeners() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::DeRegisterEventListeners() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::SetScale(float scale) {
}

// OFFSET: 0x00623530, STATUS: TODO
UILayer* UIComponent::GetLayer() {
	return nullptr;
}

// OFFSET: 0x00623590, STATUS: WIP
UIStyleSheet* UIComponent::GetStyleSheet() {
	UILayer* layer = GetLayer();
	if (layer != nullptr) {
		if (layer->style_sheet != nullptr) {
			return layer->style_sheet;
		}
		// return lpUIManager->style_sheet;
	}
	return nullptr;
}

// OFFSET: 0x006277c0, STATUS: COMPLETE
void UIComponent::CursorClicked(std::uint32_t u_msg, int x, int y) {
	if (u_msg == WM_RBUTTONDOWN) {
		if (parent_mouse_menu != nullptr) {
			parent_mouse_menu->Callback(this, 1, x, y);
		}
	}
	else if (u_msg != WM_MBUTTONDOWN && parent_mouse_menu != nullptr) {
		parent_mouse_menu->Callback(this, 0, x, y);
	}
}

// OFFSET: 0x00627830, STATUS: COMPLETE
void UIComponent::CursorSelected(int x, int y) {
	if (parent_mouse_menu != nullptr) {
		parent_mouse_menu->Callback(this, 2, x, y);
	}
}

// OFFSET: 0x00625050, STATUS: TODO
uintptr_t UIComponent::FUN_00625050() {
	return 0;
}

// OFFSET: 0x00625100, STATUS: COMPLETE
void UIComponent::ResetMouseMenu() {
	parent_mouse_menu = nullptr;
}

// OFFSET: 0x00625110, STATUS: COMPLETE
void UIComponent::SetMouseMenu(MouseMenu* menu) {
	this->ResetMouseMenu();
	parent_mouse_menu = menu;
}

// OFFSET: 0x00625170, STATUS: COMPLETE
bool UIComponent::FUN_00625170() {
	return true;
}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::DoTick(float dt) {}

// OFFSET: INLINE, STATUS: COMPLETE
void UIComponent::DoRender(int) {}

// OFFSET: 0x006234b0, STATUS: COMPLETE
void UIComponent::Render(int _unk) {
	if (visible != 0) {
		DoRender(_unk);
	}
}
