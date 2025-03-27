#include <zlib.h>
#include "data_access.hpp"
#include "file_io.hpp"
#include "panic.hpp"

/*
Looks like devices can be marked with flag 0x4000 to denote that an entire .res file is loaded directly into memory.
*/

DataAccess* lpDataAccess = nullptr;
BlockAllocator* lpVirtualFileAllocator = nullptr;
constexpr std::ptrdiff_t VirtualSectorSize = 0x6000;
constexpr std::ptrdiff_t VirtualSectorMarkerSize = 1;

extern int g_EnableEndianSwapping;

// OFFSET: 0x00548d10, STATUS: TODO
std::uint8_t GenerateKeyXOR(char* file_path) {
    // Not a single .res file in ANY of these games uses this stupid encrpytion scheme anyway, so this does not matter at all.
    return 0x55;
}

// OFFSET: 0x00548e10, STATUS: COMPLETE
void DecryptXOR(void* buffer, std::size_t buffer_len, std::uint8_t key) {
    std::uint8_t* current = reinterpret_cast<std::uint8_t*>(buffer);
    std::uint8_t* end = current + (buffer_len - 1);
    std::uint8_t encrypted_byte = 0;
    std::uint8_t decrypted_byte = 0;

    if (buffer_len > 1) {
        do {
            encrypted_byte = *current;
            decrypted_byte = encrypted_byte ^ key;
            key += encrypted_byte;
            *current = decrypted_byte;
            current++;
        } while (current != end);
    }

    encrypted_byte = *current;
    decrypted_byte = encrypted_byte ^ key;
    key += encrypted_byte;
    *current = decrypted_byte;
}

// OFFSET: 0x005ec450, STATUS: COMPLETE
DataAccess::DataAccess() : BaseObject() {
    flags = 0;
    number_of_devices_in_list = 0;
    device_list = nullptr;
    file_list = nullptr;
    file_lookup_list = nullptr;
    file_lookup_list_num_slots = 0;
    string_block_allocator = nullptr;
    num_device_cache_pages = 8;
    (device_cache).cache_buffer = nullptr;
    (device_cache).size = 0;
    (device_cache).sector_size_list = nullptr;
    (device_cache).page_sizes = nullptr;
    next_available_device = -1;
    last_get_data_or_file_handle = -1;
    next_available_file_cache_id = -1;
    (device_cache).device_id = -1;
    (device_cache).first_sector = -1;
    (device_cache).last_sector = -1;
    first_device_stow = (int*)&active_device;
    last_device_stow = device_stow + 2;
    file_cache_list[0].offset = -VirtualSectorSize;
    file_cache_list[1].offset = -VirtualSectorSize;
    file_cache_list[2].offset = -VirtualSectorSize;
    file_cache_list[3].offset = -VirtualSectorSize;
    file_cache_list[4].offset = -VirtualSectorSize;
    file_cache_list[5].offset = -VirtualSectorSize;
    file_cache_list[6].offset = -VirtualSectorSize;
    file_cache_list[7].offset = -VirtualSectorSize;
    file_cache_list[0].cache_data = nullptr;
    file_cache_list[0].resource_handle = -2;
    file_cache_list[1].cache_data = nullptr;
    file_cache_list[1].resource_handle = -2;
    file_cache_list[2].cache_data = nullptr;
    file_cache_list[2].resource_handle = -2;
    file_cache_list[3].cache_data = nullptr;
    file_cache_list[3].resource_handle = -2;
    file_cache_list[4].cache_data = nullptr;
    file_cache_list[4].resource_handle = -2;
    file_cache_list[5].cache_data = nullptr;
    file_cache_list[5].resource_handle = -2;
    file_cache_list[6].cache_data = nullptr;
    file_cache_list[6].resource_handle = -2;
    file_cache_list[7].cache_data = nullptr;
    file_cache_list[7].resource_handle = -2;
    unk1 = 0;
    unk2 = 0;
    unk_vtable = nullptr;
    Initialize(8, 0xc0, 0x100000, 8, -1);
}

// OFFSET: 0x005ec5a0, STATUS: COMPLETE
DataAccess::~DataAccess() {
    FreeSystemResources();
}

// OFFSET: 0x00580ed0, STATUS: COMPLETE
int DataAccess::ActivateDevice(VirtualDataDevice* device, int offset) {
    if ((device->flag_data & 0x4000U) == 0) {
        if (*first_device_stow != device->id) {
            bool swapped_devices = false;
            int* current_device = first_device_stow;
            do {
                current_device++;
                if (*current_device == device->id) {
                    do {
                        *current_device = *(current_device - 1);
                        current_device--;
                    } while (current_device > first_device_stow);
                    *first_device_stow = device->id;
                    current_device = last_device_stow;
                    swapped_devices = true;
                }
            } while (current_device != last_device_stow);
            if (!swapped_devices) {
                if (*last_device_stow != -1) {
                    VirtualDataDevice* close_device = &device_list[*last_device_stow];
                    if ((close_device->flag_data & 0x12) == 0 && close_device->file_pointer != nullptr) {
                        fclose(close_device->file_pointer);
                        close_device->file_pointer = nullptr;
                    }
                }
                current_device = last_device_stow;
                do {
                    *current_device = *(current_device - 1);
                    current_device--;
                } while (current_device > first_device_stow);
                *first_device_stow = device->id;
            }
        }
        if ((device->flag_data & 0x100U) != 0) {
            (this->device_cache).first_sector = -1;
            (this->device_cache).last_sector = -1;
            (this->device_cache).device_id = device->id;
        }
        if (device->file_pointer == nullptr) {
            device->file_pointer = FOPEN(device->file_name, "rb");
            if (device->file_pointer == nullptr) {
                return 0;
            }
        }
        device->offset = offset;
    }
    return 1;
}

// OFFSET: 0x005c09e0, STATUS: COMPLETE
int DataAccess::AddFile(char* file_name, int device_id, int start_offset, int size, int* handle) {
    char lowercase_file_name[260]{};
    if (file_name == nullptr) {
        return 0;
    }
    for (std::size_t i = 0; i < sizeof(lowercase_file_name); i++) {
        if (file_name[i] == 0) {
            break;
        }
        lowercase_file_name[i] = std::tolower(file_name[i]);
    }
    VirtualDataFile* file = nullptr;
    int already_present = file_list->Lookup(file_name, &file);

    int new_cache_id = 0;
    int new_resource_handle = 0;
    void* new_resource_data = nullptr;

    if (already_present == 0) {
        int old_len = GrowFileLookupList();
        if (old_len == -1) {
            return 0;
        }
        int did_allocate_new_block = 0;
        file = reinterpret_cast<VirtualDataFile*>(lpVirtualFileAllocator->AllocBlock(&did_allocate_new_block));
        file->file_name = string_block_allocator->StringBlockAllocator_AllocStringByLength(strlen(lowercase_file_name), 0);
        strcpy(file->file_name, lowercase_file_name);
        file_list->CHTAdd(file->file_name, file);
        file_lookup_list[old_len] = file;
        new_resource_handle = old_len;
        new_resource_data = nullptr;
    }
    else {
        new_resource_handle = file->resource_handle;
        new_resource_data = file->resource_data;
        if (file->device_id != device_id) {
            new_cache_id = -1;
        }
    }
    file->flag_data = 0;
    file->device_id = device_id;
    file->start_offset = start_offset;
    file->current_offset = 0;
    file->size = size;
    file->cache_id = new_cache_id;
    file->resource_data = new_resource_data;
    file->resource_handle = new_resource_handle;
    if (handle != nullptr) {
        *handle = new_resource_handle;
    }
    return 1;
}

// OFFSET: 0x005493d0, STATUS: COMPLETE
int DataAccess::AllocateDeviceCache(VirtualDeviceCache* _device_cache, int size_bytes) {
    std::size_t bytes_per_page = static_cast<std::size_t>(std::round(size_bytes / static_cast<float>(num_device_cache_pages)));
    _device_cache->size = 0;
    int success = 1;
    if (_device_cache->cache_buffer == nullptr) {
        _device_cache->cache_buffer = new std::uint8_t* [num_device_cache_pages];
    }
    if (_device_cache->page_sizes == nullptr) {
        _device_cache->page_sizes = new std::uint32_t [num_device_cache_pages];
    }
    for (std::size_t i = 0; i < num_device_cache_pages; i++) {
        _device_cache->cache_buffer[i] = reinterpret_cast<std::uint8_t*>(malloc(bytes_per_page));
        if (i == 0 && *_device_cache->cache_buffer == nullptr) {
            *_device_cache->page_sizes = bytes_per_page;
            success = 0;
        }
        if (_device_cache->cache_buffer[i] != nullptr) {
            _device_cache->size += bytes_per_page;
            _device_cache->page_sizes[i] = bytes_per_page;
        }
    }
    return success;
}

