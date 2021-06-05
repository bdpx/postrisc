#pragma once

#include "assembler/assembler.hpp"
#include "emulator/IAddressSpace.hpp" // for CPhysAddress

namespace postrisc {

enum configuration_flags {
    CONFIG_TIMING         = 1U << 0,
    CONFIG_PROFILE        = 1U << 1,
    CONFIG_VERBOSE        = 1U << 2,
    CONFIG_DUMP_HOST_INFO = 1U << 3,
    CONFIG_DUMP_HTML      = 1U << 4,
};

class CoreConfig {
public:
    std::string name;
};

class MemoryConfig {
public:
    MemoryConfig(CPhysAddress _address, size_t _size) : address(_address), size(_size) {}

public:
    CPhysAddress  address;
    size_t        size;
};

class RomConfig {
public:
    RomConfig(void)
        : phys_address(U64C(0x00000000fc000000))
        , virt_address(U64C(0xfffffffffc000000))
    {}

public:
    CPhysAddress    phys_address;
    CVirtAddress    virt_address;
    std::vector<u8> data;
};

class VideoConfig {
public:
    VideoConfig(void)
        : phys_address(U64C(0x00000000fb000000))
        , virt_address(U64C(0xfffffffffb000000))
        , width(360)
        , height(240)
    {}

public:
    CPhysAddress      phys_address;
    CVirtAddress      virt_address;
    unsigned          width;
    unsigned          height;
};

class DeviceArrayConfig {
public:
    DeviceArrayConfig(void)
        : max_config_space_size (4 * 1024)
        , phys_address(U64C(0x00000000fee00000))
        , virt_address(U64C(0xfffffffffee00000))
    {}

public:
    u32           max_config_space_size; // default 4KiB
    CPhysAddress  phys_address;
    CVirtAddress  virt_address;
};


class EmulatorConfiguration {
public:
    EmulatorConfiguration(void);
    ~EmulatorConfiguration(void);

    void SetDefaults(void);
    bool parse(char const *filename);
    void ExportDefinitions(CAssembler& assembler) const;
    unsigned get_config_flags(void) const;

private:
    void Clear(void);

public:
    unsigned   bits_per_physical_address;
    u64        base_address;
    unsigned   config_flags;
    unsigned   bitsPerPageOffset;     // vmem page size 2^bitsPerPageOffset
    unsigned   numberOfPagingLevels;  // top-down page table levels

    std::vector<CoreConfig>    core_devices;
    std::vector<MemoryConfig>  memory_devices;
    std::vector<RomConfig>     rom_devices;
    VideoConfig                video_device;
    DeviceArrayConfig          device_array_config;
    std::string                dump_file_name;
};

} // namespace postrisc
