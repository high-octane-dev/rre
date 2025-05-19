#pragma once
#include "sound.hpp"
#include <math\vector4.hpp>

class X360Sound : public Sound {
public:
	int unk1;
	int unk2;
	float unk3;
	float unk4;
	float unk5;
	int sample;
	int stream_file;
	int channel;
	int sound_group;
	int frequency;
	int sample_rate;
	float unk8;
	int unk9;
	int unk10;
	int base_volume;
	int pan;
	int unk11;
	int iangle;
	int oangle;
	int unk12;
	float unk13;
	int unk14;
	int unk15;
	float unk16;
	float unk17;
	float unk18;
	Vector4 unk19;
	Vector4 unk20;
	Vector4 unk21;
	int unk22;
	int unk23;
	int unk24;
	int unk25;
	int unk26;
	std::uint8_t sound_flags;

	void LoadSoundData();
public:
	virtual void CreateFromFile(const char* file, int param_2, int param_3, SOUND3DBUFFER* param_4, Sound* param_5, int param_6);
	virtual int Play();
	virtual int IsPlaying();
	virtual int Pause(int param_1);
	virtual int Stop();
	virtual void ReleaseSound();
	virtual int GetBaseVolume();
	virtual int SetBaseVolume(int param_1);
	virtual int RampVolume(int param_1, int param_2, int param_3);
	virtual int UnkFunction1();
	virtual int GetSampleRate();
	virtual int UNK_0041a070(int param_1);
	virtual int UNK_0041a0d0(float param_1);
	virtual int Return0(int param_1, int param_2);
	virtual int UnkFunction2();
	virtual int UNK_0041a6f0(int* param_1);
	virtual int UNK_0041a710(float param_1);
	virtual int UnkFunction3();
	virtual int UNK_0041a210(int param_1);
	virtual int UNK_0041a3f0(int* param_1);
	virtual int UNK_0041a480(int* param_1);
	virtual int UNK_0041a510(float param_1, int param_2);
	virtual bool UNK_00419cb0();
	virtual int UNK_0041a340(float param_1);
	virtual int UNK_0041a120(int param_1);
};