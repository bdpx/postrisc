#pragma once

#include "DoomInterface.hpp"

namespace postrisc {

class CliDoomInterface : public DoomInterface {
public:
    CliDoomInterface(void) {}
    ~CliDoomInterface(void) override {}

    int Init(unsigned resx, unsigned resy) override {
        std::cerr << "CLI init: " << resx << 'x' << resy << std::endl;
        return 0;
    }

    int DrawFrame(const u8 *UNUSED(screenbuffer), unsigned resx, unsigned resy, const DoomColor *UNUSED(colormap)) override {
        std::cerr << "CLI frame: " << resx << ',' << resy << std::endl;
        return 0;
    }

    int GetKey(int* UNUSED(pressed), u8* UNUSED(key)) override {
        return 0;
    }

    int SetWindowTitle(char const *title) override {
        std::cerr << "CLI title: " << title << std::endl;
        return 0;
    }
};

} // namespace postrisc
