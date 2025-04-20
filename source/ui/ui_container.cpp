#include "ui_container.hpp"

// OFFSET: 0x0060f130, STATUS: COMPLETE
void UIContainerLayout::Create(ParameterBlock* file) {
	UIComponentLayout::Create(file);
	file->GetParameter("X", 0.0f, &x);
	file->GetParameter("Y", 0.0f, &y);
	file->GetParameter("Scale", 1.0f, &scale);
}

// OFFSET: INLINE, STATUS: COMPLETE
UIContainer::~UIContainer() {
}

// OFFSET: 0x006254a0, STATUS: COMPLETE
void UIContainer::StartComponent() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->StartComponent();
	}
}

// OFFSET: 0x006254d0, STATUS: COMPLETE
void UIContainer::EndComponent() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->EndComponent();
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
int UIContainer::IsContainer() {
	return 1;
}

// OFFSET: 0x00625500, STATUS: COMPLETE
UIComponent* UIContainer::GetComponent(char* component_name) {
	if (_stricmp(name, component_name) == 0) {
		return this;
	}
	for (std::size_t i = 0; i < components.Length(); i++) {
		UIComponent* component = components[i]->GetComponent(component_name);
		if (component != nullptr) {
			return component;
		}
	}
	return nullptr;
}

// OFFSET: 0x00457330, STATUS: COMPLETE
void UIContainer::GetDimensions(float* x, float* y) {
	_RECT extents{};
	this->GetExtents(&extents);
	*x = static_cast<float>(extents.right - extents.left);
	*y = static_cast<float>(extents.bottom - extents.top);
}

// OFFSET: 0x00625a80, STATUS: COMPLETE
void UIContainer::GetExtents(_RECT* extents) {
	bool has_visible_components = false;
	for (std::size_t i = 0; i < components.Length(); i++) {
		if (components[i]->visible) {
			_RECT child_extents{};
			components[i]->GetExtents(&child_extents);
			if (extents->top > child_extents.top) {
				extents->top = child_extents.top;
			}
			if (extents->bottom < child_extents.bottom) {
				extents->bottom = child_extents.bottom;
			}
			if (extents->left > child_extents.left) {
				extents->left = child_extents.left;
			}
			if (extents->right < child_extents.right) {
				extents->right = child_extents.right;
			}
			has_visible_components = true;
		}
	}
	if (!has_visible_components) {
		float x = 0.0f;
		float y = 0.0f;
		this->GetPosition(&x, &y);
		extents->right = static_cast<int>(x);
		extents->left = static_cast<int>(x);
		extents->bottom = static_cast<int>(y);
		extents->top = static_cast<int>(y);
	}
}

// OFFSET: 0x006236a0, STATUS: COMPLETE
void UIContainer::GetPosition(float* x, float* y) {
	*x = transform.m[3][0];
	*y = transform.m[3][1];
}

// OFFSET: 0x006236c0, STATUS: COMPLETE
void UIContainer::SetPosition(float x, float y) {
	transform.m[3][0] = x;
	transform.m[3][1] = y;
	transform.m[3][2] = 0.0f;
	Matrix4x4 mat{};
	D3DXMatrixIdentity(&mat);
	if (parent_container != nullptr) {
		parent_container->GetScreenTransform(&mat);
	}
	this->UpdateTransform(&mat);
}

// OFFSET: 0x00625590, STATUS: COMPLETE
void UIContainer::Repaint() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->Repaint();
	}
}

// OFFSET: 0x00623780, STATUS: COMPLETE
void UIContainer::SetTransform(Matrix4x4* mat) {
	transform = *mat;
	if (parent_container == nullptr) {
		this->UpdateTransform(nullptr);
	}
	else {
		Matrix4x4 parent_transform{};
		parent_container->GetScreenTransform(&parent_transform);
		this->UpdateTransform(&parent_transform);
	}
}

// OFFSET: 0x006258c0, STATUS: COMPLETE
void UIContainer::UpdateTransform(Matrix4x4* mat) {
	if (mat == nullptr) {
		for (std::size_t i = 0; i < components.Length(); i++) {
			components[i]->UpdateTransform(&transform);
		}
	}
	else {
		Matrix4x4 new_transform{};
		D3DXMatrixMultiply(&new_transform, &transform, mat);
		for (std::size_t i = 0; i < components.Length(); i++) {
			components[i]->UpdateTransform(&new_transform);
		}
	}
}

// OFFSET: 0x00623820, STATUS: COMPLETE
void UIContainer::ApplyLayout(UIComponentLayout* layout) {
	UIComponent::ApplyLayout(layout);
	UIContainerLayout* upcast = static_cast<UIContainerLayout*>(layout);
	this->SetPosition(upcast->x, upcast->y);
	this->SetScale(upcast->scale);
}

// OFFSET: 0x00625870, STATUS: COMPLETE
void UIContainer::SetAlpha(int alpha) {
	UIComponent::SetAlpha(alpha);
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->SetAlpha(alpha);
	}
}

// OFFSET: 0x00625810, STATUS: COMPLETE
void UIContainer::ScaleAlpha(float scale) {
	UIComponent::ScaleAlpha(scale);
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->ScaleAlpha(scale);
	}
}

// OFFSET: 0x00625a00, STATUS: COMPLETE
void UIContainer::RegisterEventListeners() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->RegisterEventListeners();
	}
}

// OFFSET: 0x00625a40, STATUS: COMPLETE
void UIContainer::DeRegisterEventListeners() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->DeRegisterEventListeners();
	}
}

// OFFSET: 0x00623800, STATUS: COMPLETE
void UIContainer::SetScale(float scale) {
	transform.m[0][0] = scale;
	transform.m[1][1] = scale;
	this->UpdateTransform(nullptr);
}

// OFFSET: 0x00625970, STATUS: COMPLETE
void UIContainer::DoTick(float dt) {
	for (std::size_t i = 0; i < components.Length(); i++) {
		if (components[i]->enabled) {
			components[i]->DoTick(dt);
		}
	}
}

// OFFSET: 0x006259b0, STATUS: COMPLETE
void UIContainer::DoRender(int render) {
	for (std::size_t i = 0; i < components.Length(); i++) {
		if (components[i]->visible) {
			components[i]->DoRender(render);
		}
	}
}