#include <QPainter>
#include <QScreen>
#include <QDebug>
#include <QThread>

#include "util/common.hpp"
#include "emulator_system/target_system.hpp"
#include "FrameBufferWindow.hpp"

#define GB_WIDTH   1024 // 640
#define GB_HEIGTH  768  // 480

namespace postrisc {

KeyRing KeyRing::s_KeyRing;

void KeyRing::addKeyToQueue(bool pressed, Qt::Key keyCode)
{
    std::lock_guard<std::mutex> lock(m_lock);

    m_KeyQueue[m_KeyQueueWriteIndex].keyCode = keyCode;
    m_KeyQueue[m_KeyQueueWriteIndex].pressed = pressed;
    m_KeyQueueWriteIndex++;
    m_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

bool KeyRing::GetKey(bool& pressed, Qt::Key& keyCode)
{
    std::lock_guard<std::mutex> lock(m_lock);

    if (m_KeyQueueReadIndex == m_KeyQueueWriteIndex) {
        //key queue is empty
        return false;
    } else {
        pressed = m_KeyQueue[m_KeyQueueReadIndex].pressed;
        keyCode = m_KeyQueue[m_KeyQueueReadIndex].keyCode;
        m_KeyQueueReadIndex++;
        m_KeyQueueReadIndex %= KEYQUEUE_SIZE;
        return true;
    }
}

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

#define RGB(R,G,B)  ((u32(R) << 16) | (u32(G) << 8) | u32(B))

// OEM 437 vga 8x16
static const u8 codepage[256 * FONT_HEIGHT]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7e,0x81,0xa5,0x81,0x81,0xa5,0x99,0x81,0x81,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x7e,0xff,0xdb,0xff,0xff,0xdb,0xe7,0xff,0xff,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x36,0x7f,0x7f,0x7f,0x7f,0x3e,0x1c,0x08,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x08,0x1c,0x3e,0x7f,0x3e,0x1c,0x08,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x18,0x3c,0x3c,0xe7,0xe7,0xe7,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x18,0x3c,0x7e,0xff,0xff,0x7e,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3c,0x3c,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xe7,0xc3,0xc3,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0x42,0x42,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xc3,0x99,0xbd,0xbd,0x99,0xc3,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x78,0x60,0x70,0x58,0x1e,0x33,0x33,0x33,0x33,0x1e,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x66,0x66,0x66,0x66,0x3c,0x18,0x7e,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0xfc,0xcc,0xfc,0x0c,0x0c,0x0c,0x0c,0x0e,0x0f,0x07,0x00,0x00,0x00,0x00,
0x00,0x00,0xfe,0xc6,0xfe,0xc6,0xc6,0xc6,0xc6,0xe6,0xe7,0x67,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x18,0x18,0xdb,0x3c,0xe7,0x3c,0xdb,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x01,0x03,0x07,0x0f,0x1f,0x7f,0x1f,0x0f,0x07,0x03,0x01,0x00,0x00,0x00,0x00,
0x00,0x40,0x60,0x70,0x78,0x7c,0x7f,0x7c,0x78,0x70,0x60,0x40,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x66,0x66,0x00,0x00,0x00,0x00,
0x00,0x00,0xfe,0xdb,0xdb,0xdb,0xde,0xd8,0xd8,0xd8,0xd8,0xd8,0x00,0x00,0x00,0x00,
0x00,0x3e,0x63,0x06,0x1c,0x36,0x63,0x63,0x36,0x1c,0x30,0x63,0x3e,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x7f,0x7f,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x7e,0x3c,0x18,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x3c,0x7e,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7e,0x3c,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x18,0x30,0x7f,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x0c,0x06,0x7f,0x06,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x14,0x36,0x7f,0x36,0x14,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x08,0x1c,0x1c,0x3e,0x3e,0x7f,0x7f,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7f,0x7f,0x3e,0x3e,0x1c,0x1c,0x08,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x3c,0x3c,0x3c,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x36,0x36,0x7f,0x36,0x36,0x36,0x7f,0x36,0x36,0x00,0x00,0x00,0x00,
0x18,0x18,0x3e,0x63,0x43,0x03,0x3e,0x60,0x60,0x61,0x63,0x3e,0x18,0x18,0x00,0x00,
0x00,0x00,0x00,0x00,0x43,0x63,0x30,0x18,0x0c,0x06,0x63,0x61,0x00,0x00,0x00,0x00,
0x00,0x00,0x1c,0x36,0x36,0x1c,0x6e,0x3b,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x0c,0x0c,0x0c,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x30,0x18,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x18,0x30,0x00,0x00,0x00,0x00,
0x00,0x00,0x0c,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x0c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x66,0x3c,0xff,0x3c,0x66,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x0c,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x40,0x60,0x30,0x18,0x0c,0x06,0x03,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x1c,0x36,0x63,0x63,0x6b,0x6b,0x63,0x63,0x36,0x1c,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x1c,0x1e,0x18,0x18,0x18,0x18,0x18,0x18,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x60,0x30,0x18,0x0c,0x06,0x03,0x63,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x60,0x60,0x3c,0x60,0x60,0x60,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x30,0x38,0x3c,0x36,0x33,0x7f,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x03,0x03,0x03,0x3f,0x60,0x60,0x60,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x1c,0x06,0x03,0x03,0x3f,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x63,0x60,0x60,0x30,0x18,0x0c,0x0c,0x0c,0x0c,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x63,0x63,0x3e,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x63,0x63,0x7e,0x60,0x60,0x60,0x30,0x1e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x0c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x0c,0x18,0x30,0x60,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x06,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x06,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x63,0x30,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x3e,0x63,0x63,0x7b,0x7b,0x7b,0x3b,0x03,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x08,0x1c,0x36,0x63,0x63,0x7f,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x3f,0x66,0x66,0x66,0x3e,0x66,0x66,0x66,0x66,0x3f,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x66,0x43,0x03,0x03,0x03,0x03,0x43,0x66,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x1f,0x36,0x66,0x66,0x66,0x66,0x66,0x66,0x36,0x1f,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x66,0x46,0x16,0x1e,0x16,0x06,0x46,0x66,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x66,0x46,0x16,0x1e,0x16,0x06,0x06,0x06,0x0f,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x66,0x43,0x03,0x03,0x7b,0x63,0x63,0x66,0x5c,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x63,0x63,0x63,0x7f,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x33,0x33,0x33,0x1e,0x00,0x00,0x00,0x00,
0x00,0x00,0x67,0x66,0x66,0x36,0x1e,0x1e,0x36,0x66,0x66,0x67,0x00,0x00,0x00,0x00,
0x00,0x00,0x0f,0x06,0x06,0x06,0x06,0x06,0x06,0x46,0x66,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x77,0x7f,0x7f,0x6b,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x67,0x6f,0x7f,0x7b,0x73,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x3f,0x66,0x66,0x66,0x3e,0x06,0x06,0x06,0x06,0x0f,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x63,0x6b,0x7b,0x3e,0x30,0x70,0x00,0x00,
0x00,0x00,0x3f,0x66,0x66,0x66,0x3e,0x36,0x66,0x66,0x66,0x67,0x00,0x00,0x00,0x00,
0x00,0x00,0x3e,0x63,0x63,0x06,0x1c,0x30,0x60,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x7e,0x7e,0x5a,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x36,0x1c,0x08,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x63,0x63,0x63,0x6b,0x6b,0x6b,0x7f,0x77,0x36,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x63,0x36,0x3e,0x1c,0x1c,0x3e,0x36,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x66,0x66,0x66,0x66,0x3c,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x63,0x61,0x30,0x18,0x0c,0x06,0x43,0x63,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x03,0x07,0x0e,0x1c,0x38,0x70,0x60,0x40,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3c,0x00,0x00,0x00,0x00,
0x08,0x1c,0x36,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
0x0c,0x0c,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x1e,0x30,0x3e,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x06,0x06,0x1e,0x36,0x66,0x66,0x66,0x66,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3e,0x63,0x03,0x03,0x03,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x38,0x30,0x30,0x3c,0x36,0x33,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3e,0x63,0x7f,0x03,0x03,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x1c,0x36,0x26,0x06,0x0f,0x06,0x06,0x06,0x06,0x0f,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x6e,0x33,0x33,0x33,0x33,0x33,0x3e,0x30,0x33,0x1e,0x00,
0x00,0x00,0x07,0x06,0x06,0x36,0x6e,0x66,0x66,0x66,0x66,0x67,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x18,0x00,0x1c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x60,0x60,0x00,0x70,0x60,0x60,0x60,0x60,0x60,0x60,0x66,0x66,0x3c,0x00,
0x00,0x00,0x07,0x06,0x06,0x66,0x36,0x1e,0x1e,0x36,0x66,0x67,0x00,0x00,0x00,0x00,
0x00,0x00,0x1c,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x37,0x7f,0x6b,0x6b,0x6b,0x6b,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3b,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3b,0x66,0x66,0x66,0x66,0x66,0x3e,0x06,0x06,0x0f,0x00,
0x00,0x00,0x00,0x00,0x00,0x6e,0x33,0x33,0x33,0x33,0x33,0x3e,0x30,0x30,0x78,0x00,
0x00,0x00,0x00,0x00,0x00,0x3b,0x6e,0x66,0x06,0x06,0x06,0x0f,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x3e,0x63,0x06,0x1c,0x30,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x08,0x0c,0x0c,0x3f,0x0c,0x0c,0x0c,0x0c,0x6c,0x38,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3c,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x6b,0x6b,0x6b,0x7f,0x36,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x63,0x36,0x1c,0x1c,0x1c,0x36,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x7e,0x60,0x30,0x1f,0x00,
0x00,0x00,0x00,0x00,0x00,0x7f,0x33,0x18,0x0c,0x06,0x63,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x70,0x18,0x18,0x18,0x0e,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00,
0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x0e,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0e,0x00,0x00,0x00,0x00,
0x00,0x00,0x6e,0x3b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x08,0x1c,0x36,0x63,0x63,0x63,0x7f,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x3c,0x66,0x43,0x03,0x03,0x03,0x43,0x66,0x3c,0x30,0x60,0x3e,0x00,0x00,
0x00,0x00,0x33,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x30,0x18,0x0c,0x00,0x3e,0x63,0x7f,0x03,0x03,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x08,0x1c,0x36,0x00,0x1e,0x30,0x3e,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x33,0x00,0x00,0x1e,0x30,0x3e,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x06,0x0c,0x18,0x00,0x1e,0x30,0x3e,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x1c,0x36,0x1c,0x00,0x1e,0x30,0x3e,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3c,0x66,0x06,0x06,0x66,0x3c,0x30,0x60,0x3c,0x00,0x00,0x00,
0x00,0x08,0x1c,0x36,0x00,0x3e,0x63,0x7f,0x03,0x03,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x00,0x00,0x3e,0x63,0x7f,0x03,0x03,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x06,0x0c,0x18,0x00,0x3e,0x63,0x7f,0x03,0x03,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x66,0x00,0x00,0x1c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x18,0x3c,0x66,0x00,0x1c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x06,0x0c,0x18,0x00,0x1c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x63,0x00,0x08,0x1c,0x36,0x63,0x63,0x7f,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x1c,0x36,0x1c,0x00,0x1c,0x36,0x63,0x63,0x7f,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x18,0x0c,0x06,0x00,0x7f,0x66,0x06,0x3e,0x06,0x06,0x66,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x33,0x6e,0x6c,0x7e,0x1b,0x1b,0x76,0x00,0x00,0x00,0x00,
0x00,0x00,0x7c,0x36,0x33,0x33,0x7f,0x33,0x33,0x33,0x33,0x73,0x00,0x00,0x00,0x00,
0x00,0x08,0x1c,0x36,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x00,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x06,0x0c,0x18,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x0c,0x1e,0x33,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x06,0x0c,0x18,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x63,0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x7e,0x60,0x30,0x1e,0x00,
0x00,0x63,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x63,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x18,0x18,0x3c,0x66,0x06,0x06,0x06,0x66,0x3c,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x1c,0x36,0x26,0x06,0x0f,0x06,0x06,0x06,0x06,0x67,0x3f,0x00,0x00,0x00,0x00,
0x00,0x00,0x66,0x66,0x3c,0x18,0x7e,0x18,0x7e,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x1f,0x33,0x33,0x1f,0x23,0x33,0x7b,0x33,0x33,0x33,0x63,0x00,0x00,0x00,0x00,
0x00,0x70,0xd8,0x18,0x18,0x18,0x7e,0x18,0x18,0x18,0x18,0x18,0x1b,0x0e,0x00,0x00,
0x00,0x18,0x0c,0x06,0x00,0x1e,0x30,0x3e,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x30,0x18,0x0c,0x00,0x1c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,
0x00,0x18,0x0c,0x06,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x18,0x0c,0x06,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x6e,0x3b,0x00,0x3b,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,
0x6e,0x3b,0x00,0x63,0x67,0x6f,0x7f,0x7b,0x73,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x3c,0x36,0x36,0x7c,0x00,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x1c,0x36,0x36,0x1c,0x00,0x3e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0c,0x0c,0x00,0x0c,0x0c,0x06,0x03,0x63,0x63,0x3e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0x03,0x43,0x63,0x33,0x18,0x0c,0x06,0x3b,0x61,0x30,0x18,0x7c,0x00,0x00,
0x00,0x03,0x03,0x43,0x63,0x33,0x18,0x0c,0x66,0x73,0x79,0x7c,0x60,0x60,0x00,0x00,
0x00,0x00,0x18,0x18,0x00,0x18,0x18,0x18,0x3c,0x3c,0x3c,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x6c,0x36,0x1b,0x36,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x1b,0x36,0x6c,0x36,0x1b,0x00,0x00,0x00,0x00,0x00,0x00,
0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,
0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,
0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x1f,0x18,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6f,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7f,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x00,0x00,0x00,0x00,0x00,0x1f,0x18,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6f,0x60,0x6f,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x00,0x00,0x00,0x00,0x00,0x7f,0x60,0x6f,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6f,0x60,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x18,0x18,0x18,0x18,0x18,0x1f,0x18,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0xf8,0x18,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0xec,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0xec,0x0c,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xfc,0x0c,0xec,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0xef,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xef,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0xec,0x0c,0xec,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x6c,0x6c,0x6c,0x6c,0x6c,0xef,0x00,0xef,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x18,0x18,0x18,0x18,0x18,0xff,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xff,0x00,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x18,0x18,0x18,0x18,0x18,0xf8,0x18,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xf8,0x18,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0xff,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,0x6c,
0x18,0x18,0x18,0x18,0x18,0xff,0x18,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x6e,0x3b,0x1b,0x1b,0x1b,0x3b,0x6e,0x00,0x00,0x00,0x00,
0x00,0x00,0x1e,0x33,0x33,0x33,0x1b,0x33,0x63,0x63,0x63,0x33,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x63,0x63,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7f,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x7f,0x63,0x06,0x0c,0x18,0x0c,0x06,0x63,0x7f,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x7e,0x1b,0x1b,0x1b,0x1b,0x1b,0x0e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3e,0x06,0x06,0x03,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x6e,0x3b,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x7e,0x18,0x3c,0x66,0x66,0x66,0x3c,0x18,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x1c,0x36,0x63,0x63,0x7f,0x63,0x63,0x36,0x1c,0x00,0x00,0x00,0x00,
0x00,0x00,0x1c,0x36,0x63,0x63,0x63,0x36,0x36,0x36,0x36,0x77,0x00,0x00,0x00,0x00,
0x00,0x00,0x78,0x0c,0x18,0x30,0x7c,0x66,0x66,0x66,0x66,0x3c,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x7e,0xdb,0xdb,0xdb,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xc0,0x60,0x7e,0xdb,0xdb,0xcf,0x7e,0x06,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x38,0x0c,0x06,0x06,0x3e,0x06,0x06,0x06,0x0c,0x38,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x3e,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7f,0x00,0x00,0x7f,0x00,0x00,0x7f,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x7e,0x18,0x18,0x00,0x00,0xff,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x00,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x30,0x18,0x0c,0x06,0x0c,0x18,0x30,0x00,0x7e,0x00,0x00,0x00,0x00,
0x00,0x00,0x70,0xd8,0xd8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1b,0x1b,0x1b,0x0e,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x7e,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x6e,0x3b,0x00,0x6e,0x3b,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x1c,0x36,0x36,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xf0,0x30,0x30,0x30,0x30,0x30,0x37,0x36,0x36,0x3c,0x38,0x00,0x00,0x00,0x00,
0x00,0x1b,0x36,0x36,0x36,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0e,0x1b,0x0c,0x06,0x13,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x3e,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


class CFrameBuffer {
public:
    CFrameBuffer(size_t _w, size_t _h) : fb(_w * _h, 0), w(_w), h(_h) {}

    u8 const *GetData(void) const { return reinterpret_cast<u8 const*>(fb.data()); }
    size_t width(void) const { return w; };
    size_t height(void) const { return h; };
    u32& ref(size_t x, size_t y) { return fb[y * width() + x]; }

    void DrawLogo(u32 frame);
    void PutChar(u8 c, unsigned x, unsigned y, u32 color);
    void PutChar(u8 c, unsigned x, unsigned y, u32 color, unsigned scale);
    void PutString(unsigned x, unsigned y, u32 color, char const *str);
    void PutString(unsigned x, unsigned y, u32 color, char const *str, unsigned scale);

private:
    std::vector<u32> fb;
    size_t w;
    size_t h;
};

void
CFrameBuffer::PutChar(u8 c, unsigned px, unsigned py, u32 color)
{
    for (size_t y = 0; y < FONT_HEIGHT && py + y < height(); y++) {
        u8 const mask = codepage[FONT_HEIGHT * c + y];
        for (size_t x = 0; x < FONT_WIDTH && px + x < width(); x++) {
            if (mask & (1 << x)) {
                ref(px + x, py + y) = color;
            }
        }
    }
}

void
CFrameBuffer::PutChar(u8 c, unsigned px, unsigned py, u32 color, unsigned scale)
{
    for (size_t y = 0; y < FONT_HEIGHT && py + scale*y < height(); y++) {
        u8 const mask = codepage[FONT_HEIGHT * c + y];
        for (size_t x = 0; x < FONT_WIDTH && px + scale*x < width(); x++) {
            if (mask & (1 << x)) {
                for (size_t i=0; i<scale && py + scale*y+i < height(); i++)
                    for (size_t j=0; j<scale && px + scale*x+j < width(); j++)
                        ref(px + scale*x+j, py + scale*y+i) = color;
            }
        }
    }
}

void
CFrameBuffer::PutString(unsigned x, unsigned y, u32 color, char const *str)
{
    while (*str && x < width()) {
        PutChar(*str, x, y, color);
        str++;
        x += FONT_WIDTH;
    }
}

void
CFrameBuffer::PutString(unsigned x, unsigned y, u32 color, char const *str, unsigned scale)
{
    while (*str && x < width()) {
        PutChar(*str, x, y, color, scale);
        str++;
        x += FONT_WIDTH * scale;
    }
}

void
CFrameBuffer::DrawLogo(u32 frame)
{
    for (size_t y = 0; y < height(); y++) {
        for (size_t x = 0; x < width(); x++) {
            u8 const red   = ((x + y + frame + 111) & 0xF0) + 0x0F;
            u8 const green = ((x + y + frame + 911) & 0xF0) + 0x0F;
            u8 const blue  = ((x + y + frame + 511) & 0xF0) + 0x0F;
            //image.setPixel(i, j, qRgba(r, g, b, a));
            ref(x,y) = RGB(red, green, blue);
        }
    }

    unsigned px = 10;
    unsigned py = 10;
    for (size_t y = 0; y < 8; y++, py += FONT_HEIGHT - 2) {
        for (size_t x = 0; x < 32; x++) {
            PutChar(y*32 + x, px + FONT_WIDTH * x, py, RGB(0,0,0));
        }
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "Welcome to POSTRISC: %u", frame);
    PutString(px, py, RGB(55,55,0), buf, 5);
}

FrameBufferWindow::FrameBufferWindow(TargetSystem& targetSystem, QWidget *parent)
    : QGraphicsView(parent), _parent(parent)
    , m_frame(0)
    , m_is_mouse_left_button_pressed(false)
    , m_TargetSystem(targetSystem)
{
    setAutoFillBackground(false);
    move(0, 0);

    _scene = std::make_unique<QGraphicsScene>(this);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(_scene.get());

    // connect(this, SIGNAL(finished(QImage)), this, SLOT(applyImage(QImage)), Qt::QueuedConnection);
}

FrameBufferWindow::~FrameBufferWindow()
{
}

void
FrameBufferWindow::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) {
        //qDebug() << "left press: " << ev->pos().x() << ' ' << ev->pos().y();
        m_is_mouse_left_button_pressed = true;
    }
}

