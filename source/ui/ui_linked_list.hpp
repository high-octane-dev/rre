#pragma once
#include <cstddef>

template <typename T> class UILinkedListNode {
public:
	T value;
	UILinkedListNode<T>* prev;
	UILinkedListNode<T>* next;
public:
	UILinkedListNode<T>() : value() {
		prev = nullptr;
		next = nullptr;
	}
	virtual ~UILinkedListNode<T>() {
	}
};

template <typename T> class UILinkedList {
private:
	UILinkedListNode<T>* start;
	std::size_t len;
public:
	UILinkedList<T>() {
		start = nullptr;
		len = 0;
	}
	virtual ~UILinkedList<T>() {
		UILinkedListNode<T>* node = this->start;
		while (len > 0) {
			if (node == nullptr) {
				break;
			}
			UILinkedListNode<T>* next_node = node->next;
			node->~UILinkedListNode<T>();
			node = next_node;
			len--;
		}
	}
};

#ifdef _M_IX86
static_assert(sizeof(UILinkedListNode<int>) == 0x10);
static_assert(sizeof(UILinkedList<int>) == 0xc);
#endif