// OFFSET: 0x005492f0, STATUS: COMPLETE
int DataAccess::AllowBundledAccessOnly(int should_allow_only_bundled_access) {
    unsigned int was_allowed = this->flags & 0x2000;
    if (should_allow_only_bundled_access == 0) {
        if ((this->flags & 0x2000) != 0) {
            this->flags = this->flags & 0xFFFFDFFF;
        }
    }
    else if ((this->flags & 0x2000) == 0) {
        this->flags = this->flags | 0x2000;
        return 0;
    }
    return was_allowed;
}

// OFFSET: 0x00549330, STATUS: COMPLETE
int DataAccess::AttachObject(int resource_handle, void* object) {
    this->file_lookup_list[resource_handle]->resource_data = object;
    return 1;
}

// OFFSET: 0x00549530, STATUS: COMPLETE
void DataAccess::ClearDeviceCache(VirtualDeviceCache* cache) {
    if (cache->cache_buffer != nullptr && cache->page_sizes != nullptr && this->num_device_cache_pages > 0) {
        for (std::size_t page_index = 0; page_index < this->num_device_cache_pages; page_index++) {
            if (cache->cache_buffer[page_index] != nullptr) {
                memset(cache->cache_buffer[page_index], 0, cache->page_sizes[page_index]);
            }
        }
    }
}

// OFFSET: 0x00549610, STATUS: COMPLETE
void DataAccess::CopyDeviceCache(VirtualDeviceCache* _device_cache, void* dst, std::ptrdiff_t byte_offset) {
    std::uint32_t page_index = byte_offset / *_device_cache->page_sizes;
    std::uint32_t offset_within_page = byte_offset;
    for (std::uint32_t i = 0; i < page_index; i++) {
        offset_within_page -= _device_cache->page_sizes[i];
    }

    if (_device_cache->page_sizes[page_index] < offset_within_page + VirtualSectorSize) {
        std::uint8_t local_buffer[VirtualSectorSize]{};
        std::uint8_t* buffer_ptr = local_buffer;
        size_t remaining = VirtualSectorSize;

        do {
            size_t chunk_size = std::min(_device_cache->page_sizes[page_index] - offset_within_page, remaining);
            memcpy(buffer_ptr, _device_cache->cache_buffer[page_index] + offset_within_page, chunk_size);

            buffer_ptr += chunk_size;
            remaining -= chunk_size;
            page_index ++;
            offset_within_page = 0;
        } while (remaining > 0);

        memcpy(dst, local_buffer, VirtualSectorSize);
    }
    else {
        memcpy(dst, _device_cache->cache_buffer[page_index] + offset_within_page, VirtualSectorSize);
    }
}

// OFFSET: 0x005813b0, STATUS: COMPLETE
void DataAccess::DecompressDeviceCache(VirtualDeviceCache* _device_cache, void* uncompressed, std::size_t compressed_len, std::ptrdiff_t byte_offset) {
    std::uint32_t page_index = byte_offset / *_device_cache->page_sizes;
    std::size_t remaining_compressed_len = compressed_len;
    std::uint8_t temp_buffer[VirtualSectorSize]{};
    for (std::uint32_t i = 0; i < page_index; ++i) {
        byte_offset -= _device_cache->page_sizes[i];
    }
    
    if (_device_cache->page_sizes[page_index] < byte_offset + compressed_len) {
        std::uint8_t* temp_ptr = temp_buffer;

        while (remaining_compressed_len > 0) {
            size_t page_remaining = _device_cache->page_sizes[page_index] - byte_offset;
            size_t copy_size = (remaining_compressed_len < page_remaining) ? remaining_compressed_len : page_remaining;

            memcpy(temp_ptr, reinterpret_cast<void*>(_device_cache->cache_buffer[page_index] + byte_offset), copy_size);
            temp_ptr += copy_size;
            remaining_compressed_len -= copy_size;
            page_index++;
            byte_offset = 0;
        }

        z_stream zlib_stream = {};
        zlib_stream.next_in = temp_buffer;
        zlib_stream.avail_in = sizeof(temp_buffer);
        zlib_stream.next_out = reinterpret_cast<std::uint8_t*>(uncompressed);
        zlib_stream.avail_out = VirtualSectorSize;
        zlib_stream.zalloc = nullptr;
        zlib_stream.zfree = nullptr;
        inflateInit_(&zlib_stream, "1.0", sizeof(z_stream));
        inflate(&zlib_stream, Z_FINISH);
        inflateEnd(&zlib_stream);
    }
    else {
        z_stream zlib_stream = {};
        memset(&zlib_stream, 0, sizeof(z_stream));
        zlib_stream.avail_in = VirtualSectorSize;
        zlib_stream.avail_out = VirtualSectorSize;
        zlib_stream.next_in = _device_cache->cache_buffer[page_index] + byte_offset;
        zlib_stream.next_out = reinterpret_cast<std::uint8_t*>(uncompressed);
        zlib_stream.zalloc = nullptr;
        zlib_stream.zfree = nullptr;
        inflateInit_(&zlib_stream, "1.0", sizeof(z_stream));
        inflate(&zlib_stream, Z_FINISH);
        inflateEnd(&zlib_stream);
    }
}

// OFFSET: 0x005d3460, STATUS: COMPLETE
int DataAccess::DropDevice(char* name, int force_delete) {
    if (device_list == nullptr) {
        return 0;
    }

    VirtualDataDevice* current_device = device_list;
    VirtualDataDevice* last_device = device_list + number_of_devices_in_list - 1;

    while (current_device <= last_device) {
        if (current_device->file_name != nullptr) {
            if (_stricmp(current_device->file_name, name) == 0) {
                return DropDevice(current_device, force_delete);
            }
        }
        current_device++;
    }
    return 0;
}

// OFFSET: 0x005c0b70, STATUS: COMPLETE
int DataAccess::DropDevice(VirtualDataDevice* device, int force_delete) {
    int free_device_slot = 1;
    device->flag_data |= 0x20;
    
    ContainerHashTable<char*, VirtualDataFile*>::Node* node = nullptr;
    VirtualDataFile* current_file = nullptr;
    char* current_key = nullptr;

    while (file_list->GetNextValue(&node, &current_key, &current_file) != 0) {
        if (current_file != nullptr) {
            if (current_file->device_id == device->id) {
                if (!force_delete && (current_file->resource_data != nullptr || (current_file->flag_data & 4) != 0)) {
                    free_device_slot = 0;
                }
                else {
                    FreeFile(current_file->resource_handle);
                }
            }
        }
    }

    if (free_device_slot) {
        FreeDevice(device);
        return 1;
    }
    fclose(device->file_pointer);
    device->file_pointer = nullptr;
    return 1;
}

// OFFSET: 0x005c0c50, STATUS: COMPLETE
int DataAccess::DropAllDevices(int force_delete) {
    int result = 1;

    if (this->device_list != nullptr) {
        VirtualDataDevice* current_device = this->device_list;
        VirtualDataDevice* last_device = this->device_list + this->number_of_devices_in_list - 1;

        if (this->number_of_devices_in_list > 1) {
            while (current_device != last_device) {
                if (DropDevice(current_device, force_delete) == 0) {
                    result = 0;
                }
                current_device++;
            }
        }

        if (DropDevice(last_device, force_delete) == 0) {
            result = 0;
        }

        ResetStow();
    }

    return result;
}

