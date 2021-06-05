#pragma once

/*******************************************************************
macrotable for tested functions

 X(NumInputs,                      - number of arguments
   Templt,                         - template name for testing (6 templates)
                                                i_f     - FLOAT func (INT)
                                                f_i     - INT func (FLOAT)
                                                f_f     - FLOAT func (FLOAT)
                                                af_f    - FLOAT func (ABS(FLOAT))
                                                ff_f    - FLOAT func (FLOAT, FLOAT)
                                                ff_b    - BOOL func (FLOAT, FLOAT)
                                                fff_f   - FLOAT func (FLOAT, FLOAT, FLOAT, muladd_mode_t)

   supported,                      - support modes mask (hardware, testsoft) for testing
   roundingPrecision,              - result varied by rounding precision, 80-bit only
   roundingMode,                   - result varied by rounding mode
   tininessMode,                   - result varied by tininess mode
   tininessModeAtReducedPrecision, - result varied by tininess mode at reduced precision, 80-bit only
   T1,                             - source type
   T2,                             - result type
   method                          - function name
*******************************************************************/

namespace postrisc {
namespace fpu {

// number of args
enum {
  args_f1 = 1,
  args_a1 = 1,
  args_f2 = 2,
  args_f3 = 3
};

// supported test mode flags
enum {
   SUPPORTED_NONE = 0,
   SUPPORTED_HARD = 1,
   SUPPORTED_SOFT = 2,
   SUPPORTED_BOTH = SUPPORTED_HARD | SUPPORTED_SOFT,
};


#define DECLARE_HARDWARE_FUNCTIONS_TABLE(X) \
    X(f1, i_f,    BOTH, 0, 1, 0, 0,  i32,   f32,   to_f32) \
    X(f1, i_f,    BOTH, 0, 1, 0, 0,  i64,   f32,   to_f32) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f32,       i32,   to_i32) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f32,       i32,   to_i32_round_to_zero) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f32,       i64,   to_i64) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f32,       i64,   to_i64_round_to_zero) \
    X(f1, f_f,    BOTH, 0, 1, 0, 0,  f32,       f32,       round_to_int) \
    X(a1, af_f,   BOTH, 0, 1, 0, 0,  f32,       f32,       sqrt) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f32,       f32,       add) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f32,       f32,       sub) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f32,       f32,       mul) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f32,       f32,       div) \
    X(f2, ff_f,   BOTH, 0, 0, 0, 0,  f32,       f32,       rem) \
    X(f3, fff_f,  BOTH, 0, 1, 1, 0,  f32,       f32,       muladd) \
    X(a1, af_f,   HARD, 0, 1, 0, 0,  f32,       f32,       log2) \
    X(f1, f_f,    HARD, 0, 1, 0, 0,  f32,       f32,       exp2) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f32,       bool,  eq_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f32,       bool,  le_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f32,       bool,  lt_signaling) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f32,       bool,  eq_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f32,       bool,  le_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f32,       bool,  lt_quiet) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i32,   f64,   to_f64) \
    X(f1, i_f,    BOTH, 0, 1, 0, 0,  i64,   f64,   to_f64) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f64,       i32,   to_i32) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f64,       i32,   to_i32_round_to_zero) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f64,       i64,   to_i64) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f64,       i64,   to_i64_round_to_zero) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f32,       f64,   to_f64) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f64,       f32,   to_f32) \
    X(f1, f_f,    BOTH, 0, 1, 0, 0,  f64,       f64,   round_to_int) \
    X(a1, af_f,   BOTH, 0, 1, 0, 0,  f64,       f64,   sqrt) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f64,       f64,   add) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f64,       f64,   sub) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f64,       f64,   mul) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f64,       f64,   div) \
    X(f2, ff_f,   BOTH, 0, 0, 0, 0,  f64,       f64,   rem) \
    X(f3, fff_f,  BOTH, 0, 1, 1, 0,  f64,       f64,   muladd) \
    X(a1, af_f,   HARD, 0, 1, 0, 0,  f64,       f64,   log2) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f64,       bool,  eq_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f64,       bool,  le_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f64,       bool,  lt_signaling) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f64,       bool,  eq_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f64,       bool,  le_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f64,       bool,  lt_quiet) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i32,   f80,   to_f80) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i64,   f80,   to_f80) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f80,       i32,   to_i32) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f80,       i32,   to_i32_round_to_zero) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f80,       i64,   to_i64) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f80,       i64,   to_i64_round_to_zero) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f80,       f32,   to_f32) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f80,       f64,   to_f64) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f32,       f80,   to_f80) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f64,       f80,   to_f80) \
    X(f1, f_f,    BOTH, 0, 1, 0, 0,  f80,       f80,   round_to_int) \
    X(a1, af_f,   BOTH, 1, 1, 0, 0,  f80,       f80,   sqrt) \
    X(f2, ff_f,   BOTH, 1, 1, 0, 1,  f80,       f80,   add) \
    X(f2, ff_f,   BOTH, 1, 1, 0, 1,  f80,       f80,   sub) \
    X(f2, ff_f,   BOTH, 1, 1, 1, 1,  f80,       f80,   mul) \
    X(f2, ff_f,   BOTH, 1, 1, 0, 1,  f80,       f80,   div) \
    X(f2, ff_f,   BOTH, 0, 0, 0, 0,  f80,       f80,   rem) \
    X(f3, fff_f,  BOTH, 0, 1, 1, 0,  f80,       f80,   muladd) \
    X(a1, af_f,   HARD, 0, 1, 0, 0,  f80,       f80,   log2) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f80,       bool,  eq_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f80,       bool,  le_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f80,       bool,  lt_signaling) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f80,       bool,  eq_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f80,       bool,  le_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f80,       bool,  lt_quiet) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i32,   f128,  to_f128) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i64,   f128,  to_f128) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f128,  i32,   to_i32) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f128,  i32,   to_i32_round_to_zero) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f128,  i64,   to_i64) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f128,  i64,   to_i64_round_to_zero) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f128,  f32,   to_f32) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f128,  f64,   to_f64) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f128,  f80,   to_f80) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f32,   f128,  to_f128) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f64,   f128,  to_f128) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f80,   f128,  to_f128) \
    X(f1, f_f,    BOTH, 0, 1, 0, 0,  f128,  f128,  round_to_int) \
    X(a1, af_f,   BOTH, 0, 1, 0, 0,  f128,  f128,  sqrt) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f128,  f128,  add) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f128,  f128,  sub) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f128,  f128,  mul) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f128,  f128,  div) \
    X(f2, ff_f,   BOTH, 0, 0, 0, 0,  f128,  f128,  rem) \
    X(f3, fff_f,  BOTH, 0, 1, 1, 0,  f128,  f128,  muladd) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f128,  bool,  eq_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f128,  bool,  le_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f128,  bool,  lt_signaling) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f128,  bool,  eq_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f128,  bool,  le_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f128,  bool,  lt_quiet) \

