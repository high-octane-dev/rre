#pragma once
#include "ui_page.hpp"
#include "game_text.hpp"

class CarsUIPage : public UIPage {
protected:
	GameText* game_text;
	int unk;
public:
	virtual ~CarsUIPage() override;
};

#ifdef _M_IX86
static_assert(sizeof(CarsUIPage) == 0x48);
#endif