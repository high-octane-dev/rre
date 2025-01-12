#pragma once
#include "game.hpp"
#include <windows.h>
#include <d3d9.h>

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
	virtual ~X360Game() override;
	virtual int InitializeRenderer(char*) override;
	virtual int PreGameInitialize(DisplayMode*) override;
	virtual int SetBasicRenderStates() override;
	virtual void PrepareFrame() override;
	virtual int KeyUpHandler(ProcessNode*, KeyInfo*) override;
	virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;
	virtual int Terminate() override;
	virtual int SetDisplayMode(DisplayMode*) override;
	virtual RenderTarget* CreateRenderTarget() override;
};

