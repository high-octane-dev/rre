#pragma once
#include "game_object.hpp"
#include "containers/container_linked_list.hpp"
#include "containers/dynamic_array.hpp"
#include "string_block_allocator.hpp"
#include "parameter_block.hpp"
#include "action_script.hpp"
#include "action_sequence.hpp"

struct MakeActionInfo {};
struct MakeActionTestInfo {};

class ActionManager : public GameObject
{
public:
	LinkedList<ActionSequence*> action_sequences;
	LinkedList<ActionScript*> action_scripts;
	int unk1;
	int unk2;
	DynamicArray<MakeActionInfo*> action_infos;
	DynamicArray<MakeActionTestInfo*> action_test_infos;
	int unk3;
	int unk4;
	StringBlockAllocator* string_block_allocator;
public:
	virtual int Tick(float) override;

	ActionScript* CreateActionScript(char *, char *, char*);
	ActionScript* CreateActionScript(char *, char *, ParameterBlock*);
	ActionSequence* CreateActionSequence(char*, char*, char*);
	ActionSequence* CreateActionSequence(char*, char*, ParameterBlock*);
};