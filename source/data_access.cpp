#include "data_access.hpp"
#include "file_io.hpp"

DataAccess* lpDataAccess = nullptr;
BlockAllocator* lpVirtualFileAllocator = nullptr;

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
    file_cache_list[0].offset = -0x6000;
    file_cache_list[1].offset = -0x6000;
    file_cache_list[2].offset = -0x6000;
    file_cache_list[3].offset = -0x6000;
    file_cache_list[4].offset = -0x6000;
    file_cache_list[5].offset = -0x6000;
    file_cache_list[6].offset = -0x6000;
    file_cache_list[7].offset = -0x6000;
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

DataAccess::~DataAccess() {
    FreeSystemResources();
}

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

int DataAccess::AddFile(char* file_name, int device_id, int start_offset, int size, int* handle)
{
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
    return 1;
}

int DataAccess::AllocateDeviceCache(VirtualDeviceCache* device_cache, int size_bytes)
{
    std::size_t bytes_per_page = static_cast<std::size_t>(std::round(size_bytes / static_cast<float>(num_device_cache_pages)));
    device_cache->size = 0;
    int success = 1;
    if (device_cache->cache_buffer == nullptr) {
        device_cache->cache_buffer = new std::uint8_t* [num_device_cache_pages];
    }
    if (device_cache->page_sizes == nullptr) {
        device_cache->page_sizes = new std::uint32_t [num_device_cache_pages];
    }
    for (std::size_t i = 0; i < num_device_cache_pages; i++) {
        device_cache->cache_buffer[i] = reinterpret_cast<std::uint8_t*>(malloc(bytes_per_page));
        if (i == 0 && *device_cache->cache_buffer == nullptr) {
            *device_cache->page_sizes = bytes_per_page;
            success = 0;
        }
        if (device_cache->cache_buffer[i] != nullptr) {
            device_cache->size += bytes_per_page;
            device_cache->page_sizes[i] = bytes_per_page;
        }
    }
    return success;
}

int DataAccess::AllowBundledAccessOnly(int should_allow_only_bundled_access)
{
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

int DataAccess::AttachObject(int resource_handle, void* object)
{
    this->file_lookup_list[resource_handle]->resource_data = object;
    return 1;
}

int DataAccess::DropDevice(char*, int)
{
    return 0;
}

int DataAccess::DropDevice(VirtualDataDevice*, int)
{
    return 0;
}

int DataAccess::DropAllDevices(int)
{
    return 0;
}

int DataAccess::FClose(int resource_handle)
{
    return 0;
}

char* DataAccess::FGets(int, char*, int)
{
    return nullptr;
}

int DataAccess::FileExists(char* path)
{
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

int DataAccess::FindVirtualFile(char*)
{
    return 0;
}

int DataAccess::FOpen(const char*, const char*)
{
    return -1;
}

int DataAccess::FRead(int resource_handle, void* dst, int size)
{
    return 0;
}

int DataAccess::FRead(int resource_handle, void* dst, int size, int count) {
    size_t read_bytes = FRead(resource_handle, dst, size * count);
    if (size != 0) {
        return read_bytes / size;
    }
    return 0;
}

void DataAccess::FreeAllFiles()
{
}

void DataAccess::FreeDevice(VirtualDataDevice*)
{
}

void DataAccess::FreeDeviceCache(VirtualDeviceCache*)
{
}

void DataAccess::FreeFile(int)
{
}

void DataAccess::FreeSystemResources()
{
}

int DataAccess::FSize(int)
{
    return 0;
}

// stubbed because it has no purpose
int DataAccess::UNK_00549770()
{
    return 1;
}

int DataAccess::GetDataOrFileHandle(char*, char*, int*, int*, int)
{
    return 0;
}

void DataAccess::GetDeviceSlot(int)
{
}

int DataAccess::GrowDeviceList(int)
{
    return 0;
}

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

int DataAccess::Initialize(int, std::size_t, int, int, int)
{
    return 0;
}

int DataAccess::LoadDiskFile(char*, char*, int)
{
    return 0;
}

int DataAccess::LoadResourceFile(char*, int, unsigned int*, int, int, unsigned int*, unsigned int*, int**, int**, int)
{
    return 0;
}

void DataAccess::LoadResourceFromBuffer(unsigned int*, int, char*, int**, unsigned int, int, unsigned char*, int**, int**, int**, unsigned int*, int**, int*, int, int, long*)
{
}

void DataAccess::LoadResourceFromFile(char*, unsigned int*, unsigned int, int, unsigned char*, unsigned int**, long*, unsigned int, unsigned int*, unsigned int*, int*, int, int, long*)
{
}

void DataAccess::OpenVirtualFile(int)
{
}

unsigned int DataAccess::ReadData(VirtualDataDevice*, void*, void*, unsigned int)
{
    return 0;
}

int DataAccess::ResizeDeviceCache(unsigned int)
{
    return 0;
}

void DataAccess::SaveResourceFile(char*, int, int, int, int, void*, size_t, int)
{
}

void DataAccess::SaveResourceFileList(char*, char*)
{
}

int DataAccess::SetNumberOfDeviceCachePages(int param_1)
{
    if (this->num_device_cache_pages == param_1) {
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
    this->num_device_cache_pages = param_1;
    if (AllocateDeviceCache(&this->device_cache, cache_size) == 0) {
        (this->device_cache).size = 0;
    }
    return cache_size;
}

void DataAccess::UpdateDeviceCache(VirtualDataDevice*, int, int, int)
{
}