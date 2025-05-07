#pragma once
#include <cstddef>
#include <d3d9.h>
#include "base_object.hpp"
#include "image_data_format.hpp"
#include "util/macros.hpp"

struct X360Palette;

class BaseTextureMap : public BaseObject {
protected:
	unsigned int unk1;
	unsigned short pool;
	unsigned short multi_sample_type;
	float max_projected_size;
public:
	RRE_DISABLE_COPY(BaseTextureMap);
};

class TextureMap : public BaseTextureMap /*, StreamingComponent */ {
protected:
	int unk1;
	int unk2;
	D3DFORMAT format;
	int unk3;
	int unk4;
public:
	char texture_filename[128];
protected:
	short unk5;
	char level_count;
public:
	virtual ~TextureMap();
	virtual void ClearMaxProjectedSize();
	virtual TextureMap* GetData(int);
	virtual void Unk1(int) = 0;
	virtual int LoadFromContainer(int) = 0;
	virtual int LoadFromSurfaces(long, ImageDataFormat, unsigned int, X360Palette*) = 0;
	virtual int LoadFromImage(unsigned char*, int, int, int, int, int, D3DFORMAT, unsigned int) = 0;
	virtual TextureMap* Clone() = 0;
	virtual int InVideoRAM() = 0;
	virtual int PlaceInVideoMemory(int, int, int) = 0;
	virtual int UpdateVideoMemory(RECT*, int) = 0;
	virtual void RemoveFromVideoMemory() = 0;
	virtual void Unk2() = 0;
	virtual int LockSys(RECT*, long*, unsigned int) = 0;
	virtual int UnlockSys(RECT*) = 0;
	virtual int BuildMipMapChain(int) = 0;
	virtual int* LockMipMapSurface(int) = 0;
	virtual int UnlockMipMapSurface(int) = 0;
	virtual void SetTexture(int) = 0;
	virtual int DumpChain() = 0;
};

class X360TextureMap : public TextureMap {
public:
	IDirect3DTexture9* texture;
	IDirect3DCubeTexture9* cube_texture;
	unsigned int unused_zero;
	void* level_bits[10];
	int level_pitch[10];
	unsigned int unk;
public:
	virtual void Unk1(int) override;
	virtual int LoadFromContainer(int) override;
	virtual int LoadFromSurfaces(long, ImageDataFormat, unsigned int, X360Palette*) override;
	virtual int LoadFromImage(unsigned char*, int, int, int, int, int, D3DFORMAT, unsigned int) override;
	virtual TextureMap* Clone() override;
	virtual int InVideoRAM() override;
	virtual int PlaceInVideoMemory(int, int, int) override;
	virtual int UpdateVideoMemory(RECT*, int) override;
	virtual void RemoveFromVideoMemory() override;
	virtual void Unk2() override;
	virtual int LockSys(RECT*, long*, unsigned int) override;
	virtual int UnlockSys(RECT*) override;
	virtual int BuildMipMapChain(int) override;
	virtual int* LockMipMapSurface(int) override;
	virtual int UnlockMipMapSurface(int) override;
	virtual void SetTexture(int) override;
	virtual int DumpChain() override;

	int LoadViaD3DX(std::uint8_t*, std::size_t);
	void ReadDesc();
};