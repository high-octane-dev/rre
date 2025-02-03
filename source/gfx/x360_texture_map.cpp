#include <dxsdk-d3dx/d3dx9tex.h>
#include "x360_texture_map.hpp"
#include "data_access.hpp"
#include "x360_video_card.hpp"

extern int g_EnableEndianSwapping;

// OFFSET: 0x0040f760, STATUS: COMPLETE
void BaseTextureMap::ClearMaxProjectedSize() {
	max_projected_size = 0.0f;
}

// OFFSET: 0x0040f770, STATUS: COMPLETE
TextureMap* TextureMap::GetData() {
	return this;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360TextureMap::Unk1() {
	return;
}

// OFFSET: 0x00410100, STATUS: WIP
int X360TextureMap::Unk2(unsigned int a1) {
	D3DXIMAGE_INFO local_3c;

	//this->field3_0xd0 = 0;
	lpDataAccess->FRead(a1, &a1, 4, 1);
	if (g_EnableEndianSwapping == 0)
	{
		a1 = (a1 & 0xff0000 | a1 >> 0x10) >> 8 |
			(a1 & 0xff00 | a1 << 0x10) << 8;
	}
	void* pSrcData = malloc(a1);
	lpDataAccess->FRead(a1, pSrcData, a1, 1);
	D3DXGetImageInfoFromFileInMemory(pSrcData, a1, &local_3c);
	if (local_3c.ResourceType == D3DRTYPE_CUBETEXTURE)
	{
		D3DXCreateCubeTextureFromFileInMemoryEx(g_D3DDevice9, pSrcData, a1, local_3c.Width, local_3c.MipLevels, 0, local_3c.Format, D3DPOOL_MANAGED, -1, -1, 0, nullptr, nullptr, &cube_texture);
	}
	else
	{
		D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, pSrcData, a1, local_3c.Width, local_3c.Height, local_3c.MipLevels, 0, local_3c.Format, D3DPOOL_MANAGED, -1, -1, 0, nullptr, nullptr, &texture);
	}
	free(pSrcData);
	/*
	pIVar2 = (IDirect3DCubeTexture9 *)this->texture;
	if (pIVar2 == (IDirect3DCubeTexture9 *)0x0) {
		pIVar2 = this->cube_texture;
	}
	(*(code *)((IDirect3DTexture9_VTable *)pIVar2->field0_0x0)->GetLevelDesc)
			((IDirect3DTexture9 *)pIVar2,0,&local_20);
	(this->TextureMap).BaseTextureMap.field3_0xe = (word)local_20.Height;
	(this->TextureMap).field4_0x38 = local_20.Format;
	(this->TextureMap).BaseTextureMap.field2_0xc = (word)local_20.Width;
	*/

	return 1;
}

// OFFSET: 0x00442ef0, STATUS: COMPLETE
int X360TextureMap::Unk3() {
	return 0;
}

// OFFSET: 0x00410260, STATUS: TODO
int X360TextureMap::Unk4(void*, int, int, int, int, int, D3DFORMAT, unsigned int) {
	return 0;
}

// OFFSET: 0x0040fb90, STATUS: TODO
int X360TextureMap::Unk5() {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360TextureMap::Unk6() {
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360TextureMap::Unk7() {
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360TextureMap::Unk8() {
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360TextureMap::Unk9() {
	return;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360TextureMap::Unk10() {
	return;
}

// OFFSET: 0x0040fde0, STATUS: TODO
int X360TextureMap::Unk11(int, int, int*) {
	return 0;
}

// OFFSET: 0x0040fe30, STATUS: COMPLETE
int X360TextureMap::Unk12() {
	return texture->UnlockRect(0) == 0;
}

// OFFSET: 0x0040fcb0, STATUS: TODO
int X360TextureMap::Unk13(int) {
	return 0;
}

// OFFSET: 0x00410570, STATUS: TODO
int* X360TextureMap::Unk14(int) {
	return nullptr;
}

// OFFSET: 0x004105c0, STATUS: TODO
int X360TextureMap::Unk15(int) {
	return 0;
}

// OFFSET: 0x0040fd90, STATUS: TODO
void X360TextureMap::Unk16(int) {
	return;
}

// OFFSET: 0x00410e40, STATUS: TODO
int X360TextureMap::Unk17() {
	return 0;
}

// OFFSET: 0x00410040, STATUS: COMPLETE
int X360TextureMap::LoadViaD3DX(std::uint8_t* data, std::size_t len) {
	D3DXIMAGE_INFO info{};
	if (!SUCCEEDED(D3DXGetImageInfoFromFileInMemory(data, len, &info))) {
		return 0;
	}
	if (info.ResourceType == D3DRTYPE_CUBETEXTURE) {
		if (!SUCCEEDED(D3DXCreateCubeTextureFromFileInMemoryEx(g_D3DDevice9, data, len, info.Width, info.MipLevels, 0, info.Format, D3DPOOL_MANAGED, 0xffffffff, 0xffffffff, 0, 0, 0, &cube_texture))) {
			return 0;
		}
	}
	else {
		if (!SUCCEEDED(D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, data, len, info.Width, info.Height, info.MipLevels, 0, info.Format, D3DPOOL_MANAGED, 0xffffffff, 0xffffffff, 0, 0, 0, &texture))) {
			return 0;
		}
	}
	ReadDesc();
	return 1;
}

// OFFSET: 0x0040fff0, STATUS: WIP
void X360TextureMap::ReadDesc(){
	D3DSURFACE_DESC desc{};
	if (texture == nullptr) {
		texture->GetLevelDesc(0, &desc);
	}
	else {
		if (cube_texture == nullptr) {
			return;
		}
		cube_texture->GetLevelDesc(0, &desc);
	}
	// unk_level = 0;
	pool = desc.Pool;
	multi_sample_type = desc.MultiSampleType;
}
