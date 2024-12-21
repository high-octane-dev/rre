#pragma once
#include "game_object.hpp"
#include "action_sequence.hpp"
#include "parameter_block.hpp"

class ActionScript : public GameObject
{
public:
	ActionScript();
	ActionSequence* post_action;
	int unk1;
	int unk2;
	char* unk3;
	uint8_t unk4;

public:
	virtual int Tick(float) override;

	int IsPlaying();
	void ReadActionScript(char*, char*);
	void ReadActionScript(char*, ParameterBlock*);
	void ReadNonBodySequence(ParameterBlock*, char*);
	void Reset();
	void Run();
};