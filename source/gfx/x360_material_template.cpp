#include "x360_material_template.hpp"

#include <cstring>

#include "parameter_block.hpp"
#include "x360_parse.hpp"
#include "util/rsstring_util.hpp"

// OFFSET: 0x0040ba30, STATUS: COMPLETE
X360MaterialTemplate::X360MaterialTemplate() {
	memset(VertexShaderName, 0, sizeof(VertexShaderName));
	memset(PixelShaderName, 0, sizeof(PixelShaderName));
	ShaderType = 0;

	for (unsigned int i = 0; i < 6; ++i) {
		AddressU[i] = D3DTADDRESS_WRAP;
		AddressV[i] = D3DTADDRESS_WRAP;
		AddressW[i] = D3DTADDRESS_WRAP;
	}

	AddressU[6] = D3DTADDRESS_CLAMP;
	AddressV[6] = D3DTADDRESS_CLAMP;
	AddressW[6] = D3DTADDRESS_CLAMP;

	for (unsigned int i = 7; i < material_template_array_size; ++i) {
		AddressU[i] = D3DTADDRESS_BORDER;
		AddressV[i] = D3DTADDRESS_BORDER;
		AddressW[i] = D3DTADDRESS_BORDER;
	}

	for (unsigned int i = 0; i < 8; ++i) {
		MagFilter[i] = D3DTEXF_LINEAR;
		MinFilter[i] = D3DTEXF_LINEAR;
		MipFilter[i] = D3DTEXF_LINEAR;
	}

	for (unsigned int i = 8; i < material_template_array_size; ++i) {
		MagFilter[i] = D3DTEXF_POINT;
		MinFilter[i] = D3DTEXF_POINT;
		MipFilter[i] = D3DTEXF_NONE;
	}

	for (unsigned int i = 0; i < material_template_array_size; ++i) {
		MipMapLodBias[i] = 0.0;
		MaxAnisotropy[i] = 10;
	}

	AlphaBlendEnable = false;
	AlphaTestEnable = false;
	AlphaRef = 0;
	HighPrecisionEnable = false;
	SrcBlend = D3DBLEND_SRCALPHA;
	AlphaFunc = D3DCMP_GREATER;
	ZEnable = D3DZB_TRUE;
	ZFunc = D3DCMP_GREATEREQUAL;
	ZWriteEnable = true;
	UnkField = 0xf;
	CullMode = D3DCULL_CCW;
	DestBlend = D3DBLEND_INVSRCALPHA;
	BlendOp = D3DBLENDOP_ADD;
}

// OFFSET: 0x0040bb50, STATUS: COMPLETE
void X360MaterialTemplate::LoadFromFile(const char* file_name) {
	char* format_arg;
	ParameterBlock parameter_block;
	char parameter_content[64];
	char file_name_with_extension[260];
	char ssnprintf_dest[64];
	int result = 0;

	// originally it probably used strcpy
	strncpy_s(file_name_with_extension, file_name, sizeof(file_name_with_extension) - 1);
	strncat_s(file_name_with_extension, ".xnm", sizeof(file_name_with_extension) - 1);

	if (parameter_block.OpenFile(file_name_with_extension, 0, -1, nullptr, 0xffffffff) == 0) {
		return;
	}

	parameter_block.ReadParameterBlock("DefaultShaderParams");
	parameter_block.GetParameter("VertexShaderName", "default.xvu", VertexShaderName, sizeof(VertexShaderName));
	parameter_block.GetParameter("PixelShaderName", "default.xpu", PixelShaderName, sizeof(PixelShaderName));
	parameter_block.GetParameter("PixelShaderType", "3D", parameter_content, sizeof(parameter_content));

	if (_strcmpi(parameter_content, "3D") == 0) {
		ShaderType = 0;
	}
	else if (_strcmpi(parameter_content, "2D") == 0) {
		ShaderType = 1;
	}

	for (unsigned int array_index = 0; array_index < material_template_array_size; ++array_index) {
		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "ADDRESSU_%d", array_index);
		X360Parse::GetAddressMode(&parameter_block, parameter_content, AddressU[array_index], &AddressU[array_index]);
		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "ADDRESSV_%d", array_index);
		X360Parse::GetAddressMode(&parameter_block, parameter_content, AddressV[array_index], &AddressV[array_index]);
		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "ADDRESSW_%d", array_index);
		X360Parse::GetAddressMode(&parameter_block, parameter_content, AddressW[array_index], &AddressW[array_index]);

		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "MAGFILTER_%d", array_index);
		X360Parse::GetFilter(&parameter_block, parameter_content, MagFilter[array_index], &MagFilter[array_index]);
		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "MINFILTER_%d", array_index);
		X360Parse::GetFilter(&parameter_block, parameter_content, MinFilter[array_index], &MinFilter[array_index]);
		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "MIPFILTER_%d", array_index);
		X360Parse::GetFilter(&parameter_block, parameter_content, MipFilter[array_index], &MipFilter[array_index]);

		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "MIPMAPLODBIAS_%d", array_index);
		X360Parse::GetFloat(&parameter_block, parameter_content, MipMapLodBias[array_index],
		                    &MipMapLodBias[array_index]);
		RSStringUtil::Ssnprintf(parameter_content, sizeof(parameter_content), "MAXANISOTROPY_%d", array_index);
		X360Parse::GetLong(&parameter_block, parameter_content, MaxAnisotropy[array_index],
		                   &MaxAnisotropy[array_index]);
	}

	X360Parse::GetD3DZBufferType(&parameter_block, "ZENABLE", ZEnable, &ZEnable);
	X360Parse::GetD3DCmpFunc(&parameter_block, "ZFUNC", ZFunc, &ZFunc);
	X360Parse::GetBool(&parameter_block, "ZWRITEENABLE", ZWriteEnable, &ZWriteEnable);
	X360Parse::GetD3DCull(&parameter_block, "CULLMODE", CullMode, &CullMode);
	X360Parse::GetBool(&parameter_block, "ALPHABLENDENABLE", AlphaBlendEnable, &AlphaBlendEnable);
	X360Parse::GetD3DBlend(&parameter_block, "SRCBLEND", SrcBlend, &SrcBlend);
	X360Parse::GetD3DBlend(&parameter_block, "DESTBLEND", DestBlend, &DestBlend);
	X360Parse::GetD3DBlendOp(&parameter_block, "BLENDOP", BlendOp, &BlendOp);
	X360Parse::GetBool(&parameter_block, "ALPHATESTENABLE", AlphaTestEnable, &AlphaTestEnable);
	X360Parse::GetULong(&parameter_block, "ALPHAREF", AlphaRef, &AlphaRef);
	X360Parse::GetD3DCmpFunc(&parameter_block, "ALPHAFUNC", AlphaFunc, &AlphaFunc);
	X360Parse::GetBool(&parameter_block, "HIGHPRECISIONBLENDENABLE", HighPrecisionEnable, &HighPrecisionEnable);
}
