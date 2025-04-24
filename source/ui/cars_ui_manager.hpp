#pragma once
#include "ui_manager.hpp"
#include "ui_event.hpp"

class CarsUIManager : public UIManager, UIControllerEventListener {
public:
	virtual ~CarsUIManager() override;
};