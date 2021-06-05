#pragma once

#include <future>

#include "assembler/assembler.hpp"
#include "emulator/IAddressSpace.hpp" // for CPhysAddress

namespace postrisc {

class IDoomInterface;

enum configuration_flags {
    CONFIG_TIMING         = 1U << 0,
    CONFIG_PROFILE        = 1U << 1,
    CONFIG_VERBOSE        = 1U << 2,
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
    CPhysAddress           phys_address;
    CVirtAddress           virt_address;
    std::vector<std::byte> data;
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

class Configuration {
public:
    Configuration(void);
    ~Configuration(void);
    int parse(int argc, char **argv);
    int run(int argc, char **argv);
    int usage(std::ostream& out) const;
    int do_run(IDoomInterface& doom, int argc, char **argv);

    void SetDefaults(void);
    void ExportDefinitions(CAssembler& assembler) const;
    unsigned get_config_flags(void) const { return m_config_flags; }
    void add_config_flags(unsigned flags) { m_config_flags |= flags; }
    u64 get_base_address(void) const { return m_base_address; }
    u32 get_ncores(void) const { return m_ncores; }
    unsigned get_bits_per_physical_address(void) const { return m_bits_per_physical_address; }
    unsigned get_bits_per_page_offset(void) const { return m_bitsPerPageOffset; }
    unsigned get_paging_levels(void) const { return m_numberOfPagingLevels; }
    const std::vector<MemoryConfig>& get_memory_devices(void) const { return m_memory_devices; }
    const std::vector<RomConfig>& get_rom_devices(void) const { return m_rom_devices; }
    const VideoConfig& get_video_device(void) const { return m_video_device; }
    const DeviceArrayConfig& get_device_array_config(void) const { return m_device_array_config; }

private:
    int do_version(void);
    int do_tokenize(void);
    int do_tokenize_html(void);
    int do_assemble(void);
    int do_disassemble(void);
    int do_dumpbin(void);
    int do_generate(void);
    int do_execute(void);
    int do_execute_app(IDoomInterface& doom, int argc, char**argv);
    int do_export_html(void);
    int do_export_llvm(void);
    int do_export_definitions(void);

    bool check(void);
    void Clear(void);

private:
    unsigned m_config_flags = 0;
    unsigned m_bits_per_physical_address = 44;
    unsigned m_bitsPerPageOffset = 12;    // vmem page size 2^bitsPerPageOffset
    unsigned m_numberOfPagingLevels = 3;  // top-down page table levels
    unsigned m_command = 0;
    unsigned m_codesize = 0;
    unsigned m_rodatasize = 0;
    unsigned m_datasize = 0;
    unsigned m_ncores = 1;
    u64 m_base_address = 0;
    std::vector<MemoryConfig> m_memory_devices;
    std::vector<RomConfig> m_rom_devices;
    VideoConfig m_video_device;
    DeviceArrayConfig m_device_array_config;
    std::string m_dump_file_name;
    std::string m_file_name;
    std::map<std::string, std::string> m_env;
    std::future<int> m_Task;
    CAssembler::assemble_mode m_assemble_mode = CAssembler::assemble_mode_obj;
};

void xhtml_header(std::ostream& out, const char *title, const char *description, bool colormap = false);

} // namespace postrisc