// OFFSET: 0x005a9af0, STATUS: COMPLETE
int DataAccess::FClose(int resource_handle) {
    if (resource_handle < 0) {
        return 0;
    }

    VirtualDataFile* current_file = file_lookup_list[resource_handle];

    if ((current_file->flag_data & 4U) == 0) {
        return 0;
    }

    VirtualDataDevice* device = &device_list[current_file->device_id];

    if ((device->flag_data & 0x4000U) != 0) {
        device->flag_data &= ~4U;
        current_file->flag_data &= ~4U;
        return 1;
    }

    current_file->current_offset = 0;
    current_file->flag_data &= ~4U;

    if (current_file->cache_id >= 0 && file_cache_list[current_file->cache_id].resource_handle == resource_handle) {
        next_available_file_cache_id = current_file->cache_id;
    }

    if ((device->flag_data & 2U) == 0) {
        if (device->file_pointer != nullptr) {
            if ((device->flag_data & 0x10U) != 0) {
                fseek(device->file_pointer, 0, SEEK_END);
                long file_size = ftell(device->file_pointer);
                current_file->size = file_size;
                device->size = file_size;
            }
            fclose(device->file_pointer);
            device->flag_data &= ~4U;
            device->file_pointer = nullptr;
        }
        if (current_file->resource_data == 0 && ((current_file->flag_data & 8) == 0)) {
            FreeDevice(device);
            FreeFile(current_file->resource_handle);
        }
    }
    return 1;
}

// OFFSET: 0x005c0cd0, STATUS: COMPLETE
char* DataAccess::FGets(int resource_handle, char* buffer, int buffer_size) {
    VirtualDataFile* target_file = this->file_lookup_list[resource_handle];
    unsigned int remaining_size = target_file->size - target_file->current_offset;

    if (remaining_size < 1) {
        *buffer = '\0';
        return nullptr;
    }

    VirtualDataDevice* associated_device = this->device_list + target_file->device_id;
    if (ActivateDevice(associated_device, target_file->start_offset + target_file->current_offset) == 0) {
        return nullptr;
    }

    unsigned int remaining_in_block = ((target_file->current_offset / VirtualSectorSize) + 1) * VirtualSectorSize - target_file->current_offset;
    unsigned int bytes_to_read = std::min({ remaining_size, static_cast<unsigned int>(buffer_size - 2), remaining_in_block });

    unsigned int bytes_read = ReadData(associated_device, target_file, buffer, bytes_to_read);
    if (bytes_read == 0) {
        *buffer = '\0';
        return nullptr;
    }

    char* buffer_end = buffer + bytes_to_read;
    *buffer_end = '\0';

    char* newline_pos = strchr(buffer, '\n');
    if (newline_pos == nullptr) {
        if (bytes_to_read != remaining_size) {
            unsigned int additional_bytes = std::min(remaining_size - bytes_to_read, static_cast<unsigned int>(buffer_size - bytes_read - 2));
            unsigned int additional_read = ReadData(associated_device, target_file, buffer_end, additional_bytes);
            if (additional_read == 0) {
                *buffer_end = '\0';
                return nullptr;
            }

            buffer_end[additional_read] = '\0';
            bytes_to_read += additional_read;
            newline_pos = strchr(buffer, '\n');
        }
    }

    if (newline_pos != nullptr) {
        char* adjusted_pos = newline_pos;
        if ((buffer < newline_pos) && (newline_pos[-1] == '\r')) {
            adjusted_pos = newline_pos - 1;
            *adjusted_pos = '\n';
        }
        adjusted_pos[1] = '\0';
        target_file->current_offset += static_cast<int>(newline_pos - buffer + 1);
    }
    else {
        target_file->current_offset += bytes_to_read;
    }

    return buffer;
}

// OFFSET: 0x005810a0, STATUS: COMPLETE
int DataAccess::FileExists(char* path) {
    if (FindVirtualFile(path) != -1) {
        return 1;
    }
    if (((this->flags & 0x1000) == 0)) {
        FILE* fp = FOPEN(path, "rb");
        if (fp != nullptr) {
            fclose(fp);
            return 1;
        }
    }
    return 0;
}

// OFFSET: 0x00580fd0, STATUS: COMPLETE
int DataAccess::FindVirtualFile(const char* path) {
    char lowercase_file_name[260]{};
    if (path == nullptr) {
        return 0;
    }
    for (std::size_t i = 0; i < sizeof(lowercase_file_name); i++) {
        if (path[i] == 0) {
            break;
        }
        lowercase_file_name[i] = std::tolower(path[i]);
    }

    if (last_get_data_or_file_handle != -1 && file_lookup_list[last_get_data_or_file_handle] != nullptr) {
        if (_stricmp(file_lookup_list[last_get_data_or_file_handle]->file_name, lowercase_file_name) == 0) {
            return last_get_data_or_file_handle;
        }
    }

    VirtualDataFile* virtual_file = nullptr;
    if (file_list->Lookup(lowercase_file_name, &virtual_file) != 0) {
        if (virtual_file != nullptr) {
            return virtual_file->resource_handle;
        }
    }

    return -1;
}

// OFFSET: 0x005d34f0, STATUS: COMPLETE
int DataAccess::FOpen(char* file_name, const char* mode) {
    int* last_resource_handle = &this->last_get_data_or_file_handle;
    int resource_handle = FindVirtualFile(file_name);
    *last_resource_handle = resource_handle;
    if (resource_handle != -1) {
        if ((this->flags & 0x800) == 0) {
            OpenVirtualFile(*last_resource_handle);
            return *last_resource_handle;
        }
        if (_stricmp(file_lookup_list[resource_handle]->file_name, device_list[this->file_lookup_list[resource_handle]->device_id].file_name) == 0) {
            OpenVirtualFile(*last_resource_handle);
            return *last_resource_handle;
        }
    }
    LoadDiskFile(file_name, mode, last_resource_handle);
    OpenVirtualFile(*last_resource_handle);
    return *last_resource_handle;
}

// OFFSET: 0x005c0e90, STATUS: COMPLETE
int DataAccess::FRead(int resource_handle, void* dst, int size) {
    VirtualDataFile* file = file_lookup_list[resource_handle];
    VirtualDataDevice* device = &device_list[file->device_id];
    if (ActivateDevice(device, file->start_offset + file->current_offset) == 0) {
        return 0;
    }
    
    int size_to_read = size;
    int size_remaining = file->size - file->current_offset;
    if (size_remaining < size_to_read) {
        size_to_read = size_remaining;
    }
    if (size_to_read < 1) {
        return 0;
    }

    if ((file->flag_data & 0x4000) != 0) {
        // WHAT IN THE UB IS THIS
        std::memcpy(dst, reinterpret_cast<const void*>(device->primary_data_offset + file->start_offset + file->current_offset), size_to_read);
        file->current_offset += size_to_read;
        return size_to_read;
    }

    int size_read = ReadData(device, file, dst, size_to_read);
    file->current_offset += size_read;
    return size_read;
}

// OFFSET: 0x005d34c0, STATUS: COMPLETE
int DataAccess::FRead(int resource_handle, void* dst, int size, int count) {
    size_t read_bytes = FRead(resource_handle, dst, size * count);
    if (size != 0) {
        return read_bytes / size;
    }
    return 0;
}

// OFFSET: 0x005812f0, STATUS: COMPLETE
int DataAccess::FREADDeviceCache(VirtualDeviceCache* _device_cache, std::size_t len, FILE* fp) {
    std::size_t bytes_read = 0;
    std::size_t bytes_remaining = 0;

    for (std::size_t i = 0; i < num_device_cache_pages; i++) {
        if (_device_cache->cache_buffer[i] != nullptr) {
            bytes_remaining = len - bytes_read;
            if (_device_cache->page_sizes[i] < len - bytes_read) {
                bytes_remaining = _device_cache->page_sizes[i];
            }
            if (0 < bytes_remaining) {
                bytes_remaining = fread(_device_cache->cache_buffer[i], 1, bytes_remaining, fp);
                bytes_read = bytes_read + bytes_remaining;
            }
        }
    }

    return bytes_read;
}

// OFFSET: 0x005a92f0, STATUS: COMPLETE
void DataAccess::FreeAllFiles() {
    if (file_list != nullptr) {
        ContainerHashTable<char*, VirtualDataFile*>::Node* node = nullptr;
        VirtualDataFile* current_file = nullptr;
        char* current_key = nullptr;

        while (file_list->GetNextValue(&node, &current_key, &current_file) != 0) {
            if (current_file != nullptr) {
                FreeFile(current_file->resource_handle);
            }
        }
    }
}

