#include "x360_parse.hpp"

#include <string.h>
#include <d3d9types.h>
#include <cstdlib>

#include "BASS.h"

// OFFSET: 0x00424070, STATUS: COMPLETE
void X360Parse::GetAddressMode(ParameterBlock* parameter_block, const char* parameter, const D3DTEXTUREADDRESS fallback, D3DTEXTUREADDRESS* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DTADDRESS_WRAP") == 0) {
		*dest = D3DTADDRESS_WRAP;
		return;
	}

	if (_stricmp(parameter_content, "D3DTADDRESS_MIRROR") == 0) {
		*dest = D3DTADDRESS_MIRROR;
		return;
	}

	if (_stricmp(parameter_content, "D3DTADDRESS_CLAMP") == 0) {
		*dest = D3DTADDRESS_CLAMP;
		return;
	}

	if (_stricmp(parameter_content, "D3DTADDRESS_MIRRORONCE") == 0) {
		*dest = D3DTADDRESS_MIRRORONCE;
		return;
	}

	if (_stricmp(parameter_content, "D3DTADDRESS_BORDER_HALF") == 0 || _stricmp(parameter_content, "D3DTADDRESS_BORDER")
		== 0) {
		*dest = D3DTADDRESS_BORDER;
		return;
	}

	if (_stricmp(parameter_content, "D3DTADDRESS_MIRRORONCE_BORDER_HALF") == 0 || _stricmp(
		parameter_content, "D3DTADDRESS_MIRRORONCE_BORDER") == 0) {
		*dest = D3DTADDRESS_MIRRORONCE;
	}
}

// OFFSET: 0x00424200, STATUS: COMPLETE
void X360Parse::GetFilter(ParameterBlock* parameter_block, const char* parameter, const D3DTEXTUREFILTERTYPE fallback, D3DTEXTUREFILTERTYPE* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DTEXF_NONE") == 0) {
		*dest = D3DTEXF_NONE;
		return;
	}

	if (_stricmp(parameter_content, "D3DTEXF_POINT") == 0) {
		*dest = D3DTEXF_POINT;
		return;
	}

	if (_stricmp(parameter_content, "D3DTEXF_LINEAR") == 0) {
		*dest = D3DTEXF_LINEAR;
		return;
	}

	if (_stricmp(parameter_content, "D3DTEXF_ANISOTROPIC") == 0) {
		*dest = D3DTEXF_ANISOTROPIC;
	}
}

// OFFSET: 0x00423c40, STATUS: COMPLETE
void X360Parse::GetFloat(ParameterBlock* parameter_block, const char* parameter, const float fallback, float* dest) {
	char parameter_content[64];
	char* end_ptr = nullptr;

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	*dest = static_cast<float>(std::strtod(parameter_content, &end_ptr));
}

// OFFSET: 0x00423cd0, STATUS: COMPLETE
void X360Parse::GetLong(ParameterBlock* parameter_block, const char* parameter, const long fallback, long* dest) {
	char parameter_content[64];
	char* end_ptr = nullptr;

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	*dest = std::strtol(parameter_content, &end_ptr, 0);
}

// OFFSET: 0x00423e80, STATUS: COMPLETE
void X360Parse::GetULong(ParameterBlock* parameter_block, const char* parameter, const unsigned long fallback, unsigned long* dest) {
	char parameter_content[64];
	char* end_ptr = nullptr;

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	*dest = std::strtoul(parameter_content, &end_ptr, 0);
}

// OFFSET: 0x00423d60, STATUS: COMPLETE
void X360Parse::GetBool(ParameterBlock* parameter_block, const char* parameter, const bool fallback, bool* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	// false values are: F; 0; N
	*dest = _stricmp(parameter_content, "T") == 0 || _stricmp(parameter_content, "1") == 0 || _stricmp(
		parameter_content, "Y") == 0;
}

// OFFSET: 0x00424310, STATUS: COMPLETE
void X360Parse::GetD3DCmpFunc(ParameterBlock* parameter_block, const char* parameter, const D3DCMPFUNC fallback, D3DCMPFUNC* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_NEVER") == 0) {
		*dest = D3DCMP_NEVER;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_LESS") == 0) {
		*dest = D3DCMP_LESS;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_EQUAL") == 0) {
		*dest = D3DCMP_EQUAL;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_LESSEQUAL") == 0) {
		*dest = D3DCMP_LESSEQUAL;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_GREATER") == 0) {
		*dest = D3DCMP_GREATER;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_NOTEQUAL") == 0) {
		*dest = D3DCMP_NOTEQUAL;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_GREATEREQUAL") == 0) {
		*dest = D3DCMP_GREATEREQUAL;
		return;
	}

	if (_stricmp(parameter_content, "D3DCMP_ALWAYS") == 0) {
		*dest = D3DCMP_ALWAYS;
	}
}

