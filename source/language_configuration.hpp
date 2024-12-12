#pragma once
#include "parameter_block.hpp"
class LanguageConfiguration {
public:
	char Name[32];
	int TextLanguageIndex;
	int DialogueLanguageIndex;
	char Flag[32];
	int UseSubtitles;
	int IsDistanceImperial;
	int IsSpeedImperial;
	char TimerHundrethsMarkerFlag;
public:
	LanguageConfiguration();
	void Read(ParameterBlock* file);
};

