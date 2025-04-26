#include "cache.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
Cache::Cache() {
	offset = 0;
	len = 0;
	linked_list = nullptr;
}

// OFFSET: 0x005ea720, STATUS: COMPLETE
Cache::~Cache() {
	offset = 0;
	len = 0;
	FreeAllNodes();
	offset = 0;
	len = 0;
}

// OFFSET: 0x005d0d10, STATUS: COMPLETE
void Cache::FreeAllNodes() {
	if (linked_list != nullptr) {
		for (auto& elem : *linked_list) {
			FreeListEntry(elem);
		}
		delete linked_list;
		linked_list = nullptr;
	}
}

// OFFSET: 0x00543870, STATUS: COMPLETE
ListEntry* Cache::CreateListEntry(int _offset, int _len) {
	return new ListEntry(_offset, _len);
}

// OFFSET: 0x00577780, STATUS: COMPLETE
void Cache::FreeListEntry(ListEntry* entry) {
	if (entry != nullptr) delete entry;
}

// OFFSET: 0x005777a0, STATUS: COMPLETE
ListEntry* Cache::ClearAndAddListEntry(int) {
	int current_offset = 0;
	for (LinkedListNode<ListEntry*>* current_node = linked_list->start_node; current_node != nullptr; current_node = current_node->next) {
		ListEntry* current_entry = current_node->data;
		int offset_into_data = 0;
		if (current_node->next == nullptr) {
			offset_into_data = len + offset - current_entry->offset;
		}
		else {
			offset_into_data = current_node->next->data->offset - current_entry->offset;
		}
		current_offset = current_offset + offset_into_data;
		FreeListEntry(current_entry);
		linked_list->RemoveNode(current_node);
		if (offset <= current_offset) break;
	}
	ListEntry* new_node = CreateListEntry(offset, current_offset);
	if (new_node == nullptr) {
		return nullptr;
	}
	linked_list->Append(new_node);
	return new_node;
}