// OFFSET: 0x005a9360, STATUS: COMPLETE
void DataAccess::FreeDevice(VirtualDataDevice* device) {
    bool found_device = false;
    if ((device->flag_data & 0x4000U) == 0) {
        int* current_device_ptr = this->first_device_stow;

        do {
            if (*current_device_ptr == device->id) {
                int* last_device_ptr = this->last_device_stow;
                if (*last_device_ptr == -1) {
                    do {
                        int* device_ptr = last_device_ptr - 1;
                        last_device_ptr--;

                        if (*device_ptr != -1) {
                            *current_device_ptr = *device_ptr;
                            *last_device_ptr = -1;
                            found_device = true;
                        }
                    } while ((last_device_ptr != current_device_ptr) && (!found_device));
                }
                else {
                    *current_device_ptr = *last_device_ptr;
                    *this->last_device_stow = -1;
                    found_device = true;
                }
            }
            current_device_ptr++;
        } while ((current_device_ptr <= this->last_device_stow) && (!found_device));
    }
    if (device->id == device_cache.device_id) {
        device_cache.first_sector = -1;
        device_cache.last_sector = -1;
        device_cache.device_id = -1;
    }
    next_available_device = device->id;
    if (device->file_pointer != nullptr) {
        fclose(device->file_pointer);
        device->file_pointer = nullptr;
    }
    if (device->file_name != nullptr) {
        string_block_allocator->FreeString(device->file_name);
        device->file_name = nullptr;
    }
    device->file_pointer = nullptr;
    device->flag_data = 1;
    device->offset = 0;
    device->size = 0;
    device->primary_data_offset = 0;
    device->size_of_sector_list = 0;
    device->sector_list_offset = 0;
    device->original_key = 0;
    return;
}

// OFFSET: 0x005494c0, STATUS: COMPLETE
void DataAccess::FreeDeviceCache(VirtualDeviceCache* _device_cache) {
    if (_device_cache->cache_buffer != nullptr) {
        for (std::size_t i = 0; i < num_device_cache_pages; i++) {
            if (_device_cache->cache_buffer[i] != nullptr) {
                free(_device_cache->cache_buffer[i]);
                _device_cache->cache_buffer[i] = nullptr;
            }
        }
        free(_device_cache->cache_buffer);
        _device_cache->cache_buffer = nullptr;
        free(_device_cache->page_sizes);
        _device_cache->page_sizes = nullptr;
        _device_cache->size = 0;
    }
}

// OFFSET: 0x005a9260, STATUS: COMPLETE
void DataAccess::FreeFile(int resource_handle) {
    VirtualDataFile* file = file_lookup_list[resource_handle];
    file_list->CHTRemove(file->file_name);
    file_lookup_list[resource_handle] = nullptr;
    if (file->cache_id != -1) {
        if (file_cache_list[file->cache_id].resource_handle == resource_handle) {
            file_cache_list[file->cache_id].resource_handle = -1;
            next_available_file_cache_id = file->cache_id;
        }
    }
    if (file->file_name != nullptr) {
        string_block_allocator->FreeString(file->file_name);
        file->file_name = nullptr;
    }
    lpVirtualFileAllocator->FreeBlock(file);
}

// OFFSET: 0x005d2cc0, STATUS: COMPLETE
void DataAccess::FreeSystemResources() {
    if (device_list != nullptr) {
        DropAllDevices(1);
    }

    if (active_device != nullptr) {
        ResetStow();
    }

    if (device_list != nullptr) {
        for (std::size_t i = 0; i < number_of_devices_in_list; i++) {
            FreeDevice(&device_list[i]);
        }
        free(device_list);
        device_list = nullptr;
        number_of_devices_in_list = 0;
        next_available_device = -1;
    }

    if (file_list != nullptr) {
        FreeAllFiles();
        delete file_list;
        file_list = nullptr;
    }

    if (file_lookup_list != nullptr) {
        free(file_lookup_list);
    }
    file_lookup_list = nullptr;
    file_lookup_list_num_slots = 0;

    FreeDeviceCache(&device_cache);
    if (device_cache.sector_size_list != nullptr) {
        free(device_cache.sector_size_list);
        device_cache.sector_size_list = nullptr;
    }

    for (std::size_t i = 0; i < 8; i++) {
        if (file_cache_list[i].cache_data != nullptr) {
            free(file_cache_list[i].cache_data);
            file_cache_list[i].cache_data = nullptr;
        }
        file_cache_list[i].resource_handle = -2;
        file_cache_list[i].offset = -VirtualSectorSize;
    }

    if (string_block_allocator != nullptr) {
        delete string_block_allocator;
        string_block_allocator = nullptr;
    }
}

// OFFSET: 0x005492d0, STATUS: COMPLETE
int DataAccess::FSize(int resource_handle) {
    if (resource_handle != -1) {
        if (file_lookup_list[resource_handle] != nullptr) {
            return file_lookup_list[resource_handle]->size;
        }
    }
    return 0;
}

// stubbed because it has no purpose
// OFFSET: 0x00549770, STATUS: TODO
int DataAccess::UNK_00549770()
{
    return 1;
}

// OFFSET: 0x005d3be0, STATUS: COMPLETE
int DataAccess::GetDataOrFileHandle(char* path, char* mode, int* resource_handle, void** resource_data, int always_return_handle) {
    
    int disk_file_loaded = 0;
    int* last_handle_ptr = &last_get_data_or_file_handle;

    int file_found = FindVirtualFile(path);
    *last_handle_ptr = file_found;

    if (file_found == -1 || (((flags & 0x800) != 0) && ((device_list[file_lookup_list[file_found]->device_id].flag_data & 2) != 0))) {
        disk_file_loaded = LoadDiskFile(path, mode, last_handle_ptr);
        if (disk_file_loaded == 0 && file_found == -1) {
            return false;
        }
    }

    VirtualDataFile*  virtual_file = file_lookup_list[*last_handle_ptr];

    if (disk_file_loaded != 0 && virtual_file->resource_data != 0) {
        virtual_file->resource_data = 0;
    }

    *resource_handle = *last_handle_ptr;
    *resource_data = virtual_file->resource_data;

    if (virtual_file->resource_data == 0 || always_return_handle != 0) {
        OpenVirtualFile(*last_handle_ptr);
    }
    else if ((flags & 0x2000) != 0) {
        virtual_file->flag_data |= 8;
        return true;
    }

    return true;
}

// OFFSET: 0x00549040, STATUS: COMPLETE
int DataAccess::GetDeviceSlot() {
    int initial_slot = next_available_device;

    if (initial_slot == -1) {
        initial_slot = GrowDeviceList(4);
    }

    int current_slot = initial_slot;

    while (true) {
        current_slot += 1;

        if (static_cast<std::size_t>(current_slot) >= number_of_devices_in_list) {
            current_slot = 0;
        }

        if (current_slot == initial_slot) {
            break;
        }

        if ((device_list[current_slot].flag_data & 1) != 0) {
            next_available_device = current_slot;
            return initial_slot;
        }
    }

    next_available_device = -1;

    return initial_slot;
}

// OFFSET: 0x00548f90, STATUS: COMPLETE
int DataAccess::GrowDeviceList(int capacity_add) {
    if (capacity_add <= 0) {
        return -1;
    }

    std::size_t old_len = number_of_devices_in_list;
    device_list = reinterpret_cast<VirtualDataDevice*>(realloc(device_list, (old_len + capacity_add) * sizeof(VirtualDataDevice)));
    number_of_devices_in_list += capacity_add;
    if (device_list != nullptr) {
        for (std::size_t i = old_len; i < number_of_devices_in_list; i++) {
            device_list[i].flag_data = 1;
            device_list[i].file_name = 0;
            device_list[i].offset = 0;
            device_list[i].size = 0;
            device_list[i].file_pointer = 0;
            device_list[i].id = i;
            device_list[i].primary_data_offset = 0;
            device_list[i].size_of_sector_list = 0;
            device_list[i].sector_list_offset = 0;
            device_list[i].original_key = 0;
        }
        return old_len;
    }

    return -1;
}

// OFFSET: 0x00549090, STATUS: COMPLETE
int DataAccess::GrowFileLookupList() {
    std::size_t used_slots = 0;
    for (std::size_t i = 0; i < file_lookup_list_num_slots; i++) {
        if (file_lookup_list[i] == nullptr) {
            break;
        }
        used_slots++;
    }
    if (file_lookup_list_num_slots <= used_slots) {
        file_lookup_list = reinterpret_cast<VirtualDataFile**>(realloc(file_lookup_list, (file_lookup_list_num_slots + 16) * sizeof(VirtualDataFile*)));
        if (file_lookup_list == nullptr) {
            return -1;
        }
        memset(file_lookup_list + file_lookup_list_num_slots, 0, 16 * sizeof(VirtualDataFile*));
        file_lookup_list_num_slots += 16;
    }
    return used_slots;
}

