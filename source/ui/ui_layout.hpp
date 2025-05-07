#pragma once
#include "ui_component.hpp"
#include "containers/container_list.hpp"

class UILayout {
public:
	std::uint8_t unk;
	ContainerList<UIComponentLayout*> component_layouts;
public:
	RRE_DISABLE_COPY(UILayout);
	virtual ~UILayout();
	virtual void Create();

	UIComponentLayout* GetComponentLayout(char* name);
};

#ifdef _M_IX86
static_assert(sizeof(UILayout) == 0x20);
#endif