void
FrameBufferWindow::mouseMoveEvent(QMouseEvent* UNUSED(ev))
{
    if (!m_is_mouse_left_button_pressed) {
        return;
    }
    //qDebug() << "move: " << ev->pos().x() << ' ' << ev->pos().y();
}

void
FrameBufferWindow::mouseReleaseEvent(QMouseEvent* UNUSED(ev))
{
    m_is_mouse_left_button_pressed = false;
    //qDebug() << "release: " << ev->pos().x() << ' ' << ev->pos().y();
}

void FrameBufferWindow::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "key pressed: " << event->key();
    KeyRing::s_KeyRing.addKeyToQueue(true, (Qt::Key)event->key());
}

void FrameBufferWindow::keyReleaseEvent(QKeyEvent *event)
{
    //qDebug() << "key released: " << event->key();
    KeyRing::s_KeyRing.addKeyToQueue(false, (Qt::Key)event->key());
}

void
FrameBufferWindow::updateGbScreen(void)
{
    CFrameBuffer frameBuffer(GB_WIDTH, GB_HEIGTH);
    // QImage image = QImage(GB_WIDTH, GB_HEIGTH, QImage::Format_RGBA8888);
    frameBuffer.DrawLogo(m_frame);

    QImage image(frameBuffer.GetData(),
                 frameBuffer.width(),
                 frameBuffer.height(), QImage::Format_RGB32); // Format_RGBA8888

#if 0
    // try to scale but only pixel-perfect
    size_t factor = 1;
    while ((factor + 1) * frameBuffer.width() <= (size_t)width() &&
           (factor + 1) * frameBuffer.height() <= (size_t)height()) {
        ++factor;
    }
    image = image.scaled(factor*frameBuffer.width(), factor*frameBuffer.height(), Qt::KeepAspectRatio);
    image = image.copy(0, 0, std::min<size_t>(factor*frameBuffer.width(), width()),
                             std::min<size_t>(factor*frameBuffer.height(), height()));
#endif
    image = image.scaled(width(), height());

    QPixmap p = QPixmap::fromImage(image);
    /*
    if (_do_resize) {
        resize(image.width(), image.height());
        move(QPoint(0, //(width() - image.width()) / 2,
                0) //(height() - image.height()) / 2
               );
        _do_resize = false;
    }
    */
    _scene->clear();
    _scene->addPixmap(p);
    ++m_frame;
}

void FrameBufferWindow::applyImage(const QImage& image)
{
    // qDebug() << QThread::currentThread() << " applyImage!";

    // try to scale but only pixel-perfect
    size_t factor = 1;
    while ((factor + 1) * image.width() <= (size_t)width() &&
           (factor + 1) * image.height() <= (size_t)height()) {
        ++factor;
    }
    QImage image2 = image.scaled(factor*image.width(), factor*image.height(), Qt::KeepAspectRatio);
    //image2 = image2.copy(0, 0, std::min<size_t>(factor*image.width(), width()),
    //                         std::min<size_t>(factor*image.height(), height()));
    // or stretch to window
    // image = image.scaled(width(), height());

    QPixmap p = QPixmap::fromImage(image2);
    _scene->clear();
    _scene->addPixmap(p);
    ++m_frame;
}

} // namespace postrisc