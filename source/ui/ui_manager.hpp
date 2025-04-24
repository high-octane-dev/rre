#pragma once
#include "ui_page.hpp"
#include "ui_layout.hpp"
#include "ui_component.hpp"
#include "ui_layer.hpp"

class UIManager : public GameObject {
public:
public:
	virtual ~UIManager() override;
	virtual int Tick(float dt) override;
	virtual int Render3D(int) override;
	virtual int KeyUpHandler(ProcessNode*, KeyInfo*) override;
	virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;

	virtual UIManager* Create(unsigned int);
	virtual UIPage* LoadPage();
	virtual UILayer* LoadLayer() =0;
	virtual UIComponent* LoadComponent();
	virtual UIComponentLayout* LoadComponentLayout();
	virtual void LoadPages(char*);
	virtual void LoadPageFile(ParameterBlock*);
	virtual void LoadLodFile(ParameterBlock*);
	virtual void FormGameTextName(char*, char*, int);
	virtual UILayout* CreateLayout();
	virtual void SetupCamera();
	virtual void SetupSearchNode();
	virtual void CreateScene(char*);
	virtual int AttachSceneObjects();
};