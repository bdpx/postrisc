#pragma once

namespace postrisc {

class DoomColor {
public:
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
};

class IDoomInterface {
public:
    virtual ~IDoomInterface(void) {}
    virtual int Init(unsigned resx, unsigned resy) const = 0;
    virtual int DrawFrame(const u8 *screenbuffer, unsigned resx, unsigned resy, const DoomColor *colormap) const = 0;
    virtual int GetKey(int* pressed, unsigned char* Key) const = 0;
    virtual int SetWindowTitle(char const *title) const = 0;
};

} // namespace postrisc
