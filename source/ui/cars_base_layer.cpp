#include "cars_base_layer.hpp"

// OFFSET: 0x00473480, STATUS: COMPLETE
CarsBaseLayer::CarsBaseLayer() : UILayer(), VNObject(2), UIFlashAnimationListener() {
	flags = 0;
}

// OFFSET: 0x00427d30, STATUS: COMPLETE
CarsBaseLayer::~CarsBaseLayer() {
}

// OFFSET: 0x004538c0, STATUS: TODO
void CarsBaseLayer::Render(int) {
}
