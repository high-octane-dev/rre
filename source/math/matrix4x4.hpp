#pragma once
#include <dxsdk-d3dx/d3dx9math.h>

#ifdef _MSC_VER
__declspec(align(16)) struct Matrix4x4 : public D3DXMATRIX { };
#else
struct Matrix4x4 : public D3DXMATRIX {} __attribute__((aligned(16)));
#endif