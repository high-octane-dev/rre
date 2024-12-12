// Cars2Renderer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <cstdio>
#include <clocale>
#include <algorithm>
#include <cstdlib>

#include <Windows.h>
#include <d3d9.h>
#include <span>
#include "cars_game.hpp"
#include "parameter_block.hpp"
#include "language_configuration.hpp"

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

HINSTANCE g_hInstance;
HWND g_HWND;
HINSTANCE g_hInstanceReal;
HWND g_HWNDReal;
HANDLE g_GameMutex;
int g_IsWindowed = 1;
int g_WindowWidth = 1280;
int g_WindowHeight = 720;
float g_TargetFrameRate = 0.0;
char g_ClassName[256]{};
char g_Caption[256]{};
char g_DataDir[256] = {"e:\\dev_Cars2\\DataPC\\"};
char g_RootDirectory[260]{};
char g_LanguageName[64]{};
char g_LocalizedMovieDirectory[260]{};
char g_NTSCOrPal = 0;
int g_GameInitialized = 0;
int g_TickGame = 1;

void GetPlatformIDAndD3DVersion(unsigned int* direct3dVersion, DWORD* outPlatformId, unsigned int* canUseSoftwareRenderer) {
    HMODULE blade = LoadLibraryA("Blade.dll");
    if (blade == nullptr) {
        *canUseSoftwareRenderer = 0;
    }
    else {
        *canUseSoftwareRenderer = 1;
        FreeLibrary(blade);
    }
    OSVERSIONINFOA version_info{};
    version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    if (!GetVersionExA(&version_info)) {
        *direct3dVersion = 0;
        *outPlatformId = 0;
    }
    else if (version_info.dwPlatformId == 0) {
        *direct3dVersion = 0;
        *outPlatformId = 0;
    }
    else {
        *outPlatformId = version_info.dwPlatformId;
        if ((version_info.dwPlatformId == 2) && (version_info.dwMajorVersion < 4)) {
            *outPlatformId = 0;
        }
        HMODULE d3d9_dll = LoadLibraryA("d3d9.dll");
        if (d3d9_dll != nullptr) {
            FreeLibrary(d3d9_dll);
            IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
            if (d3d9 != nullptr) {
                d3d9->Release();
                *direct3dVersion = DIRECT3D_VERSION;
                return;
            }
        }
        *direct3dVersion = 0;
    }
    return;
}

