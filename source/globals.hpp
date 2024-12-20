#pragma once
#include <wtypes.h>

extern HINSTANCE g_hInstance;
extern HWND g_HWND;
extern HINSTANCE g_hInstanceReal;
extern HWND g_HWNDReal;
extern HANDLE g_GameMutex;
extern int g_IsWindowed;
extern int g_WindowWidth;
extern int g_WindowHeight;
extern float g_TargetFrameRate;
extern char g_ClassName[256];
extern char g_Caption[256];
extern char g_DataPCDirectory[256];
extern char g_StreamingDataPCDirectory[260];
extern char g_LanguageName[64];
extern char g_LocalizedMovieDirectory[260];
extern char g_NTSCOrPal;
extern int g_GameInitialized;
extern int g_TickGame;
