#pragma once

/******************************************************************************
* global test fixture
******************************************************************************/

namespace postrisc {
namespace fpu {

class TestController {
public:
    TestController(void);
    ~TestController(void);

    void setup();
    void teardown();
    static void checkEarlyExit(u64& count, u64 errorCount);
    static void catchSIGINT(int signalCode);
    static void writeTestsTotal(u64 count);
    static void writeTestsPerformed(u64 count, u64 errorCount);
    static bool GetHardwareTininessMode(void);

    template<typename FPU> static int CheckHardwareTininessMode(FPU& fpu);

private:
    static int ParseCommandLine(int argc, char **argv);
    static void abort_testing(void);

private:
    static int   stop;
    static bool  is_cout_tty;
    static bool    errorStop;

public:
    static bool    checkNaNs;
    static bool    checkFlags;
    static bool    forever;
    static u32     maxErrorCount;
};

} // namespace fpu
} // namespace postrisc
