#pragma once
#include "parameter_block.hpp"
class LanguageConfiguration {
public:
	char name[32];
	int text_language_index;
	int dialogue_language_index;
	char flag[32];
	int use_subtitles;
	int is_distance_imperial;
	int is_speed_imperial;
	char timer_hundreths_marker_flag;
public:
	LanguageConfiguration();
	void Read(ParameterBlock* file);
};

