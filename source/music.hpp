#pragma once
#include <Windows.h>
#include "containers/container_list.hpp"
#include "string_block_allocator.hpp"
#include "game_object.hpp"

enum PlayingState {
	Stopped,
	Paused,
	Playing
};

class Music : GameObject {
public:
	struct Playlist {
		char* name;
		ContainerList<int> songs;
	};
private:
	Playlist* selected_playlist;
	ContainerList<Playlist*> playlists;
	ContainerList<int> unk_list;
	unsigned int unk[2];
	int genre;
	int artist;
	StringBlockAllocator* string_block_allocator;
	unsigned int unk2;
	int current_song;
	unsigned int unk3[4];
	unsigned int max_volume;
	char** genres;
	char** artists;
	struct X360Sound* sound;
	struct X360SoundGroup* sound_group;
	unsigned int unk4;
	float current_elapased_time;
	unsigned int remaining_time;
	unsigned int unk5[2];
	unsigned int unk_volume;
	unsigned int unk6[2];
	unsigned int volume;
	unsigned int unk7;
	unsigned int flags;
	unsigned int unk8[2];
	unsigned int current_track;
	unsigned int unk9[2];
	void* unk10;
	PlayingState previous_state;
	PlayingState current_state;
	unsigned int unk11[13];
	HANDLE unk_lock;
	HANDLE unk_event;
	int unk12;
public:
	virtual ~Music() override;
	virtual int Tick(float dt) override = 0;
	virtual int Pause(int paused) override = 0;
	virtual void CreateNumTracks(int param_1, int track_count) = 0;
	virtual void Play(int song, int param_3) = 0;
	virtual bool IsPlaying() = 0;
	virtual void Repeat(int param_1, int param_2) = 0;
	virtual void Loop(int param_1, int param_2) = 0;
	virtual bool IsPaused() = 0;
	virtual int Stop(float param_1) = 0;
	virtual int SetVolume(int param_1) = 0;
	virtual int RampVolume(int param_1, int param_2, int param_3) = 0;
	virtual int GetCurrentTrack() = 0;
	virtual int Shuffle(int param_1) = 0;
	virtual int ShuffleStartingAt(int param_1, int param_2) = 0;
	virtual float GetCurrentElapsedTime() = 0;
	virtual int GetAdditionalNumberOfJukeboxTracks() = 0;
};

class X360Music : public Music {
public:
	virtual ~X360Music() override;
	virtual int Tick(float dt) override;
	virtual int Pause(int paused) override;
	virtual void CreateNumTracks(int param_1, int track_count) override;
	virtual void Play(int song, int param_3) override;
	virtual bool IsPlaying() override;
	virtual void Repeat(int param_1, int param_2) override;
	virtual void Loop(int param_1, int param_2) override;
	virtual bool IsPaused() override;
	virtual int Stop(float param_1) override;
	virtual int SetVolume(int param_1) override;
	virtual int RampVolume(int param_1, int param_2, int param_3) override;
	virtual int GetCurrentTrack() override;
	virtual int Shuffle(int param_1) override;
	virtual int ShuffleStartingAt(int param_1, int param_2) override;
	virtual float GetCurrentElapsedTime() override;
	virtual int GetAdditionalNumberOfJukeboxTracks() override;
};