#define DECLARE_FUNCTIONS_TABLE(X) \
    DECLARE_HARDWARE_FUNCTIONS_TABLE(X) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i32,   f256,  to_f256) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i64,   f256,  to_f256) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f32,   f256,  to_f256) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f64,   f256,  to_f256) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f80,   f256,  to_f256) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f128,  f256,  to_f256) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f256,  i32,   to_i32) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f256,  i32,   to_i32_round_to_zero) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f256,  i64,   to_i64) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f256,  i64,   to_i64_round_to_zero) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f256,  f32,   to_f32) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f256,  f64,   to_f64) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f256,  f80,   to_f80) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f256,  f128,  to_f128) \
    X(f1, f_f,    BOTH, 0, 1, 0, 0,  f256,  f256,  round_to_int) \
    X(a1, af_f,   BOTH, 0, 1, 0, 0,  f256,  f256,  sqrt) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f256,  f256,  add) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f256,  f256,  sub) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f256,  f256,  mul) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f256,  f256,  div) \
    X(f2, ff_f,   BOTH, 0, 0, 0, 0,  f256,  f256,  rem) \
    X(f3, fff_f,  BOTH, 0, 1, 1, 0,  f256,  f256,  muladd) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f256,  bool,  eq_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f256,  bool,  le_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f256,  bool,  lt_signaling) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f256,  bool,  eq_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f256,  bool,  le_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f256,  bool,  lt_quiet) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i32,   f512,  to_f512) \
    X(f1, i_f,    BOTH, 0, 0, 0, 0,  i64,   f512,  to_f512) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f32,   f512,  to_f512) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f64,   f512,  to_f512) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f80,   f512,  to_f512) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f128,  f512,  to_f512) \
    X(f1, f_f,    BOTH, 0, 0, 0, 0,  f256,  f512,  to_f512) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f512,  i32,   to_i32) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f512,  i32,   to_i32_round_to_zero) \
    X(f1, f_i,    BOTH, 0, 1, 0, 0,  f512,  i64,   to_i64) \
    X(f1, f_i,    BOTH, 0, 0, 0, 0,  f512,  i64,   to_i64_round_to_zero) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f512,  f32,   to_f32) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f512,  f64,   to_f64) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f512,  f80,   to_f80) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f512,  f128,  to_f128) \
    X(f1, f_f,    BOTH, 0, 1, 1, 0,  f512,  f256,  to_f256) \
    X(f1, f_f,    BOTH, 0, 1, 0, 0,  f512,  f512,  round_to_int) \
    X(a1, af_f,   BOTH, 0, 1, 0, 0,  f512,  f512,  sqrt) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f512,  f512,  add) \
    X(f2, ff_f,   BOTH, 0, 1, 0, 0,  f512,  f512,  sub) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f512,  f512,  mul) \
    X(f2, ff_f,   BOTH, 0, 1, 1, 0,  f512,  f512,  div) \
    X(f2, ff_f,   NONE, 0, 0, 0, 0,  f512,  f512,  rem) \
    X(f3, fff_f,  BOTH, 0, 1, 1, 0,  f512,  f512,  muladd) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f512,  bool,  eq_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f512,  bool,  le_signaling) \
    X(f2, ff_b,   NONE, 0, 0, 0, 0,  f512,  bool,  lt_signaling) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f512,  bool,  eq_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f512,  bool,  le_quiet) \
    X(f2, ff_b,   BOTH, 0, 0, 0, 0,  f512,  bool,  lt_quiet) \



#define FPU_FUNCTION_ENUM(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) T1##_##method,

enum {
    DECLARE_FUNCTIONS_TABLE(FPU_FUNCTION_ENUM)
    NUM_FUNCTIONS
};

} // namespace fpu
} // namespace postrisc
