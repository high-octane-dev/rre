#pragma 
#include <d3d9.h>
#include <cstddef>
#include "containers/container_linked_list.hpp"
#include "x360_index_buffer_cache.hpp"

class IndexBufferManager {
public:
	RRE_DISABLE_COPY(IndexBufferManager);
	virtual int CreateIndexBuffer(D3DFORMAT format, int index_count) = 0;
	virtual void EmptyCacheList() = 0;
};

class X360IndexBufferManager : public IndexBufferManager {
private:
	std::size_t unk_capacity;
	LinkedList<X360IndexBufferCache*>* cache_list;
	unsigned int unused;
public:
	X360IndexBufferManager();
	~X360IndexBufferManager();
	virtual int CreateIndexBuffer(D3DFORMAT format, int index_count) override;
	virtual void EmptyCacheList() override;
};