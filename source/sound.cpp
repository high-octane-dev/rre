#include "sound.hpp"

// OFFSET: 0x005749b0, STATUS: COMPLETE
Sound::Sound()
{
	unk4 = 0.0f;
	unk5 = 0;
	ref_count = 1;
	flags = 0;
	unk1 = 0;
	unk2 = 0;
	unk3 = 0;
	unk6 = 0;
	file_name = nullptr;
}
