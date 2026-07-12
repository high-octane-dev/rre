#include "util/rsstring_util.hpp"
#include "x360_sound.hpp"
#include "x360_sound_interface.hpp"
#include <cstring>
#include <debugapi.h>
#include "cars_game.hpp"
#include "containers\string_table.hpp"
#include "globals.hpp"
#include "bass.h"

char g_AudioDirectory[260] = "C\\Audio\\";

// OFFSET: 0x00419d10, STATUS: WIP
void X360Sound::LoadSoundData()
{
	char local_108[256];

	if ((sound_flags & 32) != NULL)
	{
		return;
	}
	
	RSStringUtil::Ssnprintf(local_108, 0x104, "%s%s", g_DataPCDirectory, file_name);
	//RSStringUtil::ChangeExtension(local_108, ".ogg");

	if ((sound_flags & 16) != NULL)
	{
		sample = BASS_SampleLoad(0, local_108, 0, 0, 1, (sound_flags & 1) * 4 | flags & 8);
	
		if (BASS_SampleGetInfo(sample, NULL))
		{
			sample_rate = (int)local_108;
		}
	}
	else
	{
		stream_file = BASS_StreamCreateFile(0, local_108, 0, 0, (sound_flags & 1) * 4);
	}
	sound_flags = sound_flags | 32;
	return;
}

// OFFSET: 0x0041b720, STATUS: WIP
void X360Sound::CreateFromFile(const char* file, int param_2, int param_3, SOUND3DBUFFER* param_4, Sound* param_5, int param_6)
{
	char* pcVar2;
	char local_108[256];

	if (strstr(file, "windmill03_loop") != NULL)
	{
		DebugBreak();
	}

	flags = param_2;
	unk1 = param_3;

	if (param_2 < '\0')
	{
		sound_flags = sound_flags | 1;
	}

	if ((param_2 & 64) != 0)
	{
		sound_flags = sound_flags | 8;
	}

	if ((param_2 & 2) != 0)
	{
		sound_flags = sound_flags | 16;
	}

	pcVar2 = g_AudioDialogueDirectory;

	int i;

	if ((param_2 & 16) == 0)
	{
		if (param_6 == 0)
		{
			pcVar2 = g_AudioDirectory;
		}
		RSStringUtil::Ssnprintf(local_108, 0x104, "%s%s", pcVar2, file);
		//RSStringUtil::ChangeExtension(local_108, ".ogg");
		//RSStringUtil::RemoveExtension(local_108);

		if (g_lpX360SoundInterface->audio_string->GetIndex(local_108) == -1)
		{
			pcVar2 = g_AudioDialogueDirectory;

			if (param_6 != 0)
			{
				pcVar2 = g_AudioDirectory;
			}
			RSStringUtil::Ssnprintf(local_108, 0x104, "%s%s", pcVar2, file);
			//RSStringUtil::ChangeExtension(local_108, ".ogg");
			//RSStringUtil::RemoveExtension(local_108);
		}
		//file_name = g_lpX360SoundInterface->audio_string->string_entries[i].string;
	}
	else
	{
		if (param_6 == 0)
		{
			pcVar2 = g_AudioDialogueDirectory;
		}
		RSStringUtil::Ssnprintf(local_108, 0x104, "%s%s", pcVar2, file);
		//RSStringUtil::ChangeExtension(local_108, ".ogg");

		if (g_lpX360SoundInterface->streams_jukebox->GetIndex(local_108) == -1)
		{
			pcVar2 = g_AudioDialogueDirectory;
			if (param_6 != 0)
			{
				pcVar2 = g_AudioDirectory;
			}
			RSStringUtil::Ssnprintf(local_108, 0x104, "%s%s", pcVar2, file);
			//RSStringUtil::ChangeExtension(local_108, ".ogg");
			//iVar3 = StringTable::GetIndex(lpX360SoundInterface->streams_jukebox,local_108);
		}
		//file_name = g_lpX360SoundInterface->streams_jukebox->string_entries[i].string;
		if (i < 0)
		{
			unk4 = 0.0f;
		}
		else
		{
		
			//unk4 = (float)g_lpX360SoundInterface->streams_jukebox->string_entries[i].data;
		}
	}

	LoadSoundData();

	if ((flags & 8) != 0)
	{
		if (param_4 == NULL)
		{
			if (sound_group == 0)
			{
				UNK_0041a120(100);
			}
			else
			{
				UNK_0041a120(0);
			}
		}
	}
}

// OFFSET: 0x00419e00, STATUS: WIP
int X360Sound::Play()
{
	if ((sound_flags & 16) != 0)
	{
		BASS_ChannelPlay(stream_file, 1);
		return 1;
	}

	if (BASS_SampleGetChannels(sample, NULL) < 1)
	{
		channel = BASS_SampleGetChannel(sample, 0);
	}

	BASS_ChannelSetAttributes(channel, frequency, unk10, pan);
	unk3 = 0.0f;
	BASS_ChannelPlay(channel, 1);
	return 1;
}

