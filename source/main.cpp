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
float g_TargetFrameRate = 60.0;
char g_ClassName[256]{};
char g_Caption[256]{};
char g_DataPCDirectory[256] = {"e:\\dev_Cars2\\DataPC\\"};
char g_StreamingDataPCDirectory[260]{};
char g_LanguageName[64]{};
char g_LocalizedMovieDirectory[260]{};
char g_NTSCOrPal = 0;
int g_GameInitialized = 0;
int g_TickGame = 1;
int g_EnableEndianSwapping = 0;

// OFFSET: 0x005497e0, STATUS: COMPLETE
void GetPlatformIDAndD3DVersion(unsigned int* direct3dVersion, DWORD* outPlatformId, unsigned int* canUseSoftwareRenderer) {
    HMODULE blade = LoadLibraryA("Blade.dll");
    if (blade == nullptr) {
        *canUseSoftwareRenderer = 0;
    }
    else {
        *canUseSoftwareRenderer = 1;
        FreeLibrary(blade);
    }
    /*
    // doesnt compile for some fucking reason
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
    */
    *outPlatformId = VER_PLATFORM_WIN32_NT;
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

// OFFSET: 0x00619190, STATUS: COMPLETE
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
        char video_mode[260]{};
        config.GetParameter("VideoMode", "NTSC", video_mode, sizeof(video_mode));
        if (_strnicmp(video_mode, "PAL", sizeof(video_mode)) == 0) {
            strncpy(g_LocalizedMovieDirectory, "C\\Movies\\2500\\", sizeof(g_LocalizedMovieDirectory));
            g_NTSCOrPal = 'P';
        }
        else {
            g_NTSCOrPal = 'N';
        }
        config.ReadParameterBlock("LanguageDefinition");
        int definition_count = config.GetNumberOfParameterValues("Definition");
        if (definition_count > 0) {
            config.search.SetResetParameterSearch(0);
            char definition_name[32]{};

            for (std::size_t i = 0; i < definition_count; i++) {
                if (config.GetParameter("Definition", "", definition_name, sizeof(definition_name)) != 0) {
                    LanguageDefinition* language_definition = new LanguageDefinition();
                    snprintf(language_definition->name, sizeof(language_definition->name), "%s", definition_name);
                    lpGame->lang_defs.CLAddItem(language_definition);
                }
                else {
                    break;
                }
            }

            config.search.SetResetParameterSearch(1);

            for (std::size_t i = 0; i < lpGame->lang_defs.Length(); i++) {
                lpGame->lang_defs[i]->Read(&config);
            }
        }

        config.ReadParameterBlock("LanguageConfiguration");
        int configuration_count = config.GetNumberOfParameterValues("Configuration");
        if (configuration_count > 0) {
            config.search.SetResetParameterSearch(0);
            char configuration_name[32]{};
            
            for (std::size_t i = 0; i < definition_count; i++) {
                if (config.GetParameter("Configuration", "", configuration_name, sizeof(configuration_name)) != 0) {
                    LanguageConfiguration* language_configuration = new LanguageConfiguration();
                    snprintf(language_configuration->name, sizeof(language_configuration->name), "%s", configuration_name);
                    lpGame->lang_confs.CLAddItem(language_configuration);
                }
                else {
                    break;
                }
            }

            config.search.SetResetParameterSearch(1);
            config.GetParameter("DefaultConfiguration", "English", lpGame->selected_language_configuration_name, 0x20);
            lpGame->GetLanguageConfiguration(lpGame->selected_language_configuration_name);
            
            for (std::size_t i = 0; i < lpGame->lang_confs.Length(); i++) {
                lpGame->lang_confs[i]->Read(&config);
            }

            config.ReadParameterBlock("LanguageGeneral");
            config.GetParameter("UseDashboardLocale", 0, &lpGame->use_dashboard_locale);
            config.ReadParameterBlock("LanguageTitles");
            config.GetParameter(g_LanguageName, "Cars Mater-National", g_Caption, 0x80);
            
            for (std::size_t i = 0; i < sizeof(g_Caption); i++) {
                if (g_Caption[i] == '_') {
                    g_Caption[i] = ' ';
                }
            }
            lpGame->SetLanguageConfiguration(lpGame->selected_language_configuration_name);
        }
    }
}

// OFFSET: 0x00618d50, STATUS: COMPLETE
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg < WM_QUERYENDSESSION) {
        if (uMsg == WM_CLOSE) {
            lpGame->Terminate();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            return 0;
        }
        switch (uMsg) {
        case WM_MOVE:
        case WM_SIZE:
            if ((g_GameInitialized != 0) && (IsIconic(hwnd) != 0)) {
                lpGame->Deactivate();
                return DefWindowProcA(hwnd, uMsg, wParam, lParam);
            }
            break;
        case WM_ACTIVATE:
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
    else if (uMsg < WM_HSCROLL) {
        if (uMsg == WM_TIMER) {
            if (g_TickGame != 0) {
                if (lpGame->Tick() == 0) {
                    lpGame->Terminate();
                    DestroyWindow(hwnd);
                    PostQuitMessage(0);
                    return 1;
                }
                reinterpret_cast<CarsGame*>(lpGame)->PresentFrame(1);
                return DefWindowProcA(hwnd, WM_TIMER, wParam, lParam);
            }
        }
        else {
            if (uMsg == WM_SETCURSOR) {
                if ((short)lParam == 1) {
                    SetCursor((HCURSOR)0x0);
                    return 1;
                }
                SetCursor(LoadCursorA((HINSTANCE)0x0, (LPCSTR)0x7f00));
                return 1;
            }
            if (uMsg == WM_SYSCOMMAND) {
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

// OFFSET: 0x00619050, STATUS: COMPLETE
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
        return 0;
    }
    SetWindowPos(g_HWND, nullptr, 0, 0, 0, 0, 0x43);
    UpdateWindow(g_HWND);
    if (g_IsWindowed != false) {
        AdjustWindowRect(&wnd_rect, 0xcf0000, 0);
        MoveWindow(g_HWND, 0, 30, wnd_rect.right - wnd_rect.left, wnd_rect.bottom - wnd_rect.top, 1);
    }
    return 1;
}

// OFFSET: 0x006196a0, STATUS: COMPLETE
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
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
    GetCurrentDirectoryA(sizeof(lpGame->install_dir), lpGame->install_dir);

    snprintf(g_DataPCDirectory, sizeof(g_DataPCDirectory), "%s\\DataPC\\", lpGame->install_dir);
    strcpy(g_StreamingDataPCDirectory, g_DataPCDirectory);

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
            MessageBoxA(nullptr, quit_message, g_Caption, 0x10);
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
            BOOL message_available = PeekMessageA(&message, nullptr, 0, 0, 1);
            while (message_available != 0) {
                if (message.message == WM_QUIT) {
                    should_exit = true;
                }
                TranslateMessage(&message);
                DispatchMessageA(&message);
                message_available = PeekMessageA(&message, nullptr, 0, 0, 1);
            }
            if ((!should_exit) && (g_TickGame != 0)) {
                if (lpGame->Tick() == 0) {
                    break;
                }
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

int main() {
    CoInitialize(nullptr);
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}