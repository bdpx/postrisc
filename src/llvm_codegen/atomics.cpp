#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

enum Ordering {
    ATOMIC_RELAXED,
    ATOMIC_ACQUIRE,
    ATOMIC_CONSUME,
    ATOMIC_RELEASE,
    ATOMIC_ACQ_REL,
    ATOMIC_SEQ_CST,
};

static const char *GetOrderingName(Ordering ordering)
{
    switch (ordering) {
        case ATOMIC_RELAXED: return "__ATOMIC_RELAXED";
        case ATOMIC_ACQUIRE: return "__ATOMIC_ACQUIRE";
        case ATOMIC_CONSUME: return "__ATOMIC_CONSUME";
        case ATOMIC_RELEASE: return "__ATOMIC_RELEASE";
        case ATOMIC_ACQ_REL: return "__ATOMIC_ACQ_REL";
        case ATOMIC_SEQ_CST: return "__ATOMIC_SEQ_CST";
    }
}

static const char *GetOrderingPrettyName(Ordering ordering)
{
    switch (ordering) {
        case ATOMIC_RELAXED: return "relaxed";
        case ATOMIC_ACQUIRE: return "acquire";
        case ATOMIC_CONSUME: return "consume";
        case ATOMIC_RELEASE: return "release";
        case ATOMIC_ACQ_REL: return "acq_rel";
        case ATOMIC_SEQ_CST: return "seq_cst";
    }
}

static bool isOrderingFenced(Ordering ordering)
{
    switch (ordering) {
        case ATOMIC_RELAXED:
        case ATOMIC_ACQUIRE:
        case ATOMIC_CONSUME:
        case ATOMIC_RELEASE:
        case ATOMIC_ACQ_REL:
            return false;
        case ATOMIC_SEQ_CST:
            return true;
    }
}

