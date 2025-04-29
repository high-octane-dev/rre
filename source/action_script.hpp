#pragma once
#include "game_object.hpp"
#include "action_sequence.hpp"
#include "parameter_block.hpp"

class ActionScript : public GameObject {
public:
	int unk1;
	ActionSequence* post_action;
	int unk2;
	int unk3;
	char* unk4;
	uint8_t unk5;
public:
	ActionScript();
	virtual ~ActionScript() override;
	virtual int Tick(float) override;

	int IsPlaying();
	void ReadActionScript(char*, char*);
	void ReadActionScript(char*, ParameterBlock*);
	void ReadNonBodySequence(ParameterBlock*, char*);
	void Reset();
	void Run();
};