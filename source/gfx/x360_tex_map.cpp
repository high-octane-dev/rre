#include "x360_tex_map.hpp"
#include "data_access.hpp"
#include "panic.hpp"

// OFFSET: 0x00410670, STATUS: TODO
TextureMap* X360TexMap::GetTextureMapFromResourceName(char* path, int, unsigned int) {
    // FIXME: This entire implementation is a hack.
    char copy[260]{};
    char mode[] = "rb\0";
    strcpy(copy, path);
    strcat(copy, ".dds");

    int out_res_handle = 0;
    void* out_object = nullptr;
    if (lpDataAccess->GetDataOrFileHandle(copy, mode, &out_res_handle, &out_object, 0) != 0) {
        X360TextureMap* map = new X360TextureMap();
        strcpy(map->texture_filename, copy);
        if (out_res_handle != -1) {
            std::size_t len = lpDataAccess->FSize(out_res_handle);
            std::uint8_t* ptr = new std::uint8_t[len];
            lpDataAccess->FRead(out_res_handle, ptr, len);
            map->LoadViaD3DX(ptr, len);
            lpDataAccess->AttachObject(out_res_handle, map);
            lpDataAccess->FClose(out_res_handle);
            return map;
        }
        else {
            panic("[X360TexMap::GetTextureMapFromResourceName] Failed to acquire resource handle! We have recieved an object instead.");
        }
    }
    else {
        panic("[X360TexMap::GetTextureMapFromResourceName] Failed to acquire either a resource handle or an attached object!");
    }
    return nullptr;
}
