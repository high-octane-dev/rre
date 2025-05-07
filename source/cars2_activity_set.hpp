#pragma once
#include "cars2_activity_info.hpp"
#include "util/macros.hpp"

class Cars2ActivitySet {
public:
	char* activity_type_str;
	ActivityType activity_type;
	Cars2ActivityInfo* activities;
	int activities_len;
public:
	RRE_DISABLE_COPY(Cars2ActivitySet);
	Cars2ActivitySet();
	~Cars2ActivitySet();

	Cars2ActivityInfo* GetActivityInfo(char* name);
};