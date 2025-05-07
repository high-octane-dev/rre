#pragma once
#include "containers/container_list.hpp"
#include "util/macros.hpp"

class UIStyleSheet {
private:
	ContainerList<void*> color_styles;
	ContainerList<void*> font_styles;
	ContainerList<void*> text_styles;
private:
	void InitializeLists();
public:
	RRE_DISABLE_COPY(UIStyleSheet);
	UIStyleSheet();
	virtual ~UIStyleSheet();
	void CreateFromFile(char*);
	void AddColorStyle(char*, unsigned int);
	void AddFontStyle(char*, char*, char*);
	void AddTextStyle(char*, char*);

	void* GetColorStyle(char*);
	void* GetFontStyle(char*);
	void* GetTextStyle(char*);
};