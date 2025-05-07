#pragma once

#include <d3d9.h>

class X360MaterialTemplate {
	static constexpr int material_template_array_size = 16;
	using MaterialD3DTextureAddressArray = D3DTEXTUREADDRESS[material_template_array_size];
	using MaterialD3DTextureFilterTypeArray = D3DTEXTUREFILTERTYPE[material_template_array_size];

	char VertexShaderName[64];
	char PixelShaderName[64];
	// 3D: 0
	// 2D: 1
	int ShaderType;
	MaterialD3DTextureAddressArray AddressU;
	MaterialD3DTextureAddressArray AddressV;
	MaterialD3DTextureAddressArray AddressW;

	MaterialD3DTextureFilterTypeArray MagFilter;
	MaterialD3DTextureFilterTypeArray MinFilter;
	MaterialD3DTextureFilterTypeArray MipFilter;

	float MipMapLodBias[material_template_array_size];
	long MaxAnisotropy[material_template_array_size];

	D3DZBUFFERTYPE ZEnable;
	D3DCMPFUNC ZFunc;
	// Im not sure about those booleans. These are 4 byte fields in memory
	// but this works out because of memory alignment.
	// Maybe these are just ints anyways.
	bool ZWriteEnable;
	unsigned int UnkField;
	D3DCULL CullMode;
	bool AlphaBlendEnable;
	D3DBLEND SrcBlend;
	D3DBLEND DestBlend;
	D3DBLENDOP BlendOp;
	bool AlphaTestEnable;
	unsigned long AlphaRef;
	D3DCMPFUNC AlphaFunc;
	bool HighPrecisionEnable;

public:
	X360MaterialTemplate();
	void LoadFromFile(const char* file_name);
};

#ifdef _M_IX86
static_assert(sizeof(X360MaterialTemplate) == 0x2b8);
#endif