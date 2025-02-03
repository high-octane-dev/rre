#pragma once
#include "base_object.hpp"
#include "containers/container_hash_table.hpp"
#include "string_block_allocator.hpp"

struct VirtualDataDevice {
    int id;
    char* file_name;
    int offset;
    std::uint32_t flag_data;
    FILE* file_pointer;
    std::uint32_t size;
    std::intptr_t primary_data_offset;
    int sector_list_offset;
    std::uint32_t size_of_sector_list;
    std::uint8_t original_key;
};

struct VirtualDataFile {
    int device_id;
    int cache_id;
    int resource_handle;
    int start_offset;
    int current_offset;
    char* file_name;
    std::uint32_t size;
    std::uint32_t flag_data;
    void* resource_data;
};

struct VirtualFileCache {
    std::uint8_t* cache_data;
    int resource_handle;
    int offset;
};

struct VirtualDeviceCache {
    std::uint8_t** cache_buffer;
    std::uint32_t* page_sizes;
    int device_id;
    std::uint32_t size;
    int first_sector;
    int last_sector;
    std::int16_t* sector_size_list;
};

struct ResMonitorData {
    int device_id;
    std::uint32_t time_start;
    std::uint32_t mem_used_start;
    int num_cache_updates;
    int cache_updates[128][4];
};

class DataAccess : public BaseObject {
public:
	std::uint32_t flags;
	int next_available_device;
    VirtualDataDevice* device_list;
    std::size_t number_of_devices_in_list;
    VirtualDataDevice* active_device;
    int device_stow[3];
    int* first_device_stow;
    int* last_device_stow;
    VirtualDeviceCache device_cache;
    std::size_t num_device_cache_pages;
    VirtualFileCache file_cache_list[8];
    int next_available_file_cache_id;
    ContainerHashTable<char*, VirtualDataFile*>* file_list;
    VirtualDataFile** file_lookup_list;
    std::size_t file_lookup_list_num_slots;
    int last_get_data_or_file_handle;

    // the game checks if these are != 0, but they never get set to anything other than 0
    int unk1;
    int unk2;
    void* unk_vtable;

    // all of this goes completely unused
    int report_bytes_read_uncached;
    int report_bytes_read_device_cache;
    int report_bytes_copied_to_device_cache;
    int report_bytes_copied_to_user;
    int report_bytes_decompressed;
    ResMonitorData resource_monitor;

    // not this though!!!!!
    StringBlockAllocator* string_block_allocator;
public:
    DataAccess();
    DataAccess(const DataAccess&) = delete;
    DataAccess& operator=(const DataAccess&) = delete;
    virtual ~DataAccess() override;

    int ActivateDevice(VirtualDataDevice*, int);
    int AddFile(char*, int, int, int, int*);
    int AllocateDeviceCache(VirtualDeviceCache*, int);
    int AllowBundledAccessOnly(int);
    int AttachObject(int, void*);
    void ClearDeviceCache(VirtualDeviceCache*);
    void CopyDeviceCache(VirtualDeviceCache* deviceCache, void* dst, std::ptrdiff_t byte_offset);
    void DecompressDeviceCache(VirtualDeviceCache* deviceCache, void* uncompressed, std::size_t compressed_len, std::ptrdiff_t byte_offset);
    int DropDevice(char*, int);
    int DropDevice(VirtualDataDevice*, int);
    int DropAllDevices(int);
    int FClose(int resource_handle);
    char* FGets(int, char*, int);
    int FileExists(char*);
    int FindVirtualFile(const char*);
    int FOpen(char*, const char*);
    int FRead(int, void*, int);
    int FRead(int, void*, int, int);
    int FREADDeviceCache(VirtualDeviceCache*, std::size_t, FILE*);
    void FreeAllFiles();
    void FreeDevice(VirtualDataDevice*);
    void FreeDeviceCache(VirtualDeviceCache*);
    void FreeFile(int);
    void FreeSystemResources();
    int FSize(int);

    // basically does nothing anyway
    int UNK_00549770();

    int GetDataOrFileHandle(char*, char*, int*, void**, int);
    int GetDeviceSlot();
    int GrowDeviceList(int);
    int GrowFileLookupList();
    int Initialize(std::size_t, std::size_t, std::size_t, std::size_t, int);
    int LoadDiskFile(char*, const char*, int*);
    int LoadResourceFile(char*, int, unsigned int*, int, int, unsigned int*, unsigned int*, int**, int**, int);
    void LoadResourceFromBuffer(unsigned int*, int, char*, int**, unsigned int, int, unsigned char*, int**, int**, int**, unsigned int*, int**, int*, int, int, long*);
    void LoadResourceFromFile(char*, unsigned int*, unsigned int, int, unsigned char*, unsigned int**, long*, unsigned int, unsigned int*, unsigned int*, int*, int, int, long*);
    int MemcpyDeviceCache(VirtualDeviceCache*, std::size_t, void*);
    void OpenVirtualFile(int);
    unsigned int ReadData(VirtualDataDevice*, VirtualDataFile*, void*, unsigned int);
    void ResetStow();
    int ResizeDeviceCache(unsigned int);
    void SaveResourceFile(char*, int, int, int, int, void*, size_t, int);
    void SaveResourceFileList(char*, char*);
    int SetNumberOfDeviceCachePages(int);
    void UpdateDeviceCache(VirtualDataDevice*, int, std::uint8_t*, std::size_t);
};

extern DataAccess* lpDataAccess;
extern BlockAllocator* lpVirtualFileAllocator;

#ifdef _M_IX86
static_assert(sizeof(VirtualDataDevice) == 40);
static_assert(sizeof(VirtualDataFile) == 36);
static_assert(sizeof(VirtualFileCache) == 12);
static_assert(sizeof(VirtualDeviceCache) == 28);
static_assert(sizeof(ResMonitorData) == 2064);
static_assert(sizeof(DataAccess) == 2296);
#endif