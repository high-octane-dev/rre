#pragma once
#include <d3d9.h>
#include "cache.hpp"
#include "index_buffer.hpp"

class VertexBuffer {
private:
	int unk;
	D3DFORMAT format;
	int bytes_per_vertex;
	int vertex_count;
public:
	RRE_DISABLE_COPY(VertexBuffer);
	virtual int Create(D3DFORMAT fmt, int index_count);
	virtual void Release();
	virtual int ReadData(void* dst, int data_index, int len);
	virtual int CopyFrom(VertexBuffer* other, int unk, int data_index, int len);
	virtual int Lock(int data_index, int count, DWORD flags);
	virtual bool Unlock();
	virtual IDirect3DVertexBuffer9* Get();
	virtual void Unk();
};