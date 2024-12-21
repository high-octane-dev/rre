#pragma once
#include "game_object.hpp"
#include "containers/dynamic_array.hpp"
#include "parameter_block.hpp"

struct ActionSequenceStep {};

class ActionSequence : public GameObject
{
public:
	ActionSequence();

	DynamicArray<ActionSequenceStep*> action_sequence_steps;
	int unk1;
	int unk2;
	//int unk3;
	int unk4;
	//int unk5;
	uint8_t unk6;

public:
	GameObject* Create() override;
	virtual int Tick(float) override;

	void ReadActionScript(char *, ParameterBlock*);
};