#pragma once
#include "game_object.hpp"
#include "containers/dynamic_array.hpp"
#include "parameter_block.hpp"

struct ActionSequenceStep {};

class ActionSequence : public GameObject {
public:
	DynamicArray<ActionSequenceStep*> action_sequence_steps;
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	char* unk5;
	uint8_t unk6;
public:
	ActionSequence();
	virtual ~ActionSequence() override;
	GameObject* Create() override;
	virtual int Tick(float) override;

	void ReadActionScript(char*, ParameterBlock*);
};