#pragma once
#include <cstddef>
#include <cstdint>
#include "graphics_data_item.hpp"
#include "x360_video_card.hpp"

class DrawableCubeDataItem : public GraphicsDataItem {
public:
	struct DrawableCube* cube;
public:
	inline DrawableCubeDataItem() : GraphicsDataItem(), cube(nullptr) {}
	inline virtual void SendData(int) override {
		/*
		int iVar1 = *(int *)(drawable_cube->field3_0x30 + 0x40);
		if ((iVar1 != 0x2b) && (iVar1 != 0x2c)) {
			drawable_cube->RenderAsTexture(param_1);
			return;
		}
		drawable_cube->FUN_00615900(this_00);
		return;
		*/
	}
};