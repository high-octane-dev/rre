#pragma
#include <cstdint>
#include "parameter_block.hpp"
#include "ui_container.hpp"
#include "ui_layout.hpp"
#include "ui_page.hpp"
#include "ui_style_sheet.hpp"

class UILayer {
protected:
	char* name;
	int layer_unk;
	UIContainer* container;
	ContainerList<UIComponent*> components;
public:
	UIStyleSheet* style_sheet;
protected:
	std::uint8_t layer_unk2;
public:
	RRE_DISABLE_COPY(UILayer);
	UILayer();
	virtual ~UILayer();
	virtual void CreateFromFile(char*, ParameterBlock*);
	virtual void CreateFromName(char*);
	virtual void Tick(float dt);
	virtual void Render(int);
	virtual void StartLayer();
	virtual void EndLayer();
	virtual void StartComponents();
	virtual void EndComponents();
	virtual void RegisterEventListeners();
	virtual void DeRegisterEventListeners();
	virtual void PageStackStateChanged(PageStackState);
	virtual void SetEnabled(int);
	virtual int IsEnabled();
	virtual void SetVisible(int);
	virtual int IsVisible();
	virtual void SetLayout(UILayout*);
	virtual int FUN_004699c0();
	virtual void Repaint(int);
	virtual int CursorClicked(std::uint32_t u_msg, int button, int x, int y);
	virtual void CursorSelected(std::uint32_t u_msg, int button, int x, int y);

	UIComponent* GetComponent(const char*);
};

#ifdef _M_IX86
static_assert(sizeof(UILayer) == 0x30);
#endif