#pragma once
#include <cstddef>
#include <d3d9.h>
#include "base_object.hpp"

class BaseTextureMap : public BaseObject {
protected:
	unsigned int unk1;
	unsigned short pool;
	unsigned short multi_sample_type;
	float max_projected_size;
public:
	virtual void ClearMaxProjectedSize();
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
	short unk6;
public:
	virtual TextureMap* GetData();
	virtual void Unk1() = 0;
	virtual int Unk2(unsigned int) = 0;
	virtual int Unk3() = 0;
	virtual int Unk4(void *, int, int, int, int, int, D3DFORMAT, unsigned int) = 0;
	virtual int Unk5() = 0;
	virtual int Unk6() = 0;
	virtual int Unk7() = 0;
	virtual int Unk8() = 0;
	virtual void Unk9() = 0;
	virtual void Unk10() = 0;
	virtual int Unk11(int, int, int*) = 0;
	virtual int Unk12() = 0;
	virtual int Unk13(int) = 0;
	virtual int * Unk14(int) = 0;
	virtual int Unk15(int) = 0;
	virtual void Unk16(int) = 0;
	virtual int Unk17() = 0;
};

class X360TextureMap : public TextureMap {
public:
	IDirect3DTexture9* texture;
	IDirect3DCubeTexture9* cube_texture;
public:
	void Unk1();
	int Unk2(unsigned int);
	int Unk3();
	int Unk4(void*, int, int, int, int, int, D3DFORMAT, unsigned int);
	int Unk5();
	int Unk6();
	int Unk7();
	int Unk8();
	void Unk9();
	void Unk10();
	int Unk11(int, int, int*);
	int Unk12();
	int Unk13(int);
	int* Unk14(int);
	int Unk15(int);
	void Unk16(int);
	int Unk17();

	int LoadViaD3DX(std::uint8_t*, std::size_t);
	void ReadDesc();
};