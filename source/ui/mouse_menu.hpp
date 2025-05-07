#pragma once

class MouseMenu {
public:
	virtual void Callback(class UIComponent* component, int button, int x, int y) = 0;
};

#ifdef _M_IX86
static_assert(sizeof(MouseMenu) == 0x4);
#endif