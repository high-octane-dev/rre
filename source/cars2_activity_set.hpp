#pragma once
#include "cars2_activity_info.hpp"

class Cars2ActivitySet {
public:
	char* activity_type_str;
	ActivityType activity_type;
	Cars2ActivityInfo* activities;
	int activities_len;
public:
	Cars2ActivitySet();
	Cars2ActivitySet(const Cars2ActivitySet&) = delete;
	Cars2ActivitySet& operator=(const Cars2ActivitySet&) = delete;
	~Cars2ActivitySet();

	Cars2ActivityInfo* GetActivityInfo(char* name);
};