// OFFSET: 0x005d2e10, STATUS: COMPLETE
int DataAccess::Initialize(std::size_t initial_device_count, std::size_t initial_file_lookup_capacity, std::size_t default_device_cache_size, std::size_t initial_file_cache_len, int _flags) {
    if (-1 < _flags) {
        flags = _flags;
    }
    
    UNK_00549770();
    
    FreeSystemResources();
    
    string_block_allocator = new StringBlockAllocator(0x2000, 0x2000);
    
    next_available_device = GrowDeviceList(initial_device_count);
    
    file_list = new ContainerHashTable<char*, VirtualDataFile*>();
    file_list->CHTCreateFull(503, 150, StringHashValueFunction, StringHashCompareFunction);
    
    file_lookup_list_num_slots = initial_file_lookup_capacity;
    file_lookup_list = reinterpret_cast<VirtualDataFile**>(calloc(initial_file_lookup_capacity, sizeof(VirtualDataFile*)));
    
    last_get_data_or_file_handle = -1;

    int return_value = AllocateDeviceCache(&device_cache, default_device_cache_size);
    next_available_file_cache_id = -1;
    for (std::size_t i = 0; i < 8; i++) {
        if (i < initial_file_cache_len) {
            file_cache_list[i].cache_data = reinterpret_cast<std::uint8_t*>(malloc(VirtualSectorSize));
            file_cache_list[i].resource_handle = -1;
        }
        else {
            file_cache_list[i].resource_handle = -2;
        }
        file_cache_list[i].offset = -VirtualSectorSize;
    }

    ResetStow();

    return return_value;
}

// OFFSET: 0x005d3270, STATUS: WIP
int DataAccess::LoadDiskFile(char* file_name, const char* mode, int* out_handle) {
    if ((flags & 0x1000) != 0) {
        return 0;
    }

    int device_id = GetDeviceSlot();

    char mode_cpy[12]{};
    std::size_t len = strlen(mode);
    std::memcpy(mode_cpy, mode, len);
    // Unless 't' is passed, force binary mode.
    if (strchr(mode, 't') == nullptr && strchr(mode, 'b') == nullptr) {
        mode_cpy[len] = 'b';
    }

    FILE* fp = FOPEN(file_name, mode);
    if (fp != nullptr) {
        fseek(fp, 0, SEEK_END);
        std::size_t file_size = ftell(fp);
        if (AddFile(file_name, device_id, 0, file_size, out_handle) != 0) {
            VirtualDataDevice* device = &device_list[device_id];
            device->size = file_size;
            device->file_pointer = fp;
            device->flag_data = 4;
            device->offset = 0;
            device->file_name = string_block_allocator->StringBlockAllocator_AllocStringByString(file_name, 0);
            strcpy(device->file_name, file_name);
            if (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+')) {
                device->flag_data |= 0x10;
            }
            device->primary_data_offset = 0;
            device->size_of_sector_list = 0;
            device->sector_list_offset = 0;
            device->original_key = 0;
            device->id = device_id;
            return 1;
        }
        fclose(fp);
        return 0;
    }
    FreeDevice(&device_list[device_id]);
    next_available_device = device_id;
    return 0;
}

// OFFSET: 0x005d2fc0, STATUS: COMPLETE
int DataAccess::LoadResourceFile(char* file_name, int force_read, void* out_buffer, std::size_t out_buffer_len, int encrypted_only, unsigned int* user_data_type, void* user_data, std::size_t user_data_buffer_size, std::size_t* user_data_bytes_read, int force_load) {
    if (out_buffer != nullptr) {
        force_read = 1;
    }

    unsigned int resource_size = 0;
    unsigned int resource_file_flags = 0;
    int primary_data_offset = 0;
    unsigned int sector_list_size = 0;
    int sector_list_offset = 0;
    std::uint8_t original_key = 0;

    if (!force_load) {
        VirtualDataDevice* device = device_list;
        VirtualDataDevice* last_device = device + number_of_devices_in_list - 1;

        for (; device <= last_device; ++device) {
            if (device->file_name && _stricmp(device->file_name, file_name) == 0) {
                if (!(device->flag_data & 0x20)) {
                    return 1;
                }
                DropDevice(device, 1);
            }
        }
    }

    int cache_id = next_available_file_cache_id;
    if (cache_id == -1) {
        bool found_slot = false;

        for (int i = 0; i < 8; i++) {
            int handle = file_cache_list[i].resource_handle;
            if (file_cache_list[i].resource_handle == -1 || file_cache_list[i].resource_handle == -2) {
                file_cache_list[i].resource_handle = -1;
                file_cache_list[i].offset = -VirtualSectorSize;
                found_slot = true;
                cache_id = i;
                break;
            }
            if ((file_lookup_list[handle]->flag_data & 4) == 0) {
                file_cache_list[i].resource_handle = -1;
                file_cache_list[i].offset = -VirtualSectorSize;
                found_slot = true;
                cache_id = i;
                break;
            }
        }
        if (!found_slot) {
            cache_id = -1;
        }
    }
    else {
        file_cache_list[cache_id].resource_handle = -1;
        file_cache_list[cache_id].offset = -VirtualSectorSize;
        next_available_file_cache_id = -1;
    }

    int device_id = GetDeviceSlot();
    int load_result = 0;

    if (!out_buffer || out_buffer_len < 1) {
        load_result = LoadResourceFromFile(
            file_name, &resource_file_flags, user_data_type, encrypted_only,
            &original_key, &sector_list_size, &sector_list_offset, user_data_buffer_size,
            user_data, user_data_bytes_read, &primary_data_offset, cache_id, device_id, &resource_size);
    }
    else {
        load_result = LoadResourceFromBuffer(
            out_buffer, out_buffer_len, file_name, &resource_file_flags, user_data_type,
            encrypted_only, &original_key, &sector_list_size, &sector_list_offset,
            user_data_buffer_size, user_data, user_data_bytes_read, &primary_data_offset,
            cache_id, device_id, &resource_size);
    }

    next_available_file_cache_id = cache_id;

    if (load_result == 0) {
        FreeDevice(&device_list[device_id]);
        next_available_device = device_id;
        return 0;
    }

    VirtualDataDevice* device = &device_list[device_id];
    device->flag_data = resource_file_flags | 2;
    device->file_pointer = nullptr;

    device->file_name = string_block_allocator->StringBlockAllocator_AllocStringByLength(strlen(file_name) + 1, 0);
    strcpy(device->file_name, file_name);

    device->size = resource_size;
    device->primary_data_offset = primary_data_offset;
    device->offset = 0;
    device->size_of_sector_list = sector_list_size;
    device->sector_list_offset = sector_list_offset;
    device->original_key = original_key;

    if (force_read) {
        ActivateDevice(device, 0);
        UpdateDeviceCache(device, 0, reinterpret_cast<std::uint8_t*>(out_buffer), out_buffer_len);
    }

    return load_result;
}

// OFFSET: 0x005c0f40, STATUS: TODO
int DataAccess::LoadResourceFromBuffer(void* buffer, std::size_t buffer_len, char* file_name, unsigned int* resource_file_type_flags, unsigned int* user_data_type, int encrypted_only, std::uint8_t* original_key, unsigned int* sector_list_size, int* sector_list_offset, std::size_t user_data_buffer_size, void* user_data, std::size_t* user_data_bytes_read, int* primary_data_offset, int desired_cache_id, int desired_device_id, unsigned int* resource_size) {
    // Pretty sure the game never calls this function anyway.
    panic("[DataAccess::LoadResourceFromBuffer] Unimplemented!");

}