void ReadConfigIni() {
    ParameterBlock language_ini{};
    if (language_ini.OpenFile("language.ini", 0, -1, nullptr, -1) == 0) {
        strcpy(g_LanguageName, "English");
    }
    else {
        language_ini.ReadParameterBlock("Language");
        language_ini.GetParameter("Name", "English", g_LanguageName, 0x40);
    }
    ParameterBlock config{};
    if (config.OpenFile("config.ini", 0, -1, nullptr, 0xffffffff) != 0) {
        config.ReadParameterBlock("Boot Config");
        char VideoName[260]{};
        config.GetParameter("VideoMode", "NTSC", VideoName, 0x104);
        if (_strnicmp(VideoName, "PAL", 0x104) == 0) {
            strncpy(g_LocalizedMovieDirectory, "C\\Movies\\2500\\", 0x104);
            g_NTSCOrPal = 'P';
        }
        else {
            g_NTSCOrPal = 'N';
        }
        config.ReadParameterBlock("LanguageDefinition");
        int iVar5 = config.GetNumberOfParameterValues("Definition");
        if (0 < iVar5) {
            config.search.SetResetParameterSearch(0);
            char Definition[32]{};
            iVar5 = config.GetParameter("Definition", "", Definition, 0x20);
            while (iVar5 != 0) {
                LanguageDefinition* language_definition = new LanguageDefinition();
                snprintf(language_definition->name, 0x20, "%s", Definition);
                lpGame->lang_defs.CLAddItem(language_definition);
                iVar5 = config.GetParameter("Definition", "", Definition, 0x20);
            }
            config.search.SetResetParameterSearch(1);
            iVar5 = 0;
            if (0 < lpGame->lang_defs.Length()) {
                do {
                    lpGame->lang_defs[iVar5]->Read(&config);
                    iVar5 = iVar5 + 1;
                } while (iVar5 < lpGame->lang_defs.Length());
            }
        }
        config.ReadParameterBlock("LanguageConfiguration");
        iVar5 = config.GetNumberOfParameterValues("Configuration");
        if (0 < iVar5) {
            config.search.SetResetParameterSearch(0);
            char Configuration[32]{};
            iVar5 = config.GetParameter("Configuration", "", Configuration, 0x20);
            while (iVar5 != 0) {
                LanguageConfiguration* language_configuration = new LanguageConfiguration();
                snprintf(language_configuration->Name, 0x20, "%s", Configuration);
                lpGame->lang_confs.CLAddItem(language_configuration);
                iVar5 = config.GetParameter("Configuration", "", Configuration, 0x20);
            }
            config.search.SetResetParameterSearch(1);
            config.GetParameter("DefaultConfiguration", "English", lpGame->selected_language_configuration_name, 0x20);
            lpGame->GetLanguageConfiguration(lpGame->selected_language_configuration_name);
            iVar5 = 0;
            if (0 < lpGame->lang_confs.Length()) {
                do {
                    lpGame->lang_confs[iVar5]->Read(&config);
                    iVar5 = iVar5 + 1;
                } while (iVar5 < lpGame->lang_confs.Length());
            }
            config.ReadParameterBlock("LanguageGeneral");
            config.GetParameter("UseDashboardLocale", 0, &lpGame->use_dashboard_locale);
            config.ReadParameterBlock("LanguageTitles");
            config.GetParameter(g_LanguageName, "Cars Mater-National", g_Caption, 0x80);
            
            for (auto i = 0; i < sizeof(g_Caption); i++) {
                if (g_Caption[i] == '_') {
                    g_Caption[i] = ' ';
                }
            }
            lpGame->SetLanguageConfiguration(lpGame->selected_language_configuration_name);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg < 0x11) {
        if (uMsg == 0x10) {
            lpGame->Terminate();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            return 0;
        }
        switch (uMsg) {
        case 3:
        case 5:
            if ((g_GameInitialized != 0) && (IsIconic(hwnd) != 0)) {
                lpGame->Deactivate();
                return DefWindowProcA(hwnd, uMsg, wParam, lParam);
            }

        case 6:
            if (((short)wParam != 0) && (wParam >> 0x10 == 0)) {
                g_TickGame = 1;
                lpGame->Activate();
                return DefWindowProcA(hwnd, uMsg, wParam, lParam);
            }
            g_TickGame = 0;
            lpGame->Deactivate();
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
        }
    }
    else if (uMsg < 0x114) {
        if (uMsg == 0x113) {
            if (g_TickGame != 0) {
                if (lpGame->Tick() == 0) {
                    lpGame->Terminate();
                    DestroyWindow(hwnd);
                    PostQuitMessage(0);
                    return 1;
                }
                reinterpret_cast<CarsGame*>(lpGame)->PresentFrame(1);
                return DefWindowProcA(hwnd, 0x113, wParam, lParam);
            }
        }
        else {
            if (uMsg == 0x20) {
                if ((short)lParam == 1) {
                    SetCursor((HCURSOR)0x0);
                    return 1;
                }
                SetCursor(LoadCursorA((HINSTANCE)0x0, (LPCSTR)0x7f00));
                return 1;
            }
            if (uMsg == 0x112) {
                switch (wParam & 0xfffffff0) {
                case 0xf100:
                case 0xf140:
                case 0xf170:
                case 0xf190:
                    if (g_TickGame != 0) {
                        return 0;
                    }
                }
            }
        }
    }
    else if ((0x1ff < uMsg) && ((uMsg < 0x203 || (uMsg == 0x204)))) {
        // HandleUIClick(uMsg, wParam, (int)(short)lParam, (int)(short)((unsigned int)lParam >> 0x10));
    }
    return  DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

int InitializeWindow(HINSTANCE param_1) {
    
    WNDCLASSA wnd_class;
    wnd_class.style = 8;
    wnd_class.hIcon = nullptr;
    wnd_class.hCursor = nullptr;
    wnd_class.lpfnWndProc = WndProc;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hInstance = param_1;
    wnd_class.hbrBackground = (HBRUSH)GetStockObject(4);
    wnd_class.lpszMenuName = nullptr;
    wnd_class.lpszClassName = g_ClassName;
    RegisterClassA(&wnd_class);

    RECT wnd_rect{};
    wnd_rect.left = 0;
    wnd_rect.top = 0;
    wnd_rect.right = g_WindowWidth;
    wnd_rect.bottom = g_WindowHeight;
    
    DWORD dwStyle = 0;
    if (g_IsWindowed == false) {
        dwStyle = 0x80080000;
    }
    else {
        dwStyle = 0x10cf0000;
    }
    
    g_HWND = CreateWindowExA(0, g_ClassName, g_Caption, dwStyle, 0, 0, g_WindowWidth, g_WindowHeight, nullptr, nullptr, param_1, nullptr);
    if (g_HWND == nullptr) {
        GetLastError();
        return 0;
    }
    SetWindowPos(g_HWND, nullptr, 0, 0, 0, 0, 0x43);
    UpdateWindow(g_HWND);
    if (g_IsWindowed != false) {
        AdjustWindowRect(&wnd_rect, 0xcf0000, 0);
        MoveWindow(g_HWND, 0, 30, wnd_rect.right - wnd_rect.left, wnd_rect.bottom - wnd_rect.top, 1);
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    unsigned int direct3DVersion = 0;
    DWORD platformID = 0;
    unsigned int canUseSwRenderer = 0;

    g_hInstance = hInstance;
    if (strstr(lpCmdLine, "-windowed") != nullptr) {
        g_IsWindowed = true;
    }
    
    GetPlatformIDAndD3DVersion(&direct3DVersion, &platformID, &canUseSwRenderer);
    
    MEMORYSTATUSEX memory_status_ex;
    memset(&memory_status_ex, 0, 0x40);
    memory_status_ex.dwLength = 0x40;
    GlobalMemoryStatusEx(&memory_status_ex);

    if ((platformID != 1) && (platformID != 2)) {
         MessageBoxA(nullptr, nullptr, "This program cannot be used by this operating system", MB_ICONERROR);
        _exit(0);
    }
    if (direct3DVersion < 0x900) {
        MessageBoxA(nullptr, nullptr, "You must use DirectX version 9.0 or higher", MB_ICONERROR);
        _exit(0);
    }
    /*
    uVar6 = memory_status_ex.ullTotalPhys._4_4_ + (uint)(-0x2000000 < memory_status_ex.ullTotalPhys._0_4_);
    if ((memory_status_ex.ullTotalPageFile._4_4_ <= uVar6) &&
        ((memory_status_ex.ullTotalPageFile._4_4_ < uVar6 ||
            (memory_status_ex.ullTotalPageFile._0_4_ < memory_status_ex.ullTotalPhys._0_4_ + 0x2000000)))) {
        MessageBoxA(nullptr, nullptr, "This program cannot be used without enabling Virtual Memory", 0x10);
        _exit(0);
    }*/

    strcpy(g_ClassName, "Rainbow Studios");
    strcpy(g_Caption, "Cars");
    setlocale(0, "English");
    GetCurrentDirectoryA(0x104, lpGame->install_dir);

    std::span install_dir_view(lpGame->install_dir);
    auto it = std::find(install_dir_view.rbegin(), install_dir_view.rend(), '\\');
    if (it != install_dir_view.rend()) {
        auto index = std::distance(it, install_dir_view.rend()) - 1;
        lpGame->install_dir[index] = 0;
    }

    vsnprintf(g_DataDir, 256, "%s\\DataPC\\", lpGame->install_dir);
    strcpy(g_RootDirectory, g_DataDir);

    ReadConfigIni();

    HANDLE mutex = CreateMutexA(nullptr, 1, "Cars Mater-National Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        mutex = nullptr;
    }
    g_GameMutex = mutex;
    if (mutex == nullptr) {
        HWND existing_window = FindWindowA(g_ClassName, g_Caption);
        if (existing_window != nullptr) {
            ShowWindow(existing_window, 9);
        }
        _exit(0);
    }

    if (lpGame->PreWindowInitialize() == 0) {
        _exit(0);
    }
    int return_value = 0;
    if (InitializeWindow(hInstance) == 0) {
        return_value = 1;
    }
    else {
        g_HWNDReal = g_HWND;
        g_hInstanceReal = hInstance;
        if (lpGame->PreGameInitialize(nullptr) == 0) {
            _exit(0);
        }

        char quit_message[260]{};
        if (lpGame->InitializeRenderer(quit_message) == 0) {
        
            DestroyWindow(g_HWND);
            if ((lpGame->flags & 2) != 0) {
                ShowCursor(1);
            }
            MessageBoxA(nullptr, quit_message + 4, g_Caption, 0x10);
            PostQuitMessage(0);
        }
        if (lpGame->Initialize() == 0) {
            DestroyWindow(g_HWND);
            if ((lpGame->flags & 2) != 0) {
                ShowCursor(1);
            }
            PostQuitMessage(0);
        }

        g_GameInitialized = 1;
        bool should_exit = false;
        MSG message{};
        do {
            BOOL message_available = PeekMessageA(&message, (HWND)0x0, 0, 0, 1);
            while (message_available != 0) {
                if (message.message == 0x12) {
                    should_exit = true;
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
                message_available = PeekMessageA(&message, nullptr, 0, 0, 1);
            }
            if ((!should_exit) && (g_TickGame != 0)) {
                if (lpGame->Tick() == 0) break;
                reinterpret_cast<CarsGame*>(lpGame)->PresentFrame(1);
            }
            Sleep(1);
        } while (!should_exit);
        if (g_GameMutex != nullptr) {
            ReleaseMutex(g_GameMutex);
        }
        g_GameMutex = nullptr;
        return_value = message.wParam;
    }
    return return_value;
}