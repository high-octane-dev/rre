#pragma once
#include "containers/container_linked_list.hpp"
#include "util/macros.hpp"

struct ListEntry {
	int offset;
	int len;
};

class Cache {
protected:
	int offset;
	int len;
	LinkedList<ListEntry*>* linked_list;
public:
	RRE_DISABLE_COPY(Cache);
	Cache();
	~Cache();
	void FreeAllNodes();

	virtual ListEntry* CreateListEntry(int, int);
	virtual void FreeListEntry(ListEntry*);
	virtual ListEntry* ClearAndAddListEntry(int);
};

class StaticCache : public Cache {
	// OFFSET: 0x005ea780, STATUS: COMPLETE
	inline StaticCache(int initial_offset, int initial_len) : Cache() {
		offset = initial_offset;
		len = initial_len;
	}
	// OFFSET: INLINE, STATUS: COMPLETE
	inline virtual ListEntry* ClearAndAddListEntry(int) override {
		return nullptr;
	}
};
