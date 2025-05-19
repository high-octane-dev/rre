#pragma once
#include "Containers/container_list.hpp"
#include "sound.hpp"
#include "music.hpp"

class SoundInterface {
public:
	ContainerList<Sound*> sound_list_1;
	ContainerList<Sound*> sound_list_2;
	std::uint8_t flags;
	//AuralScape* aural_scape
	X360Music* music;
};