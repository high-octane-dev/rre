#pragma once
#include <d3d9.h>
#include "cache.hpp"

class IndexBuffer {
private:
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

class VertexBuffer {
private:
	int unk;
	D3DFORMAT format;
	int bytes_per_vertex;
	int vertex_count;
public:
	virtual int Create(D3DFORMAT fmt, int index_count);
	virtual void Release();
	virtual int ReadData(void* dst, int data_index, int len);
	virtual int CopyFrom(IndexBuffer* other, int unk, int data_index, int len);
	virtual int Lock(int data_index, int count, DWORD flags);
	virtual bool Unlock();
	virtual IDirect3DVertexBuffer9* Get();
	virtual void Unk();
};

class X360VertexBuffer : public VertexBuffer {
private:
	IDirect3DVertexBuffer9* vertex_buffer;
};

class X360IndexBuffer : public IndexBuffer {
private:
	IDirect3DIndexBuffer9* vertex_buffer;
};

class X360VertexBufferCache {
private:
	X360VertexBuffer* vertex_buffer;
	StaticCache* cache;
};

class X360IndexBufferCache {
private:
	X360IndexBuffer* index_buffer;
	StaticCache* cache;
};