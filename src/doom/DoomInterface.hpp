#pragma once

namespace postrisc {

class DoomColor {
public:
    u8 blue;
    u8 green;
    u8 red;
    u8 alpha;
};

class DoomInterface {
public:
    static size_t const width = 320;
    static size_t const height = 200;

    virtual ~DoomInterface(void) {}
    virtual int Init(unsigned resx, unsigned resy) = 0;
    virtual int DrawFrame(const u8 *screenbuffer, unsigned resx, unsigned resy, const DoomColor *colormap) = 0;
    virtual int GetKey(int* pressed, unsigned char* Key) = 0;
    virtual int SetWindowTitle(char const *title) = 0;

    size_t getFramesNum(void) const { return m_frames_num; }
    void IncFramesNum(void) { ++m_frames_num; }

private:
    size_t m_frames_num = 0;
};

} // namespace postrisc
