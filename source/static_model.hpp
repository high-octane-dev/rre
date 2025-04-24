#pragma once
#include "soultree/soultree_object.hpp"
#include "game_object.hpp"

class StaticModel : GameObject {
public:
	int type;
	struct X360RenderableObject* renderable;
	int start_enabled;
	int unused;
};