#pragma once
#include "game_object.hpp"
#include "cars_ui_manager.hpp"

class CarsUI : public GameObject {
public:
	CarsUIManager* manager;
	int unused0;
	int unused1;
	std::uint8_t unused2;
	int padding[15];
	int unused3;
public:
	CarsUI();
	CarsUI(const CarsUI&) = delete;
	CarsUI& operator=(const CarsUI&) = delete;
	virtual ~CarsUI() override;
	virtual GameObject* Create() override;
	virtual int Tick(float dt) override;
	void LoadCommon(int);
	void LoadGroup(char*, char*, int);
	void LoadLocalization();
};