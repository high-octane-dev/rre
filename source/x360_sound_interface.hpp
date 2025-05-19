#pragma once
#include "sound_interface.hpp"
#include "Containers/string_table.hpp"
#include "math/vector4.hpp"

class X360SoundInterface : public SoundInterface {
public:
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int volume;
	std::uint8_t flags;
	int unk5;
	float unk6;
	float unk7;
	float unk8;
	Vector4 unk9;
	Vector4 unk10;
	Vector4 unk11;
	Vector4 unk12;
	int unk13;
	int unk14;
	int unk15;
	StringTable* audio_string;
	StringTable* streams_jukebox;
};

extern X360SoundInterface* g_lpX360SoundInterface;