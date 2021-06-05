#pragma once

#include <atomic>

#include "emulator/Device.hpp"

namespace postrisc {

/***********************************************************************
* the video device config space
************************************************************************/
class VideoConfigSpace {
friend class boost::serialization::access;
public:
    VideoConfigSpace();
    template<typename Archive> void serialize(Archive& ar, const unsigned int version);
    void dump(const DumpFormatter& out) const;

public:
    ConfigSpaceHeader       header;
    u32                     width = 0;
    u32                     height = 0;
    u32                     clear = 0;
    u32                     redraw = 0;
    u32                     foreground_color = 0;
    u32                     background_color = 0;
    u32                     character_pos_x = 0;
    u32                     character_pos_y = 0;
    u32                     character_value = 0;
    u32                     pad = 0;
    space_filler<u64, 11>   reserved;
};

static_assert(sizeof(VideoConfigSpace) <= DEVICE_CONFIG_SPACE_SIZE);
static_assert(std::is_standard_layout<VideoConfigSpace>::value);

template<typename Archive>
void
VideoConfigSpace::serialize(Archive& archive, const unsigned int UNUSED(version))
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
class DeviceVideo final : public Device {
public:
    explicit DeviceVideo(const std::string& name_, size_t width, size_t height);
    ~DeviceVideo(void) override;

    STANDARD_DEVICE_INTERFACE

    size_t width(void) const { return m_Width; }
    size_t height(void) const { return m_Height; }
    std::byte const *data(void) const { return m_Data.data(); }
    void CheckRedraw(size_t &width, size_t &height);

private:
    void clear(u32 color);
    void redraw(void);

private:
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive& archive, const unsigned int version);

private:
    size_t                   m_Width;
    size_t                   m_Height;
    std::vector<std::byte>   m_Data;
    std::atomic<u32>         m_Redraw;
    VideoConfigSpace         config;
};

template<typename Archive>
void DeviceVideo::serialize(Archive& archive, const unsigned int UNUSED(version))
{
    LOG_DEBUG(LOG_SERIALIZATION) << "enter";

    archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Device);

    archive & BOOST_SERIALIZATION_NVP(m_Width);
    archive & BOOST_SERIALIZATION_NVP(m_Height);
    archive & BOOST_SERIALIZATION_NVP(m_Data);
}

} // namespace postrisc
