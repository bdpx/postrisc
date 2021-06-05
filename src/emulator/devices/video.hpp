#pragma once

#include "device.hpp"

namespace postrisc {

/***********************************************************************
* the video device config space
************************************************************************/
class VideoConfigSpace {
friend class boost::serialization::access;
public:
    VideoConfigSpace();
    template<class Archive> void serialize(Archive & ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader            header;
    uint32_t                     width;
    uint32_t                     height;
    uint32_t                     clear;
    uint32_t                     redraw;
    space_filler<uint64_t, 14>   reserved;
};

STATIC_ASSERT(sizeof(VideoConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
STATIC_ASSERT(std::is_standard_layout<VideoConfigSpace>::value);

template<class Archive>
void
VideoConfigSpace::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    archive & BOOST_SERIALIZATION_NVP(header);
    archive & BOOST_SERIALIZATION_NVP(width);
    archive & BOOST_SERIALIZATION_NVP(height);
    archive & BOOST_SERIALIZATION_NVP(clear);
    archive & BOOST_SERIALIZATION_NVP(redraw);
}

/***********************************************************************
* the video device
************************************************************************/
class device_video : public Device {
public:
    explicit device_video(TargetSystem *parent_, const std::string& name_, size_t width, size_t height);
    ~device_video(void) override;

    CStatus read(device_address_t address, size_t len, void* bytes) override;
    CStatus write(device_address_t address, size_t len, const void* bytes) override;
    CStatus complex_operation(device_address_t address, IMemoryOperation& op) override;
    CStatus read_config(device_address_t address, size_t len, void *bytes) override;
    CStatus write_config(device_address_t address, size_t len, const void *bytes) override;
    uint64_t size(void) const override;
    uint64_t get_config_size(void) const override;
    void dump_mapping(const PhysMemoryMapping& mapping, const DumpFormatter& out) const override;
    void dump_config(uint64_t address, const DumpFormatter& out) const override;
    void dump_statistic(const DumpFormatter& out) const override;

private:
    void clear(uint32_t color);
    void redraw(void);

private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & archive, const unsigned int version);

private:
    size_t                   m_Width;
    size_t                   m_Height;
    std::vector<uint8_t>     m_Data;
    VideoConfigSpace         config;
};

template<class Archive>
void
device_video::serialize(Archive & archive, const unsigned int UNUSED(version))
{
    LOG_TRACE(LOG_SERIALIZATION) << "enter";

    archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);

    archive & BOOST_SERIALIZATION_NVP(m_Width);
    archive & BOOST_SERIALIZATION_NVP(m_Height);
    archive & BOOST_SERIALIZATION_NVP(m_Data);
}

} // namespace postrisc
