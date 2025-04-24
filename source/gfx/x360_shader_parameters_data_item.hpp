#pragma once
#include <cstddef>
#include <cstdint>
#include "graphics_data_item.hpp"
#include "x360_video_card.hpp"

struct ShaderParameter {
	std::uint32_t is_vertex;
	std::uint32_t register_index;
	float value[4];
};

class X360ShaderParametersDataItem : public GraphicsDataItem {
protected:
	std::size_t parameter_count;
	ShaderParameter* parameters;
public:
	// OFFSET: 0x00409fd0, STATUS: COMPLETE
	inline X360ShaderParametersDataItem(std::uint32_t desired_parameter_count) : GraphicsDataItem(), parameter_count(desired_parameter_count) {
		parameters = new ShaderParameter[parameter_count]{};
	}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual ~X360ShaderParametersDataItem() override {
		delete[] parameters;
	}
	// OFFSET: 0x0040a080, STATUS: COMPLETE
	inline virtual void SendData(int) override {
		for (std::size_t i = 0; i < parameter_count; i++) {
			if (parameters[i].is_vertex == 1) {
				g_D3DDevice9->SetVertexShaderConstantF(parameters[i].register_index, &parameters[i].value[0], 1);
			}
			else {
				g_D3DDevice9->SetPixelShaderConstantF(parameters[i].register_index, &parameters[i].value[0], 1);
			}
		}
	}
};