// OFFSET: 0x005c16b0, STATUS: TODO
int DataAccess::LoadResourceFromFile(char* file_name, unsigned int* resource_file_type_flags, unsigned int* user_data_type, int encrypted_only, unsigned char* original_key, unsigned int* sector_list_size, int* sector_list_offset, std::size_t user_data_buffer_size, void* user_data, std::size_t* user_data_bytes_read, int* primary_data_offset, int desired_cache_id, int desired_device_id, unsigned int* resource_size) {
    if (file_name == nullptr || resource_file_type_flags == nullptr || original_key == nullptr || sector_list_size == nullptr || sector_list_offset == nullptr || primary_data_offset == nullptr || resource_size == nullptr) {
        panic("[DataAccess::LoadResourceFromFile] nullptr was passed as a non-optional argument.");
        return 0;
    }
    
    UNK_00549770();

    FILE* fp = FOPEN(file_name, "rb");
    if (fp == nullptr) {
        return 0;
    }

    std::uint32_t version = 0;
    std::uint32_t header_len = 0;
    std::int32_t remaining_header_len = 0;

    if (fread(&version, sizeof(std::uint32_t), 1, fp) != sizeof(std::uint32_t)) {
        fclose(fp);
        return 0;
    }
    if (fread(&header_len, sizeof(std::uint32_t), 1, fp) != sizeof(std::uint32_t)) {
        fclose(fp);
        return 0;
    }

    if (g_EnableEndianSwapping != 0) {
        version = std::byteswap(version);
        header_len = std::byteswap(header_len);
    }

    remaining_header_len = header_len;

    // Only version 2 and 3 resource files are supported in MN.
    if (version < 2 || version > 3) {
        fclose(fp);
        return 0;
    }

    if (version == 2) {
        if (user_data_type == nullptr) {
            fseek(fp, sizeof(unsigned int), SEEK_CUR);
        }
        else {
            fread(user_data_type, sizeof(unsigned int), 1, fp);
            if (g_EnableEndianSwapping) {
                *user_data_type = std::byteswap(*user_data_type);
            }
        }
        *resource_file_type_flags = 0;
        remaining_header_len -= sizeof(unsigned int);
    }
    if (version == 3) {
        fread(resource_file_type_flags, sizeof(unsigned int), 1, fp);
        if (g_EnableEndianSwapping) {
            *resource_file_type_flags = std::byteswap(*resource_file_type_flags);
        }
        if (user_data_type == nullptr) {
            fseek(fp, sizeof(unsigned int), SEEK_CUR);
        }
        else {
            fread(user_data_type, sizeof(unsigned int), 1, fp);
            if (g_EnableEndianSwapping) {
                *user_data_type = std::byteswap(*user_data_type);
            }
        }
        remaining_header_len -= sizeof(unsigned int);
        remaining_header_len -= sizeof(unsigned int);
    }
    if (encrypted_only != 0) {
        if ((*resource_file_type_flags & 0x80) == 0 || (*resource_file_type_flags & 0x400) == 0) {
            fclose(fp);
            return 0;
        }
        *original_key = GenerateKeyXOR(file_name);
    }

    if ((*resource_file_type_flags & 0x40) != 0) {
        fread(sector_list_size, sizeof(unsigned int), 1, fp);
        if (g_EnableEndianSwapping) {
            *sector_list_size = std::byteswap(*sector_list_size);
        }
        *sector_list_offset = ftell(fp);
        fseek(fp, *sector_list_size, SEEK_CUR);
        remaining_header_len -= sizeof(unsigned int);
        remaining_header_len -= *sector_list_size;
    }

    if (user_data_buffer_size != 0) {
        std::size_t udata_to_read = std::min(user_data_buffer_size, static_cast<std::size_t>(remaining_header_len));
        if (udata_to_read > 0) {
            fread(user_data, 1, udata_to_read, fp);
            *user_data_bytes_read = udata_to_read;
        }
        else {
            *user_data_bytes_read = 0;
        }
        remaining_header_len -= udata_to_read;
    }

    if (remaining_header_len > 0) {
        fseek(fp, remaining_header_len, SEEK_CUR);
    }

    std::uint32_t offset_table_len = 0;
    std::uint32_t offset_table_size = 0;

    if (fread(&offset_table_len, sizeof(std::uint32_t), 1, fp) != sizeof(std::uint32_t)) {
        fclose(fp);
        return 0;
    }
    if (fread(&offset_table_size, sizeof(std::uint32_t), 1, fp) != sizeof(std::uint32_t)) {
        fclose(fp);
        return 0;
    }

    if (g_EnableEndianSwapping != 0) {
        offset_table_len = std::byteswap(offset_table_len);
        offset_table_size = std::byteswap(offset_table_size);
    }

    if (offset_table_len == 0 || offset_table_size == 0) {
        return 0;
    }

    *primary_data_offset = ftell(fp) + offset_table_size;

    bool all_files_added = false;

    if (desired_cache_id != -1) {
        if (offset_table_size < VirtualSectorSize) {
            std::uint8_t* current_position = file_cache_list[desired_cache_id].cache_data;
            fread(current_position, offset_table_size, 1, fp);

            std::uint32_t current_file = 0;
            all_files_added = true;
            do {
                std::uint32_t name_len = 0;
                memcpy(&name_len, current_position, sizeof(std::uint32_t));
                current_position += sizeof(std::uint32_t);
                
                char virtual_file_name[260]{};
                memcpy(virtual_file_name, current_position, name_len);
                current_position += name_len;
                virtual_file_name[name_len] = 0;

                if ((*resource_file_type_flags & 0x400) != 0) {
                    DecryptXOR(virtual_file_name, name_len, *original_key);
                }
                
                std::uint32_t start_offset = 0;
                memcpy(&start_offset, current_position, sizeof(std::uint32_t));
                current_position += sizeof(std::uint32_t);

                std::uint32_t virtual_file_size = 0;
                memcpy(&virtual_file_size, current_position, sizeof(std::uint32_t));
                current_position += sizeof(std::uint32_t);

                if (!AddFile(virtual_file_name, desired_device_id, start_offset, virtual_file_size, nullptr)) {
                    all_files_added = false;
                }

                current_file++;
            } while (current_file != offset_table_len);
        }
        else {
            all_files_added = true;
            std::uint32_t current_file = 0;
            std::uint32_t bytes_remaining = offset_table_size;
            std::uint8_t* cache_data_start = file_cache_list[desired_cache_id].cache_data;
            std::uint8_t* cache_data_end = file_cache_list[desired_cache_id].cache_data + VirtualSectorSize;
            std::uint32_t extra_bytes_remaining = 0;

            do {
                std::size_t to_read = std::min(bytes_remaining, VirtualSectorSize - (extra_bytes_remaining + 1));
                bytes_remaining -= to_read;

                std::uint8_t* current_position = cache_data_start;
                fread(current_position + extra_bytes_remaining, to_read, 1, fp);

                std::uint8_t* record_start = current_position;

                bool has_extra_data = true;
                do {
                    record_start = current_position;

                    if ((cache_data_end - current_position) > sizeof(std::uint32_t)) {
                        std::uint32_t name_len = 0;
                        memcpy(&name_len, current_position, sizeof(std::uint32_t));
                        current_position += sizeof(std::uint32_t);

                        if ((cache_data_end - current_position) > static_cast<std::ptrdiff_t>(name_len + sizeof(std::uint32_t) + sizeof(std::uint32_t))) {

                            char virtual_file_name[260]{};
                            memcpy(virtual_file_name, current_position, name_len);
                            current_position += name_len;
                            virtual_file_name[name_len] = 0;

                            if ((*resource_file_type_flags & 0x400) != 0) {
                                DecryptXOR(virtual_file_name, name_len, *original_key);
                            }

                            std::uint32_t start_offset = 0;
                            memcpy(&start_offset, current_position, sizeof(std::uint32_t));
                            current_position += sizeof(std::uint32_t);

                            std::uint32_t virtual_file_size = 0;
                            memcpy(&virtual_file_size, current_position, sizeof(std::uint32_t));
                            current_position += sizeof(std::uint32_t);


                            if (!AddFile(virtual_file_name, desired_device_id, start_offset, virtual_file_size, nullptr)) {
                                all_files_added = false;
                            }

                            current_file++;
                        }
                        else {
                            has_extra_data = false;
                        }
                    }
                    else {
                        has_extra_data = false;
                    }
                } while ((has_extra_data) && (current_file != offset_table_len));
                extra_bytes_remaining = (cache_data_end - record_start) - 1;
                memcpy(cache_data_start, record_start, extra_bytes_remaining);

            } while (current_file != offset_table_len);
        }
    }
    else {
        all_files_added = true;
        std::uint32_t current_file = 0;
        do {
            std::uint32_t name_len = 0;
            fread(&name_len, sizeof(std::uint32_t), 1, fp);

            char virtual_file_name[260]{};
            fread(virtual_file_name, 1, name_len, fp);
            virtual_file_name[name_len] = 0;

            if ((*resource_file_type_flags & 0x400) != 0) {
                DecryptXOR(virtual_file_name, name_len, *original_key);
            }

            std::uint32_t start_offset = 0;
            fread(&start_offset, sizeof(std::uint32_t), 1, fp);

            std::uint32_t virtual_file_size = 0;
            fread(&virtual_file_size, sizeof(std::uint32_t), 1, fp);

            if (!AddFile(virtual_file_name, desired_device_id, start_offset, virtual_file_size, nullptr)) {
                all_files_added = false;
            }

            current_file++;
        } while (current_file != offset_table_len);
    }

    if (!all_files_added) {
        for (std::size_t counter = 0; counter < file_lookup_list_num_slots; counter++) {
            if (file_lookup_list[counter]->device_id == desired_device_id) {
                FreeFile(counter);
            }
        }
    }

    if (version == 2) {
        for (std::size_t counter = 0; counter < file_lookup_list_num_slots; counter++) {
            if (file_lookup_list[counter]->device_id == desired_device_id) {
                file_lookup_list[counter]->start_offset -= *primary_data_offset;
            }
        }
    }

    long pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    *resource_size = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    fclose(fp);

    return all_files_added ? 1 : 0;
}

