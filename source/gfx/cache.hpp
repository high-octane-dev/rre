#pragma once

struct ListEntry {
	int offset;
	int len;
};

class Cache {
private:
	int offset;
	int len;
	void* linked_list;
public:
	Cache();
	~Cache();
	Cache(const Cache&) = delete;
	Cache& operator=(const Cache&) = delete;
	void FreeAllNodes();

	virtual ListEntry* CreateListEntry(int, int);
	virtual void FreeListEntry(ListEntry*);
	virtual ListEntry* ClearAndAddListEntry(int);
};

class StaticCache : public Cache {};
