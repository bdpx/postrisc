#pragma once

/******************************************************************************
* global test fixture
******************************************************************************/

namespace postrisc {
namespace fpu {

class Controller {
public:
    Controller(void);
    ~Controller(void);

    void setup();
    void teardown();
    static void checkEarlyExit(u64 count, u64 errorCount);
    static void catchSIGINT(int signalCode);
    static void writeTestsTotal(const char *name);
    static void writeTestsPerformed(u64 count, u64 errorCount);
    static bool GetHardwareTininessMode(void);

    template<typename FPU> static int CheckHardwareTininessMode(FPU& fpu);

    static bool isEnoughErrors(u32 errorCount) {
        return errorCount >= s_maxErrorCount && s_maxErrorCount != 0;
    }

private:
    static int ParseCommandLine(int argc, char **argv);

private:
    static u32   s_stop;
    static bool  s_isatty;

public:
    static bool  s_checkNaNs;
    static bool  s_checkIntNaNs;
    static bool  s_checkFlags;
    static bool  s_forever;
    static u32   s_reportInterval;
    static u64   s_maxErrorCount;
    static u64   s_maxCount;
};

} // namespace fpu
} // namespace postrisc
