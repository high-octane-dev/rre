#pragma once
#include "base_object.hpp"

struct SOUND3DBUFFER
{
	int unk1;
	int unk2;
	int unk3;
	float unk4;
	float unk5;
	int unk6;
	float unk7;
	float unk8;
	float unk9;
};

class Sound : public BaseObject {
public:
	std::uint8_t flags;
	int unk1;
	int unk2;
	int unk3;
	float unk4;
	int unk5;
	int unk6;
	char* file_name;
public:
	Sound();
	virtual void CreateFromFile(const char* file, int param_2, int param_3, SOUND3DBUFFER* param_4, Sound* param_5, int param_6) = 0;
	virtual int Play() = 0;
	virtual int IsPlaying() = 0;
	virtual int Pause(int param_1) = 0;
	virtual int Stop() = 0;
	virtual void ReleaseSound() = 0;
	virtual int GetBaseVolume() = 0;
	virtual int SetBaseVolume(int param_1) = 0;
	virtual int RampVolume(int param_1, int param_2, int param_3) = 0;
	virtual int UnkFunction1() = 0;
	virtual int GetSampleRate() = 0;
	virtual int UNK_0041a070(int param_1) = 0;
	virtual int UNK_0041a0d0(float param_1) = 0;
	virtual int Return0(int param_1, int param_2) = 0;
	virtual int UnkFunction2() = 0;
	virtual int UNK_0041a6f0(int *param_1) = 0;
	virtual int UNK_0041a710(float param_1) = 0;
	virtual int UnkFunction3() = 0;
	virtual int UNK_0041a210(int param_1) = 0;
	virtual int UNK_0041a3f0(int *param_1) = 0;
	virtual int UNK_0041a480(int *param_1) = 0;
	virtual int UNK_0041a510(float param_1, int param_2) = 0;
	virtual bool UNK_00419cb0() = 0;
	virtual int UNK_0041a340(float param_1) = 0;
	virtual int UNK_0041a120(int param_1) = 0;

};