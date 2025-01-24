#include <cstddef>
#include "x360_shader.hpp"
#include "x360_shader_manager.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
X360PixelShader::X360PixelShader() {
    inner = nullptr;
}

// OFFSET: INLINE, STATUS: COMPLETE
X360PixelShader::~X360PixelShader() {
    inner = nullptr;
}

// OFFSET: 0x004114b0, STATUS: COMPLETE
bool X360PixelShader::Create(const char* _name, int unused) {
    if (inner != nullptr) {
        inner->Release();
        inner = nullptr;
    }

    inner = lpX360ShaderManager->GetPixelShader(_name);
    if (inner != nullptr) {
        inner->AddRef();
    }
    return inner != nullptr;
}

// OFFSET: 0x0042bdb0, STATUS: COMPLETE
X360VertexShader::X360VertexShader() {
    inner[0] = nullptr;
    inner[1] = nullptr;
}

// OFFSET: 0x00410f90, STATUS: COMPLETE
X360VertexShader::~X360VertexShader() {
    inner[0] = nullptr;
    inner[1] = nullptr;
}

// OFFSET: 0x00410fa0, STATUS: COMPLETE
bool X360VertexShader::Create(const char* _name, int index) {
    strcpy_s(name, _name);
    for (std::size_t i = 0; i < 260; i++) {
        if (name[i] == 0) break;
        name[i] = tolower(name[i]);
    }

    if (inner[index] != nullptr) {
        inner[index]->Release();
    }

    inner[index] = lpX360ShaderManager->GetVertexShader(_name);
    if (inner[index] != nullptr) {
        inner[index]->AddRef();
    }
    return inner[index] != nullptr;
}