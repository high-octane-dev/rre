#pragma once
#include "parameter_block.hpp"
class LanguageDefinition {
public:
	char name[32];
	char prefix[32];
	int prefix_index;
public:
	LanguageDefinition();
	void Read(ParameterBlock* file);
};

