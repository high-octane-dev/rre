#pragma once
#include "parameter_block.hpp"

class LoadingIcon
{
public:
	LoadingIcon();
	virtual void Create(char*);
	virtual void Enable();
	virtual void Disable();
	virtual int Load(ParameterBlock *);
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int unk5;
	int unk6;
	int unk7;
	int unk8;
	int unk9;
};