// OFFSET: 0x00424930, STATUS: COMPLETE
void X360Parse::GetD3DZBufferType(ParameterBlock* parameter_block, const char* parameter, const D3DZBUFFERTYPE fallback, _D3DZBUFFERTYPE* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DZB_FALSE") == 0) {
		*dest = D3DZB_FALSE;
		return;
	}

	if (_stricmp(parameter_content, "D3DZB_TRUE") == 0) {
		*dest = D3DZB_TRUE;
	}
}

// OFFSET: 0x004249f0, STATUS: COMPLETE
void X360Parse::GetD3DCull(ParameterBlock* parameter_block, const char* parameter, const D3DCULL fallback, D3DCULL* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DCULL_NONE") == 0) {
		*dest = D3DCULL_NONE;
		return;
	}

	if (_stricmp(parameter_content, "D3DCULL_CW") == 0) {
		*dest = D3DCULL_CW;
		return;
	}

	if (_stricmp(parameter_content, "D3DCULL_CCW") == 0) {
		*dest = D3DCULL_CCW;
	}
}

// OFFSET: 0x004244e0, STATUS: COMPLETE
void X360Parse::GetD3DBlend(ParameterBlock* parameter_block, const char* parameter, const D3DBLEND fallback, D3DBLEND* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_ZERO") == 0) {
		*dest = D3DBLEND_ZERO;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_ONE") == 0) {
		*dest = D3DBLEND_ONE;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_SRCCOLOR") == 0) {
		*dest = D3DBLEND_SRCCOLOR;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_INVSRCCOLOR") == 0) {
		*dest = D3DBLEND_INVSRCCOLOR;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_SRCALPHA") == 0) {
		*dest = D3DBLEND_SRCALPHA;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_INVSRCALPHA") == 0) {
		*dest = D3DBLEND_INVSRCALPHA;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_DESTCOLOR") == 0) {
		*dest = D3DBLEND_DESTCOLOR;
		return;
	}


	if (_stricmp(parameter_content, "D3DBLEND_INVDESTCOLOR") == 0) {
		*dest = D3DBLEND_INVDESTCOLOR;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_DESTALPHA") == 0) {
		*dest = D3DBLEND_DESTALPHA;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_INVDESTALPHA") == 0) {
		*dest = D3DBLEND_INVDESTALPHA;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_BLENDFACTOR") == 0) {
		*dest = D3DBLEND_BLENDFACTOR;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_INVBLENDFACTOR") == 0) {
		*dest = D3DBLEND_INVBLENDFACTOR;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_CONSTANTALPHA") == 0 || _stricmp(
		parameter_content, "D3DBLEND_INVCONSTANTALPHA") == 0) {
		DebugBreak();
		return;
	}

	if (_stricmp(parameter_content, "D3DBLEND_SRCALPHASAT") == 0) {
		*dest = D3DBLEND_SRCALPHASAT;
	}
}

// OFFSET: 0x004247e0, STATUS: COMPLETE
void X360Parse::GetD3DBlendOp(ParameterBlock* parameter_block, const char* parameter, const D3DBLENDOP fallback, D3DBLENDOP* dest) {
	char parameter_content[64];

	parameter_block->GetParameter(parameter, "undefined", parameter_content, sizeof(parameter_content));
	if (_stricmp(parameter_content, "undefined") == 0) {
		*dest = fallback;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLENDOP_ADD") == 0) {
		*dest = D3DBLENDOP_ADD;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLENDOP_SUBTRACT") == 0) {
		*dest = D3DBLENDOP_SUBTRACT;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLENDOP_MIN") == 0) {
		*dest = D3DBLENDOP_MIN;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLENDOP_MAX") == 0) {
		*dest = D3DBLENDOP_MAX;
		return;
	}

	if (_stricmp(parameter_content, "D3DBLENDOP_REVSUBTRACT") == 0) {
		*dest = D3DBLENDOP_REVSUBTRACT;
		return;
	}

	*dest = D3DBLENDOP_ADD;
}
