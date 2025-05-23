#pragma 
#include <d3d9.h>
#include <cstddef>
#include "containers/container_linked_list.hpp"
#include "x360_vertex_buffer_cache.hpp"
#include "util/macros.hpp"

class VertexBufferManager {
public:
	inline VertexBufferManager() {};
	RRE_DISABLE_COPY(VertexBufferManager);
	virtual int CreateVertexBuffer(D3DFORMAT format, int vertex_count) = 0;
	virtual void EmptyCacheList() = 0;
};

class X360VertexBufferManager : public VertexBufferManager {
private:
	std::size_t unk_capacity;
	LinkedList<X360VertexBufferCache*>* cache_list;
	unsigned int unused;
public:
	X360VertexBufferManager();
	~X360VertexBufferManager();
	virtual int CreateVertexBuffer(D3DFORMAT format, int vertex_count) override;
	virtual void EmptyCacheList() override;
};