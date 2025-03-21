#include "game_object.hpp"
#include "dynamic_searchable_object.hpp"

class SearchableGameObject : public DynamicSearchableObject, public GameObject {
public:
	SearchableGameObject(bool startEnabled);
	virtual int DoPrepareGeometry();
};