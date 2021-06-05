#pragma once

#include "doom_interface.hpp"

namespace postrisc {

class CliDoomInterface : public IDoomInterface {
public:
    CliDoomInterface() {}
    ~CliDoomInterface(void) override {}
    int Init(unsigned resx, unsigned resy) const override;
    int DrawFrame(u32 const *screenbuffer, unsigned resx, unsigned resy) const override;
    int GetKey(int* pressed, u8* key) const override;
    int SetWindowTitle(char const *title) const override;
};

inline int
CliDoomInterface::Init(unsigned resx, unsigned resy) const
{
    std::cerr << "CLI init: " << resx << ',' << resy << std::endl;
    return 0;
}

inline int
CliDoomInterface::DrawFrame(u32 const *UNUSED(screenbuffer), unsigned resx, unsigned resy) const
{
    std::cerr << "CLI frame: " << resx << ',' << resy << std::endl;
    return 0;
}

inline int
CliDoomInterface::GetKey(int* UNUSED(pressed), u8* UNUSED(Key)) const
{
    return 0;
}

inline int
CliDoomInterface::SetWindowTitle(char const *title) const
{
    std::cerr << "CLI title: " << title << std::endl;
    return 0;
}

} // namespace postrisc
