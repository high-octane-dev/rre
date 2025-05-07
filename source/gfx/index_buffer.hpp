#pragma once
#include <d3d9.h>

class IndexBuffer {
protected:
	int unk;
	D3DFORMAT format;
	int bytes_per_index;
	int index_count;
public:
	RRE_DISABLE_COPY(IndexBuffer);
	IndexBuffer();
	~IndexBuffer();
	virtual int Create(D3DFORMAT fmt, int index_count) = 0;
	virtual void Release() = 0;
	virtual int ReadData(void* dst, int data_index, int len) = 0;
	virtual int CopyFrom(IndexBuffer* other, int unk, int data_index, int len) = 0;
	virtual int Lock(int data_index, int count, DWORD flags) = 0;
	virtual bool Unlock() = 0;
	virtual IDirect3DIndexBuffer9* Get() = 0;
	virtual void Unk() = 0;
};
