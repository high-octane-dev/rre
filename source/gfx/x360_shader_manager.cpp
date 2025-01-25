#include "globals.hpp"
#include "gfx/x360_shader_manager.hpp"
#include "gfx/emotion_engine.hpp"
#include "gfx/x360_video_card.hpp"

#include <stdio.h>
#include <dxsdk-d3dx/d3dx9.h>
#include <format>

X360ShaderManager* g_lpX360ShaderManager = nullptr;

// OFFSET: 0x00415920, STATUS: COMPLETE
int X360ShaderManager::Unk1(int, int, int, int, int, int) {
    return 0;
}

// OFFSET: 0x004111d0, STATUS: COMPLETE
int X360ShaderManager::Unk2(int, int) {
    return 0;
}

// OFFSET: 0x00415900, STATUS: COMPLETE
X360ShaderManager::X360ShaderManager() {
}

// OFFSET: 0x00415b30, STATUS: COMPLETE
X360ShaderManager::~X360ShaderManager() {
    for (std::size_t i = 0; i < compiled_vertex_shaders.len; i++) {
        reinterpret_cast<IUnknown*>(compiled_vertex_shaders.entries[i].data)->Release();
    }
    compiled_vertex_shaders.len = 0;
    for (std::size_t i = 0; i < compiled_pixel_shaders.len; i++) {
        reinterpret_cast<IUnknown*>(compiled_pixel_shaders.entries[i].data)->Release();
    }
    compiled_pixel_shaders.len = 0;
    FreeSourceCode();
}

// OFFSET: 0x00416150, STATUS: COMPLETE
void X360ShaderManager::Create() {
    char filter[260]{};
    const char* shader_model_dir = g_lpD3DDeviceManager->GetRelativeShaderPath();
    snprintf(filter, sizeof(filter), "%s%s%s%s", g_DataPCDirectory, "C\\Shaders\\", shader_model_dir, "*.hlsl");

    WIN32_FIND_DATAA ffd{};
    HANDLE find = FindFirstFileA(filter, &ffd);

    if (INVALID_HANDLE_VALUE == find) {
        return;
    }

    do {
        auto vs = LoadVertexShader(ffd.cFileName);
        compiled_vertex_shaders.AddEntry(ffd.cFileName, vs);
        auto ps = LoadPixelShader(ffd.cFileName);
        compiled_pixel_shaders.AddEntry(ffd.cFileName, ps);

    } while (FindNextFileA(find, &ffd) != 0);

    FindClose(find);
}

// OFFSET: 0x00415930, STATUS: COMPLETE
void X360ShaderManager::FreeSourceCode() {
    for (std::size_t i = 0; i < shader_source_codes.len; i++) {
        if (shader_source_codes.entries[i].data != nullptr) {
            auto p = *reinterpret_cast<void**>(shader_source_codes.entries[i].data);
            if (p != nullptr) {
                free(p);
            }
            free(shader_source_codes.entries[i].data);
        }
    }
    shader_source_codes.len = 0;
    return;
}

// OFFSET: 0x00415990, STATUS: COMPLETE
IDirect3DVertexShader9* X360ShaderManager::GetVertexShader(const char* name) const {
    auto ret = compiled_vertex_shaders.GetByName(name);
    if (ret == nullptr) {
        return reinterpret_cast<IDirect3DVertexShader9*>(compiled_vertex_shaders.GetByName("360_Default_Release.hlsl"));
    }
    return reinterpret_cast<IDirect3DVertexShader9*>(ret);
}

// OFFSET: 0x004159c0, STATUS: COMPLETE
IDirect3DPixelShader9* X360ShaderManager::GetPixelShader(const char* name) const {
    auto ret = compiled_pixel_shaders.GetByName(name);
    if (ret == nullptr) {
        return reinterpret_cast<IDirect3DPixelShader9*>(compiled_pixel_shaders.GetByName("360_Default_Release.hlsl"));
    }
    return reinterpret_cast<IDirect3DPixelShader9*>(ret);
}

// OFFSET: 0x00415bb0, STATUS: COMPLETE
IDirect3DVertexShader9* X360ShaderManager::LoadVertexShader(const char* name) {
    char full_path[260]{};
    const char* shader_model_dir = g_lpD3DDeviceManager->GetRelativeShaderPath();
    snprintf(full_path, sizeof(full_path), "%s\\C\\Shaders\\%s%s.compiled.vsh", g_DataPCDirectory, shader_model_dir, name);
    FILE* f = fopen(full_path, "rb");
    if (f == nullptr) {
        return nullptr;
    }
    fseek(f, 0, SEEK_END);
    auto size = ftell(f);
    fseek(f, 0, SEEK_SET);
    ID3DXBuffer* buffer = nullptr;
    D3DXCreateBuffer(size, &buffer);
    fread(buffer->GetBufferPointer(), 1, buffer->GetBufferSize(), f);
    fclose(f);

    IDirect3DVertexShader9* shdr = nullptr;
    g_D3DDevice9->CreateVertexShader(reinterpret_cast<DWORD*>(buffer->GetBufferPointer()), &shdr);
    buffer->Release();

    return shdr;
}

// OFFSET: 0x00415e30, STATUS: COMPLETE
IDirect3DPixelShader9* X360ShaderManager::LoadPixelShader(const char* name) {
    char full_path[260]{};
    const char* shader_model_dir = g_lpD3DDeviceManager->GetRelativeShaderPath();
    snprintf(full_path, sizeof(full_path), "%s\\C\\Shaders\\%s%s.compiled.psh", g_DataPCDirectory, shader_model_dir, name);
    FILE* f = fopen(full_path, "rb");
    if (f == nullptr) {
        return nullptr;
    }
    fseek(f, 0, SEEK_END);
    auto size = ftell(f);
    fseek(f, 0, SEEK_SET);
    ID3DXBuffer* buffer = nullptr;
    D3DXCreateBuffer(size, &buffer);
    fread(buffer->GetBufferPointer(), 1, buffer->GetBufferSize(), f);
    fclose(f);

    IDirect3DPixelShader9* shdr = nullptr;
    g_D3DDevice9->CreatePixelShader(reinterpret_cast<DWORD*>(buffer->GetBufferPointer()), &shdr);
    buffer->Release();

    return shdr;
}

