#include "game_object.hpp"
#include "dynamic_searchable_object.hpp"

class SearchableGameObject : public DynamicSearchableObject
{
public:
	SearchableGameObject(bool startEnabled);
	GameObject game_object;
	virtual int DoPrepareGeometry();
};