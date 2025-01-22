#pragma once
#include <d3d9.h>

class IndexBuffer {
protected:
	int unk;
	D3DFORMAT format;
	int bytes_per_index;
	int index_count;
public:
	virtual int Create(D3DFORMAT fmt, int index_count);
	virtual void Release();
	virtual int ReadData(void* dst, int data_index, int len);
	virtual int CopyFrom(IndexBuffer* other, int unk, int data_index, int len);
	virtual int Lock(int data_index, int count, DWORD flags);
	virtual bool Unlock();
	virtual IDirect3DIndexBuffer9* Get();
	virtual void Unk();
};
