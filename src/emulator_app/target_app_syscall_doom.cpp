#include <thread>

#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "emulator/core.hpp"
#include "disassembler/decoder.hpp"
#include "target_app.hpp"

namespace postrisc {

u64 TargetSingleApp::system_doom_Init(u64 resx, u64 resy)
{
    LOG_DEBUG(LOG_DOOM) << "enter"
        << " resx=" << resx
        << " resy=" << resy;

    int const ret  = doom_interface.Init(resx, resy);
    if (ret < 0) {
       syscall_force_fail = true;
       return -1;
    }
    return 0;
}

u64 TargetSingleApp::system_doom_DrawFrame(u64 screenbuffer, u64 resx, u64 resy)
{
    LOG_DEBUG(LOG_DOOM) << "enter:"
        << " screenbuffer=" << fmt::hex(screenbuffer)
        << " resx=" << resx
        << " resy=" << resy;

    m_frames_num++;
    int const ret  = doom_interface.DrawFrame((u32 const *)screenbuffer, resx, resy);
    if (ret < 0) {
       syscall_force_fail = true;
       return -1;
    }
    return 0;
}

u64 TargetSingleApp::system_doom_SleepMs(u64 ms)
{
    LOG_DEBUG(LOG_DOOM) << "sleep: ms=" << ms;
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return 0;
}

u64 TargetSingleApp::system_doom_GetTicksMs(void)
{
    LOG_DEBUG(LOG_DOOM) << "enter";
    std::chrono::steady_clock::time_point const end = std::chrono::steady_clock::now();
    auto const diff = end - start_time;
    const u64 run_time = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    return run_time;
}

u64 TargetSingleApp::system_doom_GetKey(u64 _pressed, u64 _doomKey)
{
    LOG_DEBUG(LOG_DOOM) << "enter";
    int* pressed = (int*)_pressed;
    unsigned char* doomKey = (unsigned char*)_doomKey;
    return doom_interface.GetKey(pressed, doomKey);
}

u64 TargetSingleApp::system_doom_SetWindowTitle(u64 _title)
{
    const char * title = (const char *)_title;
    LOG_DEBUG(LOG_DOOM) << (title ? title : "null");
    doom_interface.SetWindowTitle(title);
    return 0;
}

} // namespace postrisc
