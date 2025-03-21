#include <dxsdk-d3dx/d3dx9tex.h>
#include "x360_texture_map.hpp"
#include "data_access.hpp"
#include "x360_video_card.hpp"

extern int g_EnableEndianSwapping;

// OFFSET: 0x005966b0, STATUS: TODO
TextureMap::~TextureMap() {
}

// OFFSET: 0x0040f760, STATUS: COMPLETE
void TextureMap::ClearMaxProjectedSize() {
	max_projected_size = 0.0f;
}

// OFFSET: 0x0040f770, STATUS: COMPLETE
TextureMap* TextureMap::GetData(int) {
	return this;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360TextureMap::Unk1(int) {
	return;
}

// OFFSET: 0x00410100, STATUS: COMPLETE
int X360TextureMap::LoadFromContainer(int resource_handle) {
	D3DXIMAGE_INFO info{};
	D3DSURFACE_DESC desc{};

	unused_zero = 0;
	std::uint32_t file_size = 0;
	lpDataAccess->FRead(resource_handle, &file_size, sizeof(std::uint32_t), 1);
	if (g_EnableEndianSwapping == 0) {
		file_size = std::byteswap(file_size);
	}
	void* src_data = malloc(file_size);
	lpDataAccess->FRead(resource_handle, src_data, file_size, 1);
	D3DXGetImageInfoFromFileInMemory(src_data, file_size, &info);
	if (info.ResourceType == D3DRTYPE_CUBETEXTURE) {
		D3DXCreateCubeTextureFromFileInMemoryEx(g_D3DDevice9, src_data, file_size, info.Width, info.MipLevels, 0, info.Format, D3DPOOL_MANAGED, -1, -1, 0, nullptr, nullptr, &cube_texture);
	}
	else {
		D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, src_data, file_size, info.Width, info.Height, info.MipLevels, 0, info.Format, D3DPOOL_MANAGED, -1, -1, 0, nullptr, nullptr, &texture);
	}
	free(src_data);
	if (texture == nullptr) {
		texture->GetLevelDesc(0, &desc);
	}
	else {
		if (cube_texture == nullptr) {
			return 0;
		}
		cube_texture->GetLevelDesc(0, &desc);
	}
	format = info.Format;
	pool = desc.Pool;
	multi_sample_type = desc.MultiSampleType;
	return 1;
}

// OFFSET: 0x00442ef0, STATUS: COMPLETE
int X360TextureMap::LoadFromSurfaces(long, ImageDataFormat, unsigned int, X360Palette*) {
	return 0;
}

// OFFSET: 0x00410260, STATUS: TODO
int X360TextureMap::LoadFromImage(unsigned char*, int, int, int, int, int, D3DFORMAT, unsigned int) {
	return 0;
}

// OFFSET: 0x0040fb90, STATUS: TODO
TextureMap* X360TextureMap::Clone() {
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360TextureMap::InVideoRAM() {
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360TextureMap::PlaceInVideoMemory(int, int, int) {
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360TextureMap::UpdateVideoMemory(RECT*, int) {
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360TextureMap::RemoveFromVideoMemory() {
	return;
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360TextureMap::Unk2() {
	return;
}

// OFFSET: 0x0040fde0, STATUS: TODO
int X360TextureMap::LockSys(RECT*, long*, unsigned int) {
	return 0;
}

// OFFSET: 0x0040fe30, STATUS: COMPLETE
int X360TextureMap::UnlockSys(RECT*) {
	return texture->UnlockRect(0) == 0;
}

// OFFSET: 0x0040fcb0, STATUS: TODO
int X360TextureMap::BuildMipMapChain(int) {
	return 0;
}

// OFFSET: 0x00410570, STATUS: TODO
int* X360TextureMap::LockMipMapSurface(int) {
	return nullptr;
}

// OFFSET: 0x004105c0, STATUS: TODO
int X360TextureMap::UnlockMipMapSurface(int) {
	return 0;
}

// OFFSET: 0x0040fd90, STATUS: TODO
void X360TextureMap::SetTexture(int) {
	return;
}

// OFFSET: 0x00410e40, STATUS: TODO
int X360TextureMap::DumpChain() {
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

// OFFSET: 0x0040fff0, STATUS: COMPLETE
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
	format = D3DFMT_UNKNOWN;
	pool = desc.Pool;
	multi_sample_type = desc.MultiSampleType;
}