// OFFSET: 0x00549580, STATUS: COMPLETE
int DataAccess::MemcpyDeviceCache(VirtualDeviceCache* _device_cache, std::size_t len, void* src) {
    int bytes_read = 0;
    int bytes_remaining = 0;

    for (std::size_t i = 0; i < num_device_cache_pages; i++) {
        if (_device_cache->cache_buffer[i] != nullptr) {
            bytes_remaining = len - bytes_read;
            if (_device_cache->page_sizes[i] < len - bytes_read) {
                bytes_remaining = _device_cache->page_sizes[i];
            }
            if (0 < bytes_remaining) {
                std::memcpy(_device_cache->cache_buffer[i], reinterpret_cast<const void*>(reinterpret_cast<std::uintptr_t>(src) + bytes_read), bytes_remaining);
                bytes_read = bytes_read + bytes_remaining;
            }
        }
    }

    return bytes_read;
}

// OFFSET: 0x005491c0, STATUS: COMPLETE
void DataAccess::OpenVirtualFile(int resource_handle) {
    if (resource_handle < 0) {
        return;
    }

    VirtualDataFile* file = file_lookup_list[resource_handle];
    file->flag_data |= 4;
    file->current_offset = 0;

    if (!(device_list[file->device_id].flag_data & 0x4000U)) {
        int cache_id = file->cache_id;

        if (cache_id < 0 || file_cache_list[cache_id].resource_handle != resource_handle) {
            if (next_available_file_cache_id == -1) {
                bool cache_allocated = false;
                int* cache_entry_ptr = &file_cache_list[0].resource_handle;

                for (std::size_t i = 0; i < 8; i++) {
                    if ((file_cache_list[i].resource_handle == -2) || (file_cache_list[i].resource_handle == -1)) {
                        file->cache_id = i;
                        file_cache_list[i].offset = -VirtualSectorSize;
                        file_cache_list[i].resource_handle = resource_handle;
                        cache_allocated = true;
                        break;
                    }
                    if ((file_lookup_list[file_cache_list[i].resource_handle]->flag_data & 4) == 0) {
                        file->cache_id = i;
                        file_cache_list[i].offset = -VirtualSectorSize;
                        file_cache_list[i].resource_handle = resource_handle;
                        cache_allocated = true;
                        break;
                    }
                }

                if (!cache_allocated) {
                    file->cache_id = -1;
                }
            }
            else {
                file->cache_id = next_available_file_cache_id;
                file_cache_list[next_available_file_cache_id].resource_handle = resource_handle;
                file_cache_list[next_available_file_cache_id].offset = -VirtualSectorSize;
                next_available_file_cache_id = -1;
            }
        }
        else if (next_available_file_cache_id == cache_id) {
            next_available_file_cache_id = -1;
        }

        if ((flags & 0x2000) != 0) {
            file->flag_data |= 8;
        }
    }
}

