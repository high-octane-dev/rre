#include "dynamic_searchable_object.hpp"

// OFFSET: 0x0058ef70, STATUS: TODO
SearchPlacement::SearchPlacement() {
}

// OFFSET: INLINE, STATUS: TODO
SearchPlacement::~SearchPlacement() {
}

// OFFSET: INLINE, STATUS: TODO
void SearchPlacement::FreeBlockAllocator() {
}

// OFFSET: 0x005c8170, STATUS: TODO
void SearchPlacement::PerformInitialPlacement(SearchableObject*, SearchNode*, unsigned int, Vector4 const*, Vector4 const*, float, int, char*) {
}

// OFFSET: 0x005603a0, STATUS: TODO
void SearchPlacement::Remove(SearchableObject*) {
}

// OFFSET: 0x005b3a60, STATUS: TODO
void SearchPlacement::UpdateNeighbors(SearchableObject*, Vector4 const*, Vector4 const*, float) {
}

// OFFSET: 0x005c8220, STATUS: TODO
void SearchPlacement::UpdatePlacement(SearchableObject*, Vector4 const*, Vector4 const*, float) {
}

// OFFSET: INLINE, STATUS: TODO
DynamicSearchableObject::DynamicSearchableObject() {
}

// OFFSET: 0x005c7d30, STATUS: TODO
DynamicSearchableObject::~DynamicSearchableObject() {
}

// OFFSET: 0x005dd970, STATUS: TODO
void DynamicSearchableObject::PerformInitialPlacement(SearchNode*, unsigned int, Vector4 const*, Vector4 const*, float, int, int, char*) {
}

// OFFSET: 0x005b36e0, STATUS: TODO
void DynamicSearchableObject::RemoveFromSearch() {
}

// OFFSET: 0x005f3480, STATUS: TODO
void DynamicSearchableObject::ResetInitialPlacement(SearchNode*, unsigned int, Vector4 const*, Vector4 const*, float, int, int, char*) {
}

// OFFSET: INLINE, STATUS: COMPLETE
void DynamicSearchableObject::UpdatePlacement(Vector4 const* unk1, Vector4 const* unk2, float unk3) {
	placement->UpdatePlacement(this, unk1, unk2, unk3);
}
