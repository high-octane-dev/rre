#pragma once
#include <cstdint>
#include <cstring>
#include "mouse_menu.hpp"
#include "parameter_block.hpp"
#include "math\matrix4x4.hpp"

class UIComponentLayout {
public:
	enum class ApplyCondition : std::uint8_t {
		Never = 0,
		Once,
		Always
	};
public:
	char* name;
	int visible;
	int enabled;
	int focusable;
	int focused;
	int selectable;
	int selected;
	ApplyCondition condition;
public:
	// OFFSET: 0x0060f310, STATUS: COMPLETE
	inline UIComponentLayout(char* new_name) {
		std::size_t len = strlen(new_name);
		name = new char[len + 1]();
		memcpy(name, new_name, len);
		name[len] = 0;
		visible = 1;
		enabled = 1;
		selectable = 1;
		selected = 0;
		focusable = 1;
		focused = 0;
		condition = ApplyCondition::Always;
	}
	// OFFSET: 0x00610280, STATUS: COMPLETE
	inline virtual ~UIComponentLayout() {
		delete[] name;
	}
	virtual void Create(ParameterBlock*);
};

struct _RECT {
	int left;
	int top;
	int right;
	int bottom;
};

class UIComponent {
public:
	char unk[12] = {};
	class UIContainer* parent_container;
	// char unk2[12] = {};
	__declspec(align(16)) Matrix4x4 transform;
	char* name;
	int visible;
	int enabled;
	int focusable;
	int focused;
	int selectable;
	int selected;
	MouseMenu* parent_mouse_menu;
public:
	UIComponent();
	virtual ~UIComponent();
	virtual void Create(char* new_name);
	virtual void StartComponent();
	virtual void EndComponent();
	virtual int IsContainer();
	virtual UIComponent* GetComponent(char* component);
	virtual void GetDimensions(float* x, float* y);
	virtual void SetDimensions(float* x, float* y);
	virtual void GetExtents(_RECT* extents);
	virtual void GetExtentsRelative(_RECT* extents);
	virtual float GetWidth();
	virtual float GetHeight();
	virtual void GetPosition(float* x, float* y);
	virtual void SetPosition(float x, float y);
	virtual void GetAbsolutePosition(float* x, float* y);
	virtual void SetAngle(float angle);
	virtual void PropogateChange();
	virtual void Repaint();
	virtual Matrix4x4* GetTransform();
	virtual Matrix4x4* GetScreenTransform(Matrix4x4* dest);
	virtual void SetTransform(Matrix4x4* mat);
	virtual void UpdateTransform(Matrix4x4* mat);
	virtual void ApplyLayout(UIComponentLayout* layout);
	virtual std::uint32_t GetColor();
	virtual std::uint32_t GetAlpha();
	virtual void SetAlpha(int alpha);
	virtual void ScaleAlpha(float scale);
	virtual void SetColor(std::uint32_t color);
	virtual void GetObjectColorScale(Vector4* object_color_scale);
	virtual void SetObjectColorScale(Vector4 object_color_scale);
	virtual void SetEnabled(int _enabled);
	virtual void SetVisible(int _visible);
	virtual void SetFocusable(int _focusable);
	virtual void SetFocused(UIComponent* other);
	virtual void SetFocused(int _focused);
	virtual void SetSelectable(int _selectable);
	virtual void SetSelected(UIComponent* other);
	virtual void SetSelected(int _selected);
	virtual void RegisterEventListeners();
	virtual void DeRegisterEventListeners();
	virtual void SetScale(float scale);
	virtual class UILayer* GetLayer();
	virtual class UIStyleSheet* GetStyleSheet();
	virtual void CursorClicked(std::uint32_t u_msg, int x, int y);
	virtual void CursorSelected(int x, int y);
	virtual uintptr_t FUN_00625050();
	virtual void ResetMouseMenu();
	virtual void SetMouseMenu(MouseMenu* menu);
	virtual bool FUN_00625170();
	virtual void DoTick(float dt);
	virtual void DoRender(int);

	void Render(int);
};

static_assert(sizeof(UIComponentLayout) == 0x24);
static_assert(sizeof(UIComponent) == 0x80);