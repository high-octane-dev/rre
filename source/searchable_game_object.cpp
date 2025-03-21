#include "searchable_game_object.hpp"

// OFFSET: 0x005c7dd0, STATUS: COMPLETE
SearchableGameObject::SearchableGameObject(bool startEnabled) : DynamicSearchableObject(), GameObject(startEnabled) {
	searchable_object_flags |= 0x30;
	unk_type = 7;
}

// OFFSET: 0x0055fb10, STATUS: COMPLETE
int SearchableGameObject::DoPrepareGeometry() {
	GameObject::PrepareGeometry();
	return 1;
}
