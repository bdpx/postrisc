#pragma once

namespace postrisc {

class IDoomInterface {
public:
    virtual ~IDoomInterface(void) {}
    virtual int Init(unsigned resx, unsigned resy) const = 0;
    virtual int DrawFrame(u32 const *screenbuffer, unsigned resx, unsigned resy) const = 0;
    virtual int GetKey(int* pressed, unsigned char* Key) const = 0;
    virtual int SetWindowTitle(char const *title) const = 0;
};

} // namespace postrisc
