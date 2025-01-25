#pragma once
#include "searchable_object.hpp"
#include "block_allocator.hpp"
#include "math/vector4.hpp"

extern int g_SearchPlacementCount;
extern BlockAllocator* g_SearchPlacementAllocator;
struct SearchNode {};
struct SearchMethod {};

class SearchPlacement {
public:
    Vector4 vec;
    void* unk;
    SearchNode* search_node;
    SearchMethod* search_method;
    unsigned int box_placement_code;

    SearchPlacement();
    SearchPlacement(const SearchPlacement&) = delete;
    SearchPlacement& operator=(const SearchPlacement&) = delete;
    ~SearchPlacement();

	void FreeBlockAllocator();
    void PerformInitialPlacement(SearchableObject*, SearchNode*, unsigned int, Vector4 const*, Vector4 const*, float, int, char*);
    void Remove(SearchableObject*);
    void UpdateNeighbors(SearchableObject*, Vector4 const*, Vector4 const*, float);
    void UpdatePlacement(SearchableObject*, Vector4 const*, Vector4 const*, float);
};

class DynamicSearchableObject : public SearchableObject {
protected:
    SearchPlacement* placement;
public:
	DynamicSearchableObject();
	DynamicSearchableObject(const DynamicSearchableObject&) = delete;
	DynamicSearchableObject& operator=(const DynamicSearchableObject&) = delete;
	virtual ~DynamicSearchableObject() override;

    void PerformInitialPlacement(SearchNode*, unsigned int, Vector4 const*, Vector4 const*, float, int, int, char*);
    void RemoveFromSearch();
    void ResetInitialPlacement(SearchNode*, unsigned int, Vector4 const*, Vector4 const*, float, int, int, char*);
    void UpdatePlacement(Vector4 const*, Vector4 const*, float);
};