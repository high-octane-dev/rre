#pragma once
#include "graphics_data_item.hpp"

class X360IconGeometryDataItem : public GraphicsDataItem {

public:
	inline X360IconGeometryDataItem() : GraphicsDataItem() {};
	inline virtual ~X360IconGeometryDataItem() override {
	}
	virtual void SendData(int) = 0;
};