/*
__ATOMIC_RELAXED   Implies no inter-thread ordering constraints.
__ATOMIC_CONSUME   This is currently implemented using the stronger __ATOMIC_ACQUIRE memory order because of a deficiency in C++11 semantics for memory_order_consume.
__ATOMIC_ACQUIRE   Creates an inter-thread happens-before constraint from the release (or stronger) semantic store to this acquire load. Can prevent hoisting of code to before the operation.
__ATOMIC_RELEASE   Creates an inter-thread happens-before constraint to acquire (or stronger) semantic loads that read from this release store. Can prevent sinking of code to after the operation.
__ATOMIC_ACQ_REL   Combines the effects of both __ATOMIC_ACQUIRE and __ATOMIC_RELEASE.
__ATOMIC_SEQ_CST   Enforces total ordering with all other __ATOMIC_SEQ_CST operations.

Built-in Function: bool __atomic_test_and_set (void *ptr, int memorder)
  This built-in function performs an atomic test-and-set operation on the byte at *ptr.
  The byte is set to some implementation defined nonzero set value
  and the return value is true if and only if the previous contents were set.
  It should be only used for operands of type bool or char. For other types only part of the value may be set.
  All memory orders are valid.

Built-in Function: void __atomic_clear (bool *ptr, int memorder)
  This built-in function performs an atomic clear operation on *ptr.
  After the operation, *ptr contains 0.
  It should be only used for operands of type bool or char and in conjunction with __atomic_test_and_set.
  For other types it may only clear partially. If the type is not bool prefer using __atomic_store.

  The valid memory order variants are __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, and __ATOMIC_RELEASE.

Built-in Function: void __atomic_thread_fence (int memorder)
  This built-in function acts as a synchronization fence between threads based on the specified memory order.
  All memory orders are valid.

Built-in Function: void __atomic_signal_fence (int memorder)
  This built-in function acts as a synchronization fence between a thread and signal handlers based in the same thread.
  All memory orders are valid.

Built-in Function: bool __atomic_always_lock_free (size_t size, void *ptr)
  This built-in function returns true if objects of size bytes always generate
  lock-free atomic instructions for the target architecture.
  size must resolve to a compile-time constant and the result also resolves to a compile-time constant.

  ptr is an optional pointer to the object that may be used to determine alignment.
  A value of 0 indicates typical alignment should be used.
  The compiler may also ignore this parameter.

  if (__atomic_always_lock_free (sizeof (long long), 0))

Built-in Function: bool __atomic_is_lock_free (size_t size, void *ptr)
  This built-in function returns true if objects of size bytes always generate
  lock-free atomic instructions for the target architecture.
  If the built-in function is not known to be lock-free,
  a call is made to a runtime routine named __atomic_is_lock_free.

  ptr is an optional pointer to the object that may be used to determine alignment.
  A value of 0 indicates typical alignment should be used.
  The compiler may also ignore this parameter.
*/
CODEGEN_HANDLER(AtomicFences)
{
    static const struct {
        EInstructionID insn;
        Ordering ordering;
    } tests[] = {
        { insn_fence_a,  ATOMIC_ACQUIRE },
        { insn_fence_r,  ATOMIC_RELEASE },
        { insn_fence_ar, ATOMIC_ACQ_REL },
        { insn_fence_sc, ATOMIC_SEQ_CST },
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "atomic_fence_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << InsnCounter(this, insn_alloc) << " 1" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, test.insn) << std::endl;
        out << CHECK_NEXT << InsnCounter(this, insn_retf) << " 0" << std::endl;

        out << "void " << name.str() << "(void) {" << std::endl;
        out << "  __atomic_thread_fence(" << GetOrderingName(test.ordering) << ");" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

/*
Built-in Function: type __atomic_load_n (type *ptr, int memorder)
  This built-in function implements an atomic load operation. It returns the contents of *ptr.
  The valid memory order variants are __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, __ATOMIC_ACQUIRE, and __ATOMIC_CONSUME.

Built-in Function: void __atomic_load (type *ptr, type *ret, int memorder)
  This is the generic version of an atomic load. It returns the contents of *ptr in *ret.
*/
CODEGEN_HANDLER(AtomicLoads)
{
#define TEST_TABLE_ATOMIC_LOAD_X(PREFIX, SIZE, ORDER, SEXT) \
    { insn_ ## PREFIX ## _ld_u ## SIZE, ORDER, SEXT,       "i" #SIZE }, \
    { insn_ ## PREFIX ## _ld_u ## SIZE, ORDER, insn_undef, "u" #SIZE }, \

#define DEFINE_ATOMIC_LOAD_SIZES(PREFIX, ORDER) \
    TEST_TABLE_ATOMIC_LOAD_X(PREFIX, 8,   ORDER, insn_sext_i8) \
    TEST_TABLE_ATOMIC_LOAD_X(PREFIX, 16,  ORDER, insn_sext_i16) \
    TEST_TABLE_ATOMIC_LOAD_X(PREFIX, 32,  ORDER, insn_undef) \
    TEST_TABLE_ATOMIC_LOAD_X(PREFIX, 64,  ORDER, insn_undef) \
    TEST_TABLE_ATOMIC_LOAD_X(PREFIX, 128, ORDER, insn_undef) \

    static const struct {
        EInstructionID  atomic_insn;
        Ordering        ordering;
        EInstructionID  sext_insn;
        char            type[8];
    } tests[] = {
        DEFINE_ATOMIC_LOAD_SIZES(amx, ATOMIC_RELAXED)
        DEFINE_ATOMIC_LOAD_SIZES(amq, ATOMIC_ACQUIRE)
        DEFINE_ATOMIC_LOAD_SIZES(amq, ATOMIC_CONSUME)
        DEFINE_ATOMIC_LOAD_SIZES(amx, ATOMIC_SEQ_CST)
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "atomic_load_" << test.type << "_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        if (isOrderingFenced(test.ordering)) {
            out << CHECK << insn_name(insn_fence_sc) << std::endl;
        }
        out << CHECK << InsnCounter(this, test.atomic_insn) << " %r1, %r1" << std::endl;
        // atomic loads do zero-extend, add sext if needed
        if (test.sext_insn != insn_undef) {
            out << CHECK << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
        }
        out << test.type << " " << name.str() << "(" << test.type << " *ptr) {" << std::endl;
        out << "    return __atomic_load_n(ptr, " << GetOrderingName(test.ordering) << ");" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

/*
Built-in Function: void __atomic_store_n (type *ptr, type val, int memorder)
  This built-in function implements an atomic store operation. It writes val into *ptr.
  The valid memory order variants are __ATOMIC_RELAXED, __ATOMIC_SEQ_CST, and __ATOMIC_RELEASE.

Built-in Function: void __atomic_store (type *ptr, type *val, int memorder)
  This is the generic version of an atomic store. It stores the value of *val into *ptr.
*/
CODEGEN_HANDLER(AtomicStores)
{
#define TEST_TABLE_ATOMIC_STORE_X(PREFIX, SIZE, ORDER) \
    { insn_ ## PREFIX ## _st_u ## SIZE, ORDER, "i" #SIZE }, \
    { insn_ ## PREFIX ## _st_u ## SIZE, ORDER, "u" #SIZE }, \

#define DEFINE_ATOMIC_STORE_SIZES(PREFIX, ORDER) \
    TEST_TABLE_ATOMIC_STORE_X(PREFIX, 8,   ORDER) \
    TEST_TABLE_ATOMIC_STORE_X(PREFIX, 16,  ORDER) \
    TEST_TABLE_ATOMIC_STORE_X(PREFIX, 32,  ORDER) \
    TEST_TABLE_ATOMIC_STORE_X(PREFIX, 64,  ORDER) \
    TEST_TABLE_ATOMIC_STORE_X(PREFIX, 128, ORDER) \

    static const struct {
        EInstructionID  atomic_insn;
        Ordering        ordering;
        char            type[8];
    } tests[] = {
        DEFINE_ATOMIC_STORE_SIZES(amx, ATOMIC_RELAXED)
        DEFINE_ATOMIC_STORE_SIZES(amr, ATOMIC_RELEASE)
        DEFINE_ATOMIC_STORE_SIZES(amx, ATOMIC_SEQ_CST)
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "atomic_store_" << test.type << "_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << std::endl;

        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        if (isOrderingFenced(test.ordering)) {
            out << CHECK_NEXT << insn_name(insn_fence_sc) << std::endl;
        }
        out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r2" << std::endl;

        if (isOrderingFenced(test.ordering)) {
            out << CHECK_EMPTY << std::endl;
            out << CHECK_NEXT << "! BUNDLE" << std::endl;
        }
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << "void " << name.str() << "(" << test.type << " value, " << test.type << " *ptr) {" << std::endl;
        out << "    return __atomic_store_n(ptr, value, " << GetOrderingName(test.ordering) << ");" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

/*
Built-in Function: type __atomic_exchange_n (type *ptr, type val, int memorder)
  This built-in function implements an atomic exchange operation.
  It writes val into *ptr, and returns the previous contents of *ptr.
  All memory order variants are valid.
*/
CODEGEN_HANDLER(AtomicSwaps)
{
#define TEST_TABLE_SWAP_X(PREFIX, SIZE, ORDER, SEXT) \
    { insn_ ## PREFIX ## _swap_u ## SIZE, ORDER, SEXT,       "i" #SIZE }, \
    { insn_ ## PREFIX ## _swap_u ## SIZE, ORDER, insn_undef, "u" #SIZE }, \

#define DEFINE_SWAP_SIZES(PREFIX, ORDER) \
    TEST_TABLE_SWAP_X(PREFIX, 8,   ORDER, insn_sext_i8) \
    TEST_TABLE_SWAP_X(PREFIX, 16,  ORDER, insn_sext_i16) \
    TEST_TABLE_SWAP_X(PREFIX, 32,  ORDER, insn_undef) \
    TEST_TABLE_SWAP_X(PREFIX, 64,  ORDER, insn_undef) \
    TEST_TABLE_SWAP_X(PREFIX, 128, ORDER, insn_undef) \

    static const struct {
        EInstructionID  atomic_insn;
        Ordering        ordering;
        EInstructionID  sext_insn;
        char type[8];
    } tests[] = {
        DEFINE_SWAP_SIZES(amx, ATOMIC_RELAXED)
        DEFINE_SWAP_SIZES(amq, ATOMIC_ACQUIRE)
        DEFINE_SWAP_SIZES(amq, ATOMIC_CONSUME)
        DEFINE_SWAP_SIZES(amr, ATOMIC_RELEASE)
        DEFINE_SWAP_SIZES(amz, ATOMIC_ACQ_REL)
        DEFINE_SWAP_SIZES(amx, ATOMIC_SEQ_CST)
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "atomic_exchange_" << test.type << "_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        if (isOrderingFenced(test.ordering)) {
            out << CHECK_NEXT << insn_name(insn_fence_sc) << std::endl;
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2" << std::endl;
            // atomic loads do zero-extend, add sext if needed
            out << CHECK_EMPTY << std::endl;
            out << CHECK_NEXT << "! BUNDLE" << std::endl;
            if (test.sext_insn != insn_undef) {
                out << CHECK_NEXT << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
            }
            out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
        } else {
            // no fence
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2" << std::endl;
            // atomic loads do zero-extend, add sext if needed
            if (test.sext_insn != insn_undef) {
                out << CHECK_NEXT << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
            }
            out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
        }

        out << test.type << " " << name.str() << "(" << test.type << " *ptr, " << test.type << " value) {" << std::endl;
        out << "    return __atomic_exchange_n(ptr, value, " << GetOrderingName(test.ordering) << ");" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}


/*
Built-in Function:
bool __atomic_compare_exchange_n
  (type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)

  This built-in function implements an atomic compare and exchange operation.
  This compares the contents of *ptr with the contents of *expected.
  If equal, the operation is a read-modify-write operation that writes desired into *ptr.
  If they are not equal, the operation is a read and the current contents of *ptr
  are written into *expected.
  weak is true for weak compare_exchange, which may fail spuriously,
  and false for the strong variation, which never fails spuriously.
  Many targets only offer the strong variation and ignore the parameter.
  When in doubt, use the strong variation.

  If desired is written into *ptr then true is returned and memory is affected
  according to the memory order specified by success_memorder.
  There are no restrictions on what memory order can be used here.

  Otherwise, false is returned and memory is affected according to failure_memorder.
  This memory order cannot be __ATOMIC_RELEASE nor __ATOMIC_ACQ_REL.
  It also cannot be a stronger order than that specified by success_memorder.

Built-in Function:
bool __atomic_compare_exchange
  (type *ptr, type *expected, type *desired, bool weak, int success_memorder, int failure_memorder)

  This built-in function implements the generic version of __atomic_compare_exchange.
  The function is virtually identical to __atomic_compare_exchange_n,
  except the desired value is also a pointer.
*/

CODEGEN_HANDLER(AtomicCompareSwaps)
{
#define TEST_TABLE_CAS_X(PREFIX, SIZE, SUCCESS_ORDER, FAILURE_ORDER, SEXT) \
    { insn_ ## PREFIX ## _cas_u ## SIZE, SUCCESS_ORDER, FAILURE_ORDER, SEXT,       "i" #SIZE }, \
    { insn_ ## PREFIX ## _cas_u ## SIZE, SUCCESS_ORDER, FAILURE_ORDER, insn_undef, "u" #SIZE }, \

#define DEFINE_CAS_SIZES(PREFIX, SUCCESS_ORDER, FAILURE_ORDER) \
    TEST_TABLE_CAS_X(PREFIX, 8,   SUCCESS_ORDER, FAILURE_ORDER, insn_sext_i8) \
    TEST_TABLE_CAS_X(PREFIX, 16,  SUCCESS_ORDER, FAILURE_ORDER, insn_sext_i16) \
    TEST_TABLE_CAS_X(PREFIX, 32,  SUCCESS_ORDER, FAILURE_ORDER, insn_sext_i32) \
    TEST_TABLE_CAS_X(PREFIX, 64,  SUCCESS_ORDER, FAILURE_ORDER, insn_sext_i64) \
    TEST_TABLE_CAS_X(PREFIX, 128, SUCCESS_ORDER, FAILURE_ORDER, insn_undef) \

    static const struct {
        EInstructionID cas_insn;
        Ordering success;
        Ordering failure;
        EInstructionID sext_insn;
        char type[8];
    } tests[] = {
        DEFINE_CAS_SIZES(amx, ATOMIC_RELAXED, ATOMIC_RELAXED)
        DEFINE_CAS_SIZES(amq, ATOMIC_ACQUIRE, ATOMIC_RELAXED)
        DEFINE_CAS_SIZES(amq, ATOMIC_ACQUIRE, ATOMIC_ACQUIRE)
        DEFINE_CAS_SIZES(amq, ATOMIC_ACQUIRE, ATOMIC_CONSUME)
        DEFINE_CAS_SIZES(amq, ATOMIC_CONSUME, ATOMIC_RELAXED)
        DEFINE_CAS_SIZES(amq, ATOMIC_CONSUME, ATOMIC_ACQUIRE)
        DEFINE_CAS_SIZES(amq, ATOMIC_CONSUME, ATOMIC_CONSUME)
        DEFINE_CAS_SIZES(amr, ATOMIC_RELEASE, ATOMIC_RELAXED)
        DEFINE_CAS_SIZES(amz, ATOMIC_ACQ_REL, ATOMIC_RELAXED)
        DEFINE_CAS_SIZES(amz, ATOMIC_ACQ_REL, ATOMIC_CONSUME)
        DEFINE_CAS_SIZES(amz, ATOMIC_ACQ_REL, ATOMIC_ACQUIRE)
        DEFINE_CAS_SIZES(amx, ATOMIC_SEQ_CST, ATOMIC_RELAXED)
        DEFINE_CAS_SIZES(amx, ATOMIC_SEQ_CST, ATOMIC_ACQUIRE)
        DEFINE_CAS_SIZES(amx, ATOMIC_SEQ_CST, ATOMIC_CONSUME)
        DEFINE_CAS_SIZES(amx, ATOMIC_SEQ_CST, ATOMIC_SEQ_CST)
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "atomic_cas_" << test.type << "_" << GetOrderingPrettyName(test.success) << "_" << GetOrderingPrettyName(test.failure);

        out << CHECK_LABEL << name.str() << std::endl;

        out << CHECK << insn_name(insn_alloc) << " 5" << std::endl;
        if (isOrderingFenced(test.success) || isOrderingFenced(test.failure)) {
            out << CHECK << insn_name(insn_fence_sc) << std::endl;
        }
        out << CHECK << InsnCounter(this, test.cas_insn) << " %r3, %r1, %r4, %r3" << std::endl;
        out << CHECK << insn_name(insn_retf) << " 0" << std::endl;

        out << test.type << " " << name.str()
                  << "("  << test.type << " *ptr"
                  << ", " << test.type << " *exchange"
                  << ", " << test.type << " value) {" << std::endl;
        //if (test.sext_insn != insn_undef) {
        //    out << CHECK << insn_name(test.sext_insn) << " %r3, %r3" << std::endl;
        //}
        out << "    return __atomic_compare_exchange_n(ptr, exchange, value, true, "
                  << GetOrderingName(test.success) << ", " << GetOrderingName(test.failure) << ");" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

/*
Built-in Function: type __atomic_add_fetch (type *ptr, type val, int memorder)
Built-in Function: type __atomic_sub_fetch (type *ptr, type val, int memorder)
Built-in Function: type __atomic_and_fetch (type *ptr, type val, int memorder)
Built-in Function: type __atomic_xor_fetch (type *ptr, type val, int memorder)
Built-in Function: type __atomic_or_fetch (type *ptr, type val, int memorder)
Built-in Function: type __atomic_nand_fetch (type *ptr, type val, int memorder)
  These built-in functions perform the operation suggested by the name, and return the result of the operation.
  Operations on pointer arguments are performed as if the operands were of the uintptr_t type.
  That is, they are not scaled by the size of the type to which the pointer points.

  { *ptr op= val; return *ptr; }
  { *ptr = ~(*ptr & val); return *ptr; } // nand
  The object pointed to by the first argument must be of integer or pointer type.
  It must not be a boolean type. All memory orders are valid.

Built-in Function: type __atomic_fetch_add (type *ptr, type val, int memorder)
Built-in Function: type __atomic_fetch_sub (type *ptr, type val, int memorder)
Built-in Function: type __atomic_fetch_and (type *ptr, type val, int memorder)
Built-in Function: type __atomic_fetch_xor (type *ptr, type val, int memorder)
Built-in Function: type __atomic_fetch_or (type *ptr, type val, int memorder)
Built-in Function: type __atomic_fetch_nand (type *ptr, type val, int memorder)
  These built-in functions perform the operation suggested by the name,
  and return the value that had previously been in *ptr.
  Operations on pointer arguments are performed as if the operands were of the uintptr_t type.
  That is, they are not scaled by the size of the type to which the pointer points.

  { tmp = *ptr; *ptr op= val; return tmp; }
  { tmp = *ptr; *ptr = ~(*ptr & val); return tmp; } // nand
  The same constraints on arguments apply as for the corresponding __atomic_op_fetch built-in functions. All memory orders are valid.
*/
CODEGEN_HANDLER(AtomicLoadOps)
{
#define TEST_TABLE_LOAD_OP_X(PREFIX, OP, UOP, SIZE, ORDER, FUNC, SUB, SEXT) \
    { insn_ ## PREFIX ##  OP ## SIZE, ORDER, FUNC, SUB, SEXT,       "i" #SIZE }, \
    { insn_ ## PREFIX ## UOP ## SIZE, ORDER, FUNC, SUB, insn_undef, "u" #SIZE }, \

#define DEFINE_LOAD_OP_SIZES(PREFIX, OP, UOP, ORDER, FUNC, SUB) \
    TEST_TABLE_LOAD_OP_X(PREFIX, OP, UOP, 8,   ORDER, FUNC, insn_ ## SUB ## _i32,  insn_sext_i8) \
    TEST_TABLE_LOAD_OP_X(PREFIX, OP, UOP, 16,  ORDER, FUNC, insn_ ## SUB ## _i32,  insn_sext_i16) \
    TEST_TABLE_LOAD_OP_X(PREFIX, OP, UOP, 32,  ORDER, FUNC, insn_ ## SUB ## _i32,  insn_undef) \
    TEST_TABLE_LOAD_OP_X(PREFIX, OP, UOP, 64,  ORDER, FUNC, insn_ ## SUB ## _i64,  insn_undef) \
    TEST_TABLE_LOAD_OP_X(PREFIX, OP, UOP, 128, ORDER, FUNC, insn_ ## SUB ## _i128, insn_undef) \

#define DEFINE_LOAD_OP_ORDERS(OP, UOP, SUB, FUNC) \
        DEFINE_LOAD_OP_SIZES(amx, OP, UOP, ATOMIC_RELAXED, FUNC, SUB) \
        DEFINE_LOAD_OP_SIZES(amq, OP, UOP, ATOMIC_ACQUIRE, FUNC, SUB) \
        DEFINE_LOAD_OP_SIZES(amq, OP, UOP, ATOMIC_CONSUME, FUNC, SUB) \
        DEFINE_LOAD_OP_SIZES(amr, OP, UOP, ATOMIC_RELEASE, FUNC, SUB) \
        DEFINE_LOAD_OP_SIZES(amz, OP, UOP, ATOMIC_ACQ_REL, FUNC, SUB) \
        DEFINE_LOAD_OP_SIZES(amx, OP, UOP, ATOMIC_SEQ_CST, FUNC, SUB) \

    static const struct {
        EInstructionID  atomic_insn;
        Ordering        ordering;
        char            func[32];
        EInstructionID  sub_insn;
        EInstructionID  sext_insn;
        char            type[8];
    } tests[] = {
        DEFINE_LOAD_OP_ORDERS(_ld_add_u, _ld_add_u, add, "atomic_fetch_add")
        DEFINE_LOAD_OP_ORDERS(_ld_and_u, _ld_and_u, add, "atomic_fetch_and")
        DEFINE_LOAD_OP_ORDERS(_ld_or_u,  _ld_or_u,  add, "atomic_fetch_or")
        DEFINE_LOAD_OP_ORDERS(_ld_xor_u, _ld_xor_u, add, "atomic_fetch_xor")
        DEFINE_LOAD_OP_ORDERS(_ld_min_i, _ld_min_u, add, "atomic_fetch_min")
        DEFINE_LOAD_OP_ORDERS(_ld_max_i, _ld_max_u, add, "atomic_fetch_max")
        DEFINE_LOAD_OP_ORDERS(_ld_add_u, _ld_add_u, sub, "atomic_fetch_sub")
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "test_" << test.func << "_" << test.type << "_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;

        if (test.sub_insn == insn_sub_i32 || test.sub_insn == insn_sub_i64 || test.sub_insn == insn_sub_i128) {
            if (isOrderingFenced(test.ordering)) {
                out << CHECK_NEXT << insn_name(insn_fence_sc) << std::endl;
                out << CHECK_NEXT << insn_name(test.sub_insn) << " %r2, %gz, %r3"<< std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2"<< std::endl;
                // atomic loads do zero-extend, add sext if needed
                if (test.sext_insn != insn_undef) {
                    out << CHECK_NEXT << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                } else {
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                }
            } else {
                out << CHECK_NEXT << insn_name(test.sub_insn) << " %r2, %gz, %r3"<< std::endl;
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2"<< std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
                // atomic loads do zero-extend, add sext if needed
                if (test.sext_insn != insn_undef) {
                    out << CHECK_NEXT << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                } else {
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                }
            }
        } else {
            if (isOrderingFenced(test.ordering)) {
                out << CHECK_NEXT << insn_name(insn_fence_sc) << std::endl;
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r3"<< std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
                // atomic loads do zero-extend, add sext if needed
                if (test.sext_insn != insn_undef) {
                    out << CHECK_NEXT << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                } else {
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                }
            } else {
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r3"<< std::endl;
                // atomic loads do zero-extend, add sext if needed
                if (test.sext_insn != insn_undef) {
                    out << CHECK_NEXT << insn_name(test.sext_insn) << " %r1, %r1" << std::endl;
                    out << CHECK_EMPTY << std::endl;
                    out << CHECK_NEXT << "! BUNDLE" << std::endl;
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                } else {
                    out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
                }
            }
        }

        // CHECK-NEXT: amx_ld_add_u32 %r1, %r1, %r3
        out << test.type << " " << name.str()
                  << "("  << test.type << " *ptr"
                  << ", " << test.type << " *exchange"
                  << ", " << test.type << " value) {"<< std::endl;
        out << "    return __" << test.func << "(ptr, value, " << GetOrderingName(test.ordering) << ");"<< std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(AtomicStoreOps)
{
#define TEST_TABLE_STORE_OP_X(PREFIX, OP, UOP, SIZE, ORDER, FUNC, SUB) \
    { insn_ ## PREFIX ##  OP ## SIZE, ORDER, FUNC, SUB, "i" #SIZE }, \
    { insn_ ## PREFIX ## UOP ## SIZE, ORDER, FUNC, SUB, "u" #SIZE }, \

#define DEFINE_STORE_OP_SIZES(PREFIX, OP, UOP, ORDER, FUNC, SUB) \
    TEST_TABLE_STORE_OP_X(PREFIX, OP, UOP, 8,   ORDER, FUNC, insn_ ## SUB ## _i32) \
    TEST_TABLE_STORE_OP_X(PREFIX, OP, UOP, 16,  ORDER, FUNC, insn_ ## SUB ## _i32) \
    TEST_TABLE_STORE_OP_X(PREFIX, OP, UOP, 32,  ORDER, FUNC, insn_ ## SUB ## _i32) \
    TEST_TABLE_STORE_OP_X(PREFIX, OP, UOP, 64,  ORDER, FUNC, insn_ ## SUB ## _i64) \
    TEST_TABLE_STORE_OP_X(PREFIX, OP, UOP, 128, ORDER, FUNC, insn_ ## SUB ## _i128) \

#define DEFINE_STORE_OP_ORDERS(OP, UOP, SUB, FUNC) \
        DEFINE_STORE_OP_SIZES(amx, OP, UOP, ATOMIC_RELAXED, FUNC, SUB) \
        DEFINE_STORE_OP_SIZES(amr, OP, UOP, ATOMIC_RELEASE, FUNC, SUB) \
        DEFINE_STORE_OP_SIZES(amx, OP, UOP, ATOMIC_SEQ_CST, FUNC, SUB) \

    static const struct {
        EInstructionID  atomic_insn;
        Ordering        ordering;
        char            func[32];
        EInstructionID  sub_insn;
        char            type[8];
    } tests[] = {
        // FIXME: use real 'st_op' instead of 'ld_op'
        DEFINE_STORE_OP_ORDERS(_ld_add_u, _ld_add_u, add, "atomic_fetch_add")
        DEFINE_STORE_OP_ORDERS(_ld_and_u, _ld_and_u, add, "atomic_fetch_and")
        DEFINE_STORE_OP_ORDERS(_ld_or_u,  _ld_or_u,  add, "atomic_fetch_or")
        DEFINE_STORE_OP_ORDERS(_ld_xor_u, _ld_xor_u, add, "atomic_fetch_xor")
        DEFINE_STORE_OP_ORDERS(_ld_min_i, _ld_min_u, add, "atomic_fetch_min")
        DEFINE_STORE_OP_ORDERS(_ld_max_i, _ld_max_u, add, "atomic_fetch_max")
        DEFINE_STORE_OP_ORDERS(_ld_add_u, _ld_add_u, sub, "atomic_fetch_sub")
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "test_" << test.func << "_" << test.type << "_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;

        if (test.sub_insn == insn_sub_i32 || test.sub_insn == insn_sub_i64 || test.sub_insn == insn_sub_i128) {
            if (isOrderingFenced(test.ordering)) {
                out << CHECK_NEXT << insn_name(insn_fence_sc) << std::endl;
                out << CHECK_NEXT << insn_name(test.sub_insn) << " %r2, %gz, %r3"<< std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2"<< std::endl;
                // no ZEXT/SEXT
                out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
            } else {
                out << CHECK_NEXT << insn_name(test.sub_insn) << " %r2, %gz, %r3"<< std::endl;
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2"<< std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
                // no ZEXT/SEXT
                out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
            }
        } else {
            if (isOrderingFenced(test.ordering)) {
                out << CHECK_NEXT << insn_name(insn_fence_sc) << std::endl;
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r3"<< std::endl;
                out << CHECK_EMPTY << std::endl;
                out << CHECK_NEXT << "! BUNDLE" << std::endl;
                // no ZEXT/SEXT
                out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
            } else {
                out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r3"<< std::endl;
                // no ZEXT/SEXT
                out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
            }
        }

        // CHECK-NEXT: amx_ld_add_u32 %r1, %r1, %r3
        out << "void " << name.str()
                  << "("  << test.type << " *ptr"
                  << ", " << test.type << " *exchange"
                  << ", " << test.type << " value) {"<< std::endl;
        out << "    __" << test.func << "(ptr, value, " << GetOrderingName(test.ordering) << ");"<< std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

} // namespace llvm
} // namespace postrisc
