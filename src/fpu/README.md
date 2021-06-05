
# soft fpu

## What it is:

This is a floating point software emulator.

was once forked by me from:

/*****************************************************************************

SoftFloat IEC/IEEE Floating-point Arithmetic Package, Release 2b. John R. Hauser.
International Computer Science Institute, Suite 600, 1947 Center Street, Berkeley, California 94704.
The original version was written as part of a project to build a fixed-point vector
processor in collaboration with the University of California at Berkeley,
overseen by Profs. Nelson Morgan and John Wawrzynek.  More information
is available through the Web page:
http://www.cs.berkeley.edu/~jhauser/arithmetic/SoftFloat.html

*****************************************************************************/

Has been since rewritten in C++, with the addition of new extended types (128, 256, 512 bits) and operations (fma, sqrt, etc).

Implements the IEEE-754 standard for 32, 64, 80, 128, 256, 512 bits.
Operations: add, sub, mul, div, fma, sqrt, conversion, rounding.
Support IEEE-754 standard: rounding modes, flags, quiet/signaled NANs, INFs, denormals, etc.

Was once used to invert the poorly conditioned matrices and other math.

## How is it tested?

Boost tests are used.
Testing is based on iterating the generated variants and comparing the results:

32, 64, 80 bits - comparison with hardware implementation
128 bits - comparison with gcc builtin float128 and libquadmath
all types 32, 64, 80, 128, 256, 512 - comparison with the alternative reference slow implementation.

source code for tests are placed near main code with ".test.cpp" suffixes.

## Test results

The project tests is not free from errors.
Some functions are abandoned during implementation (fp remainder).
Some functions have errors in the least significant bits (float512_sqrt).


## How to use tests (short boost tests help)

There are many tests in one build.

Use "unittest --list_content" to list individual tests.

and for example

Use "unittest --run_test=fpu/test/hard_soft/test_f64_add" to start testing f64 addition (hardware <-> optimized software compare).

Use "unittest --run_test=fpu/test/slow_soft/test_f256_add" to start testing f256 addition (reference slow naive software <-> optimized software compare).

Use "unittest --run_test=fpu/test/hard_slow/test_f32_add" to start testing f32 addition (hardware <-> etalon slow naive software).

Use "unittest --run_test=fpu/timing/soft/time_f256_add" to start performance testing f32 addition (optimized software compare).

Use "unittest --run_test=fpu/timing/hard/time_f256_add" to start performance testing f32 addition (reference hardware).

Use "unittest --run_test=fpu/timing/slow/time_f256_add" to start performance testing f32 addition (reference slow naive software).
