#include "searchable_game_object.hpp"

// OFFSET: 0x005c7dd0, STATUS: COMPLETE
SearchableGameObject::SearchableGameObject(bool startEnabled) : game_object(startEnabled)
{
	flags = flags & 0x8f;
	unk_type = '\t';
	search_time_stamp = 0;
	placement = nullptr;
	unk_type = '\a';
}

// OFFSET: 0x0055fb10, STATUS: COMPLETE
int SearchableGameObject::DoPrepareGeometry()
{
	game_object.PrepareGeometry();
	return 1;
}
