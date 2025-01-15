#pragma once
#include "parameter_block.hpp"

#include "d3d9types.h"

namespace X360Parse {
	void GetAddressMode(ParameterBlock* parameter_block, const char* parameter, const D3DTEXTUREADDRESS fallback,
	                    D3DTEXTUREADDRESS* dest);
	void GetFilter(ParameterBlock* parameter_block, const char* parameter, const D3DTEXTUREFILTERTYPE fallback,
	               D3DTEXTUREFILTERTYPE* dest);
	void GetFloat(ParameterBlock* parameter_block, const char* parameter, const float fallback, float* dest);
	void GetLong(ParameterBlock* parameter_block, const char* parameter, const long fallback, long* dest);
	void GetULong(ParameterBlock* parameter_block, const char* parameter, const unsigned long fallback,
	              unsigned long* dest);
	void GetD3DCmpFunc(ParameterBlock* parameter_block, const char* parameter, const D3DCMPFUNC fallback,
	                   D3DCMPFUNC* dest);
	void GetD3DZBufferType(ParameterBlock* parameter_block, const char* parameter, const D3DZBUFFERTYPE fallback,
	                       _D3DZBUFFERTYPE* dest);
	void GetD3DCull(ParameterBlock* parameter_block, const char* parameter, const D3DCULL fallback, D3DCULL* dest);
	void GetD3DBlend(ParameterBlock* parameter_block, const char* parameter, const D3DBLEND fallback, D3DBLEND* dest);
	void GetD3DBlendOp(ParameterBlock* parameter_block, const char* parameter, const D3DBLENDOP fallback,
	                   D3DBLENDOP* dest);
	void GetBool(ParameterBlock* parameter_block, const char* parameter, const bool fallback, bool* dest);
}