// OFFSET: 0x005a96a0, STATUS: TODO
unsigned int DataAccess::ReadData(VirtualDataDevice* device, VirtualDataFile* file, void* dst, unsigned int dst_len) {
    /*
    // "I AM LAZY AND JUST WANT SOMETHING WORKING RAAAAAAAH"
    // FIXME: This implementation is just temporarily in place for now... a proper implementation is REQUIRED for loading from a .res file.
    // Therefore this only works for files loaded from disk.
    fseek(device->file_pointer, device->primary_data_offset + device->offset, 0);
    std::size_t bytes_read = fread(dst, 1, dst_len, device->file_pointer);
    if (bytes_read != dst_len) {
        return 0;
    }
    device->offset = device->offset + dst_len;
    return dst_len;
    */

    if ((device->flag_data & 0x4000) != 0) {
        std::memcpy(dst, reinterpret_cast<const void*>(device->primary_data_offset + file->current_offset + file->start_offset), dst_len);
        return dst_len;
    }
    UNK_00549770();
    if ((device->flag_data & 0x100) == 0) {
        if (file->cache_id == -1 || dst_len >= VirtualSectorSize) {
            fseek(device->file_pointer, device->primary_data_offset + device->offset, 0);
            std::size_t bytes_read = fread(dst, 1, dst_len, device->file_pointer);
            if (bytes_read != dst_len) {
                return 0;
            }
            device->offset = device->offset + dst_len;
            return dst_len;
        }
        std::ptrdiff_t cache_offset = device->offset - file_cache_list[file->cache_id].offset;
        if (cache_offset < 0 || cache_offset >= VirtualSectorSize) {
            file_cache_list[file->cache_id].offset = device->offset;
            std::ptrdiff_t remainder = (device->size - device->offset) - device->primary_data_offset;
            std::size_t bytes_to_read = VirtualSectorSize;
            if (remainder < VirtualSectorSize) {
                bytes_to_read = remainder;
            }
            fseek(device->file_pointer, device->offset + device->primary_data_offset, SEEK_SET);
            std::size_t bytes_read = fread(file_cache_list[file->cache_id].cache_data, 1, bytes_to_read, device->file_pointer);
            if (bytes_read != bytes_to_read) {
                return 0;
            }
            cache_offset = 0;
        }
        std::size_t bytes_to_read = dst_len;
        if (VirtualSectorSize - cache_offset < static_cast<std::ptrdiff_t>(dst_len)) {
            bytes_to_read = VirtualSectorSize - cache_offset;
        }
        std::memcpy(dst, file_cache_list[file->cache_id].cache_data + cache_offset, bytes_to_read);
        device->offset = device->offset + bytes_to_read;
        if (bytes_to_read != dst_len) {
            file_cache_list[file->cache_id].offset = device->offset;
            std::ptrdiff_t remainder = (device->size - device->offset) - device->primary_data_offset;
            std::size_t bytes_to_read = VirtualSectorSize;
            if (remainder < VirtualSectorSize) {
                bytes_to_read = remainder;
            }
            fseek(device->file_pointer, device->offset + device->primary_data_offset, SEEK_SET);
            std::size_t bytes_read = fread(file_cache_list[file->cache_id].cache_data, 1, bytes_to_read, device->file_pointer);
            return bytes_read;
        }
        return dst_len;
    }

    VirtualFileCache* cache = nullptr;
    if (file->cache_id < 0) {
        // WTFRB: i just know they were mad asf when they wrote this shit
        cache = reinterpret_cast<VirtualFileCache*>(malloc(sizeof(VirtualFileCache)));
        if (cache == nullptr) {
            return 0;
        }
        cache->resource_handle = -2;
        cache->offset = -VirtualSectorSize;
        cache->cache_data = reinterpret_cast<std::uint8_t*>(malloc(VirtualSectorSize));
        if (cache->cache_data == nullptr) {
            free(cache);
            return 0;
        }
    }
    else {
        cache = &file_cache_list[file->cache_id];
    }

    std::ptrdiff_t cache_offset = 0;
    std::size_t bytes_to_copy = 0;
    std::uint32_t bytes_copied = 0;
    std::uint8_t* destination = reinterpret_cast<std::uint8_t*>(dst);

    do {
        cache_offset = device->offset - cache->offset;

        if ((cache_offset < 0) || (cache_offset >= VirtualSectorSize)) {
            std::ptrdiff_t virtual_sector_load_start = device->offset / VirtualSectorSize;
            if ((virtual_sector_load_start < device_cache.first_sector) || (virtual_sector_load_start > device_cache.last_sector)) {
                UpdateDeviceCache(device, virtual_sector_load_start, nullptr, -1);
            }

            std::ptrdiff_t sectors_into_cache = virtual_sector_load_start - device_cache.first_sector;
            std::int16_t sector_data_size = 0;
            std::uint32_t read_start_offset = 0;

            if ((device->flag_data & 0x40) != 0) {
                std::size_t padding = 0;
                if ((device->flag_data & 0x200) != 0) {
                    padding = VirtualSectorMarkerSize;
                }
                std::int16_t* start_sector_size = device_cache.sector_size_list + device_cache.first_sector;
                std::int16_t* final_sector_size = start_sector_size + sectors_into_cache;
                if (sectors_into_cache > 0) {
                    do {
                        read_start_offset += (*start_sector_size) + padding;
                        start_sector_size++;
                    } while (start_sector_size != final_sector_size);
                }
                sector_data_size = *start_sector_size;
            }
            else {
                if ((device->flag_data & 0x200) != 0) {
                    read_start_offset += sectors_into_cache * (VirtualSectorSize + VirtualSectorMarkerSize);
                }
                else {
                    read_start_offset += sectors_into_cache * VirtualSectorSize;
                }

                sector_data_size = VirtualSectorSize;
            }

            // WTFRB: what the hell does this even do????
            if ((device->flag_data & 0x200) != 0 && unk1 != 0) {
                do {
                    if ((sector_data_size + read_start_offset) < 1) {
                        break;
                    }
                    unk1 = 0;
                } while (unk1 != 0);
            }

            cache->offset = virtual_sector_load_start * VirtualSectorSize;

            if ((device->flag_data & 0x40) == 0) {
                CopyDeviceCache(&device_cache, cache, read_start_offset);
            }
            else {
                if (sector_data_size == VirtualSectorSize) {
                    CopyDeviceCache(&device_cache, cache, read_start_offset);
                }
                else {
                    DecompressDeviceCache(&device_cache, cache->cache_data, sector_data_size, read_start_offset);
                }
            }

            if ((device->flag_data & 0x80) != 0 && (device->flag_data & 0x400) != 0) {
                DecryptXOR(cache->cache_data, VirtualSectorSize, static_cast<std::uint8_t>((static_cast<std::uint32_t>(device->original_key) + virtual_sector_load_start) % 255));
            }

            cache_offset = device->offset - cache->offset;
        }

        bytes_to_copy = std::min(static_cast<std::ptrdiff_t>(dst_len - bytes_copied), VirtualSectorSize - cache_offset);
        memcpy(destination, cache->cache_data + cache_offset, bytes_to_copy);
        device->offset += bytes_to_copy;
        bytes_copied += bytes_to_copy;

        destination += bytes_to_copy;
    } while (bytes_copied != dst_len);

    // WTFRB: i just know they were mad asf when they wrote this shit
    if (cache->resource_handle == -2) {
        free(cache->cache_data);
        free(cache);
    }
    
    return 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
void DataAccess::ResetStow() {
    int* current_device = first_device_stow;
    do {
        *current_device = -1;
        current_device++;
    } while (current_device <= last_device_stow);
}

// OFFSET: 0x00581290, STATUS: COMPLETE
int DataAccess::ResizeDeviceCache(unsigned int new_size) {
    std::uint32_t current_size = device_cache.size;

    if (current_size <= new_size && (new_size <= num_device_cache_pages + current_size)) {
        return 1;
    }

    int* device_stow_ptr = first_device_stow;
    device_cache.first_sector = -1;
    device_cache.last_sector = -1;
    device_cache.device_id = -1;

    while (device_stow_ptr <= last_device_stow) {
        *device_stow_ptr = -1;
        device_stow_ptr++;
    }

    FreeDeviceCache(&device_cache);

    int allocation_result = AllocateDeviceCache(&device_cache, new_size);

    if (allocation_result == 0) {
        device_cache.size = 0;
    }

    return allocation_result;
}

// OFFSET: 0x005d3570, STATUS: TODO
void DataAccess::SaveResourceFile(char*, int, int, int, int, void*, size_t, int) {
}

// OFFSET: 0x005810f0, STATUS: TODO
void DataAccess::SaveResourceFileList(char*, char*) {
}

// OFFSET: 0x00549710, STATUS: COMPLETE
int DataAccess::SetNumberOfDeviceCachePages(int new_device_cache_page_count) {
    if (this->num_device_cache_pages == new_device_cache_page_count) {
        return 1;
    }
    int cache_size = (this->device_cache).size;
    (this->device_cache).first_sector = -1;
    (this->device_cache).last_sector = -1;
    (this->device_cache).device_id = -1;
    int* current_device = this->first_device_stow;
    do {
        *current_device = -1;
        current_device = current_device + 1;
    } while (current_device <= this->last_device_stow);
    FreeDeviceCache(&this->device_cache);
    this->num_device_cache_pages = new_device_cache_page_count;
    if (AllocateDeviceCache(&this->device_cache, cache_size) == 0) {
        (this->device_cache).size = 0;
    }
    return cache_size;
}

// OFFSET: 0x005a9450, STATUS: COMPLETE
void DataAccess::UpdateDeviceCache(VirtualDataDevice* device, int virtual_sector_needed, std::uint8_t* buffer, std::size_t max_read_len) {
    if (buffer == nullptr) {
        UNK_00549770();
        return;
    }
    
    bool should_clear_cache = (device->flag_data & 0x200) != 0;
    bool should_update_sector_list = (device->flag_data & 0x40) != 0;
    std::uint8_t* buffer_position = buffer;

    if (device_cache.first_sector == -1 && should_update_sector_list) {
        short* sector_size_list = device_cache.sector_size_list;
        if (sector_size_list != nullptr) {
            free(sector_size_list);
        }

        sector_size_list = reinterpret_cast<short*>(malloc(device->size_of_sector_list));
        device_cache.sector_size_list = sector_size_list;

        if (buffer == nullptr) {
            fseek(device->file_pointer, device->sector_list_offset, SEEK_SET);
            fread(sector_size_list, 1, device->size_of_sector_list, device->file_pointer);
        }
        else {
            buffer_position += device->sector_list_offset;
            std::memcpy(sector_size_list, buffer_position, device->size_of_sector_list);
            buffer_position += device->size_of_sector_list;
        }
        
        if (g_EnableEndianSwapping != 0 && device->size_of_sector_list > 0) {
            for (std::size_t offset = 0; offset < device->size_of_sector_list; offset += 2) {
                std::uint16_t* entry = reinterpret_cast<std::uint16_t*>(reinterpret_cast<uintptr_t>(sector_size_list) + offset);
                *entry = std::byteswap(*entry);
            }
        }
    }

    if (should_clear_cache) {
        ClearDeviceCache(&device_cache);
    }

    std::intptr_t offset = device->primary_data_offset;
    if (!should_update_sector_list || virtual_sector_needed < 1) {
        offset += virtual_sector_needed * VirtualSectorSize;
    }
    else {
        for (std::size_t i = 0; i < static_cast<std::size_t>(virtual_sector_needed); ++i) {
            offset += device_cache.sector_size_list[i];
        }
    }

    if (buffer == 0) {
        fseek(device->file_pointer, offset, SEEK_SET);
        max_read_len = INT_MAX;
    }
    else {
        buffer_position = offset + buffer;
        max_read_len = reinterpret_cast<uintptr_t>(buffer) - reinterpret_cast<uintptr_t>(buffer_position) + max_read_len;
    }

    std::size_t remaining_size = device->size - offset;
    if (remaining_size > device_cache.size) {
        remaining_size = device_cache.size;
    }
    if (remaining_size > max_read_len) {
        remaining_size = max_read_len;
    }

    if (buffer == nullptr) {
        remaining_size = FREADDeviceCache(&device_cache, remaining_size, device->file_pointer);
    }
    else {
        remaining_size = MemcpyDeviceCache(&device_cache, remaining_size, buffer_position);
    }

    unsigned int sector_count = 0;
    if (should_update_sector_list) {
        std::int16_t* sector_sizes = device_cache.sector_size_list + virtual_sector_needed;
        std::size_t total_remaining_bytes = 0;
        while (remaining_size >= total_remaining_bytes) {
            total_remaining_bytes += *sector_sizes;
            sector_count++;
            sector_sizes++;
        }
    }
    else {
        sector_count = remaining_size / VirtualSectorSize;
    }

    if (sector_count > 0) {
        device_cache.first_sector = virtual_sector_needed;
        device_cache.last_sector = virtual_sector_needed + sector_count - 1;
    }
    else {
        device_cache.first_sector = -1;
        device_cache.last_sector = -1;
    }
}