#pragma once
#include "ui_layer.hpp"
#include "ui_event.hpp"
#include "virtual_network.hpp"

class CarsBaseLayer : public UILayer, VNObject, UIFlashAnimationListener {
protected:
	int flags;
public:
	CarsBaseLayer();
	virtual ~CarsBaseLayer() override;
	virtual void Render(int) override;
};

#ifdef _M_IX86
static_assert(sizeof(CarsBaseLayer) == 0x44);
#endif