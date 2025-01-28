#pragma once
#include "game.hpp"
#include <windows.h>
#include <d3d9.h>

extern unsigned int g_ViewportWidth;
extern unsigned int g_ViewportHeight;
extern int g_ScreenEffectWidth;
extern int g_ScreenEffectHeight;
extern int g_CameraWidth;
extern int g_CameraHeight;
extern int g_ScreenMode;
extern int PlatformGraphics_BackBufferViewport_OffsetX;
extern int PlatformGraphics_BackBufferViewport_OffsetY;
extern float g_UIScaleX;
extern float g_UIScaleY;
extern float g_ViewportFadeWidth;
extern float g_ViewportFadeHeight;
extern float g_AspectRatios[3];

class X360Game : public Game {
public:
	int unused19;
	int unused20;
	int unused17;
	int unused18;
	HWND hwnd;
	char unused21[16];
	IDirect3D9* d3d9;
public:
	X360Game();
	virtual ~X360Game() override;
	virtual int InitializeRenderer(char (&quit_message)[260]) override;
	virtual int PreGameInitialize(DisplayMode*) override;
	virtual int SetBasicRenderStates() override;
	virtual void PrepareFrame() override;
	virtual int KeyUpHandler(ProcessNode*, KeyInfo*) override;
	virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;
	virtual int Terminate() override;
	virtual int SetDisplayMode(DisplayMode*) override;
	virtual RenderTarget* CreateRenderTarget() override;
};

