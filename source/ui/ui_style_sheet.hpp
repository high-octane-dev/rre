#pragma once
#include "containers/container_list.hpp"

class UIStyleSheet {
private:
	ContainerList<void*> color_styles;
	ContainerList<void*> font_styles;
	ContainerList<void*> text_styles;
private:
	void InitializeLists();
public:
	UIStyleSheet();
	UIStyleSheet(const UIStyleSheet&) = delete;
	UIStyleSheet& operator=(const UIStyleSheet&) = delete;
	virtual ~UIStyleSheet();
	void CreateFromFile(char*);
	void AddColorStyle(char*, unsigned int);
	void AddFontStyle(char*, char*, char*);
	void AddTextStyle(char*, char*);

	void* GetColorStyle(char*);
	void* GetFontStyle(char*);
	void* GetTextStyle(char*);
};