// OFFSET: 0x00419ff0, STATUS: TODO
int X360Sound::IsPlaying()
{
	return 0;
}

// OFFSET: 0x0041a270, STATUS: TODO
int X360Sound::Pause(int param_1)
{
	return 1;
}

// OFFSET: 0x00419e80, STATUS: COMPLETE
int X360Sound::Stop()
{
	int iVar1;

	if (IsPlaying() == 0)
	{
		return 1;
	}

	if ((sound_flags & 16) == 0)
	{
		if (sample == 0)
		{
			return 1;
		}

		if (BASS_SampleGetChannels(sample, NULL) < 0)
		{
			return 1;
		}
		iVar1 = channel;
	}
	else
	{
		iVar1 = stream_file;
	}

	if (iVar1 != 0)
	{
		BASS_ChannelStop(iVar1);
		channel = 0;
	}
	return 1;
}

// OFFSET: 0x00419ee0, STATUS: TODO
void X360Sound::ReleaseSound()
{
	return;
}

// OFFSET: 0x00419cc0, STATUS: COMPLETE
int X360Sound::GetBaseVolume()
{
	return base_volume;
}

// OFFSET: 0x00419ce0, STATUS: COMPLETE
int X360Sound::SetBaseVolume(int param_1)
{
	base_volume = param_1;
	UNK_0041a120(param_1);
	return 1;
}

// OFFSET: 0x0041a1a0, STATUS: COMPLETE
int X360Sound::RampVolume(int param_1, int param_2, int param_3)
{
	int iVar1;
	float fVar2;

	iVar1 = unk10;
	unk1 = iVar1;
	fVar2 = param_3 / 1000.0f;
	unk3 = fVar2;
	unk4 = 0.0f;

	if (param_3 != 0)
	{
		unk5 = (iVar1 * (param_1 / 100.0) - iVar1) / fVar2;
	}
	unk5 = (param_1 - iVar1) / fVar2;
	return 1;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360Sound::UnkFunction1()
{
	return 0;
}

// OFFSET: 0x0041af90, STATUS: COMPLETE
int X360Sound::GetSampleRate()
{
	return sample_rate;
}

// OFFSET: 0x0041a070, STATUS: WIP
int X360Sound::UNK_0041a070(int param_1)
{
	DWORD dVar1;

	/*
	if (((lpX360SoundInterface->SoundInterface).flags & 1) == 0) {
		return 1;
	}
	*/

	frequency = param_1;

	if ((sound_flags & 16) == 0)
	{
		if (sample == 0)
		{
			return 1;
		}
		dVar1 = BASS_SampleGetChannels(sample, NULL);
		if (dVar1 < 1)
		{
			return 1;
		}
		dVar1 = channel;
	}
	else
	{
		dVar1 = stream_file;
	}
	if (dVar1 != 0)
	{
		BASS_ChannelSetAttributes(dVar1, frequency, -1, -101);
	}
	return 1;
}

// OFFSET: 0x0041a0d0, STATUS: COMPLETE
int X360Sound::UNK_0041a0d0(float param_1)
{
	float local_8;

	unk8 = param_1;

	if (sample_rate < 0)
	{
		sample_rate = sample_rate + 4.2949673e+09f;
	}

	local_8 = round(sample_rate * param_1);

	return UNK_0041a070(local_8);
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360Sound::Return0(int param_1, int param_2)
{
	return 0;
}

// OFFSET: 0x0041af90, STATUS: COMPLETE
int X360Sound::UnkFunction2()
{
	return sample_rate;
}

// OFFSET: 0x0041a6f0, STATUS: TODO
int X360Sound::UNK_0041a6f0(int* param_1)
{
	return 1;
}

// OFFSET: 0x0041a710, STATUS: TODO
int X360Sound::UNK_0041a710(float param_1)
{
	return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
int X360Sound::UnkFunction3()
{
	return 0;
}

// OFFSET: 0x0041a210, STATUS: TODO
int X360Sound::UNK_0041a210(int param_1)
{
	return 0;
}

// OFFSET: 0x0041a3f0, STATUS: TODO
int X360Sound::UNK_0041a3f0(int* param_1)
{
	return 0;
}

// OFFSET: 0x0041a480, STATUS: TODO
int X360Sound::UNK_0041a480(int* param_1)
{
	return 0;
}

// OFFSET: 0x0041a510, STATUS: TODO
int X360Sound::UNK_0041a510(float param_1, int param_2)
{
	return 0;
}

// OFFSET: 0x00419cb0, STATUS: TODO
bool X360Sound::UNK_00419cb0()
{
	return false;
}

// OFFSET: 0x0041a340, STATUS: TODO
int X360Sound::UNK_0041a340(float param_1)
{
	return 0;
}

// OFFSET: 0x0041a120, STATUS: TODO
int X360Sound::UNK_0041a120(int param_1)
{
	return 0;
}
