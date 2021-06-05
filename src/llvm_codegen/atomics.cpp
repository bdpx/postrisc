#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

// optional sign extension depen on type
struct TypeInfo {
    ValueType       type;
    EInstructionID  sext_insn;
};

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
    assert(!"unknown OrderingName");
    return nullptr;
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
    assert(!"unknown OrderingPrettyName");
    return nullptr;
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
    return false;
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
        MemoryOrdering cmd_ordering;
        Ordering ordering;
    } tests[] = {
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE },
        { MemoryOrdering::RELEASE, ATOMIC_RELEASE },
        { MemoryOrdering::ACQ_REL, ATOMIC_ACQ_REL },
        { MemoryOrdering::SEQ_CST, ATOMIC_SEQ_CST },
    };

    for (const auto& test : tests) {
        std::ostringstream name;
        name << "atomic_fence_" << GetOrderingPrettyName(test.ordering);

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << InsnCounter(this, insn_alloc) << " 1\n";
        out << CHECK_NEXT << InsnCounter(this, insn_fence) << ' ' << MemoryOrdering2Str(test.cmd_ordering) << '\n';
        out << CHECK_NEXT << InsnCounter(this, insn_retf) << " 0\n";

        out << "void " << name.str() << "(void) {\n";
        out << "  __atomic_thread_fence(" << GetOrderingName(test.ordering) << ");\n";
        out << "}\n";
        out << '\n';
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
    static const struct {
        MemoryOrdering memory_ordering;
        Ordering ordering;
    } orderings[] = {
        { MemoryOrdering::RELAXED, ATOMIC_RELAXED },
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE },
        { MemoryOrdering::ACQUIRE, ATOMIC_CONSUME },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST },
    };

    static const struct {
        EInstructionID  atomic_insn;
        TypeInfo        infos[2];
    } tests[] = {
        // amold_ 8 & 16 requires sign extension for signed types up to i32
        { insn_amoldb, { { IB, insn_sextb }, { UB, insn_undef } } },
        { insn_amoldh, { { IH, insn_sexth }, { UH, insn_undef } } },
        { insn_amoldw, { { IW, insn_undef }, { UW, insn_undef } } },
        { insn_amoldd, { { ID, insn_undef }, { UD, insn_undef } } },
        { insn_amoldq, { { IQ, insn_undef }, { UQ, insn_undef } } },
    };

    for (const auto& test : tests) {
    for (const auto& order : orderings) {
    for (const auto& info : test.infos) {
        std::ostringstream name;
        name << "atomic_load_" << info.type << "_" << GetOrderingPrettyName(order.ordering);

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << insn_alloc << " 2\n";
        if (isOrderingFenced(order.ordering)) {
            out << CHECK << insn_fence << ' ' <<  MemoryOrdering2Str(MemoryOrdering::SEQ_CST) << '\n';
        }
        out << CHECK << InsnCounter(this, test.atomic_insn) << " %r1, %r1, " << MemoryOrdering2Str(order.memory_ordering) << "\n";
        // atomic loads do zero-extend, add sext if needed
        if (info.sext_insn != insn_undef) {
            out << CHECK << info.sext_insn << " %r1, %r1\n";
        }
        out << info.type << " " << name.str() << "(" << info.type << " *ptr) {\n";
        out << "    return __atomic_load_n(ptr, " << GetOrderingName(order.ordering) << ");\n";
        out << "}\n";
        out << '\n';
    }}}
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
    static const struct {
        MemoryOrdering memory_ordering;
        Ordering ordering;
    } orderings[] = {
        { MemoryOrdering::RELAXED, ATOMIC_RELAXED },
        { MemoryOrdering::RELEASE, ATOMIC_RELEASE },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST },
    };

    static const struct {
        EInstructionID  atomic_insn;
        ValueType       type;
    } tests[] = {
        { insn_amostb, IB },
        { insn_amostb, UB },
        { insn_amosth, IH },
        { insn_amosth, UH },
        { insn_amostw, IW },
        { insn_amostw, UW },
        { insn_amostd, ID },
        { insn_amostd, UD },
        { insn_amostq, IQ },
        { insn_amostq, UQ },
    };

    for (const auto& test : tests) {
    for (const auto& order : orderings) {
        std::ostringstream name;
        name << "atomic_store_" << test.type << "_" << GetOrderingPrettyName(order.ordering);

        out << CHECK_LABEL << name.str() << '\n';

        out << CHECK << insn_alloc << " 3\n";
        if (isOrderingFenced(order.ordering)) {
            out << CHECK_NEXT << insn_fence << ' ' <<  MemoryOrdering2Str(MemoryOrdering::SEQ_CST) << '\n';
        }
        out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r2, " <<  MemoryOrdering2Str(order.memory_ordering) << "\n";

        if (isOrderingFenced(order.ordering)) {
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
        }
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << name.str() << "(" << test.type << " value, " << test.type << " *ptr) {\n";
        out << "    return __atomic_store_n(ptr, value, " << GetOrderingName(order.ordering) << ");\n";
        out << "}\n";
        out << '\n';
    }}
}

/*
Built-in Function: type __atomic_exchange_n (type *ptr, type val, int memorder)
  This built-in function implements an atomic exchange operation.
  It writes val into *ptr, and returns the previous contents of *ptr.
  All memory order variants are valid.
*/
CODEGEN_HANDLER(AtomicSwaps)
{
    static const struct {
        MemoryOrdering memory_ordering;
        Ordering ordering;
    } orderings[] = {
        { MemoryOrdering::RELAXED, ATOMIC_RELAXED },
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE },
        { MemoryOrdering::ACQUIRE, ATOMIC_CONSUME },
        { MemoryOrdering::RELEASE, ATOMIC_RELEASE },
        { MemoryOrdering::ACQ_REL, ATOMIC_ACQ_REL },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST },
    };

    static const struct {
        EInstructionID  atomic_insn;
        TypeInfo        infos[2];
    } tests[] = {
        { insn_amoswapb,  { { IB, insn_sextb }, { UB, insn_undef } } },
        { insn_amoswaph,  { { IH, insn_sexth }, { UH, insn_undef } } },
        { insn_amoswapw,  { { IW, insn_undef }, { UW, insn_undef } } },
        { insn_amoswapd,  { { ID, insn_undef }, { UD, insn_undef } } },
        { insn_amoswapq,  { { IQ, insn_undef }, { UQ, insn_undef } } },
    };

    for (const auto& test : tests) {
    for (const auto& order : orderings) {
    for (const auto& info : test.infos) {
        std::ostringstream name;
        name << "atomic_exchange_" << info.type << "_" << GetOrderingPrettyName(order.ordering);

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << insn_alloc << " 3\n";
        if (isOrderingFenced(order.ordering)) {
            out << CHECK_NEXT << insn_fence << ' ' <<  MemoryOrdering2Str(MemoryOrdering::SEQ_CST) << '\n';
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2\n";
            // atomic loads do zero-extend, add sext if needed
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
            if (info.sext_insn != insn_undef) {
                out << CHECK_NEXT << info.sext_insn << " %r1, %r1\n";
            }
            out << CHECK_NEXT << insn_retf << " 0\n";
        } else {
            // no fence
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn) << " %r1, %r1, %r2, " << MemoryOrdering2Str(order.memory_ordering) << "\n";
            // atomic loads do zero-extend, add sext if needed
            if (info.sext_insn != insn_undef) {
                out << CHECK_NEXT << info.sext_insn << " %r1, %r1\n";
                out << CHECK_EMPTY << '\n';
                out << CHECK_NEXT << "! BUNDLE\n";
            }
            out << CHECK_NEXT << insn_retf << " 0\n";
        }

        out << info.type << " " << name.str() << "(" << info.type << " *ptr, " << info.type << " value) {\n";
        out << "    return __atomic_exchange_n(ptr, value, " << GetOrderingName(order.ordering) << ");\n";
        out << "}\n";
        out << '\n';
    }}}
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
    static const struct {
        MemoryOrdering memory_ordering;
        Ordering success;
        Ordering failure;
    } orderings[] = {
        { MemoryOrdering::RELAXED, ATOMIC_RELAXED, ATOMIC_RELAXED },
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE, ATOMIC_RELAXED },
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE, ATOMIC_ACQUIRE },
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE, ATOMIC_CONSUME },
        { MemoryOrdering::ACQUIRE, ATOMIC_CONSUME, ATOMIC_RELAXED },
        { MemoryOrdering::ACQUIRE, ATOMIC_CONSUME, ATOMIC_ACQUIRE },
        { MemoryOrdering::ACQUIRE, ATOMIC_CONSUME, ATOMIC_CONSUME },
        { MemoryOrdering::RELEASE, ATOMIC_RELEASE, ATOMIC_RELAXED },
        { MemoryOrdering::ACQ_REL, ATOMIC_ACQ_REL, ATOMIC_RELAXED },
        { MemoryOrdering::ACQ_REL, ATOMIC_ACQ_REL, ATOMIC_CONSUME },
        { MemoryOrdering::ACQ_REL, ATOMIC_ACQ_REL, ATOMIC_ACQUIRE },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST, ATOMIC_RELAXED },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST, ATOMIC_ACQUIRE },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST, ATOMIC_CONSUME },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST, ATOMIC_SEQ_CST },
    };

    static const struct {
        EInstructionID cas_insn;
        TypeInfo       infos[2];
    } tests[] = {
        { insn_amocasb,  { { IB, insn_sextb }, { UB, insn_undef }}},
        { insn_amocash,  { { IH, insn_sexth }, { UH, insn_undef }}},
        { insn_amocasw,  { { IW, insn_undef }, { UW, insn_undef }}},
        { insn_amocasd,  { { ID, insn_undef }, { UD, insn_undef }}},
        { insn_amocasq,  { { IQ, insn_undef }, { UQ, insn_undef }}},
    };

    for (const auto& test : tests) {
    for (const auto& order : orderings) {
    for (const auto& info : test.infos) {
        std::ostringstream name;
        name << "atomic_cas_" << info.type << "_" << GetOrderingPrettyName(order.success) << "_" << GetOrderingPrettyName(order.failure);

        out << CHECK_LABEL << name.str() << '\n';

        out << CHECK << insn_alloc << " 5\n";
        if (isOrderingFenced(order.success) || isOrderingFenced(order.failure)) {
            out << CHECK << insn_fence << ' ' <<  MemoryOrdering2Str(MemoryOrdering::SEQ_CST) << '\n';
        }
        out << CHECK << InsnCounter(this, test.cas_insn) << " %r3, %r1, %r4, %r3, " << MemoryOrdering2Str(order.memory_ordering) << "\n";
        out << CHECK << insn_retf << " 0\n";

        out << info.type << " " << name.str()
            << "("  << info.type << " *ptr"
              << ", " << info.type << " *exchange"
              << ", " << info.type << " value) {\n";
        //if (test.sext_insn != insn_undef) {
        //    out << CHECK << test.sext_insn << " %r3, %r3\n";
        //}
        out << "    return __atomic_compare_exchange_n(ptr, exchange, value, true, "
                      << GetOrderingName(order.success) << ", " << GetOrderingName(order.failure) << ");\n";
        out << "}\n";
        out << '\n';
    }}}
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

static const struct {
    EInstructionID  atomic_insn;
    EInstructionID  sext_insn;
    ValueType       type;
    char            func[32];
} LoadOpTests[] = {
    { insn_amoaddb, insn_sextb,  IB, "atomic_fetch_add" },
    { insn_amoaddb, insn_undef,  UB, "atomic_fetch_add" },
    { insn_amoaddh, insn_sexth,  IH, "atomic_fetch_add" },
    { insn_amoaddh, insn_undef,  UH, "atomic_fetch_add" },
    { insn_amoaddw, insn_undef,  IW, "atomic_fetch_add" },
    { insn_amoaddw, insn_undef,  UW, "atomic_fetch_add" },
    { insn_amoaddd, insn_undef,  ID, "atomic_fetch_add" },
    { insn_amoaddd, insn_undef,  UD, "atomic_fetch_add" },
    { insn_amoaddq, insn_undef,  IQ, "atomic_fetch_add" },
    { insn_amoaddq, insn_undef,  UQ, "atomic_fetch_add" },

    { insn_amosubb, insn_sextb,  IB, "atomic_fetch_sub" },
    { insn_amosubb, insn_undef,  UB, "atomic_fetch_sub" },
    { insn_amosubh, insn_sexth,  IH, "atomic_fetch_sub" },
    { insn_amosubh, insn_undef,  UH, "atomic_fetch_sub" },
    { insn_amosubw, insn_undef,  IW, "atomic_fetch_sub" },
    { insn_amosubw, insn_undef,  UW, "atomic_fetch_sub" },
    { insn_amosubd, insn_undef,  ID, "atomic_fetch_sub" },
    { insn_amosubd, insn_undef,  UD, "atomic_fetch_sub" },
    { insn_amosubq, insn_undef,  IQ, "atomic_fetch_sub" },
    { insn_amosubq, insn_undef,  UQ, "atomic_fetch_sub" },

    { insn_amoandb, insn_sextb,  IB, "atomic_fetch_and" },
    { insn_amoandb, insn_undef,  UB, "atomic_fetch_and" },
    { insn_amoandh, insn_sexth,  IH, "atomic_fetch_and" },
    { insn_amoandh, insn_undef,  UH, "atomic_fetch_and" },
    { insn_amoandw, insn_undef,  IW, "atomic_fetch_and" },
    { insn_amoandw, insn_undef,  UW, "atomic_fetch_and" },
    { insn_amoandd, insn_undef,  ID, "atomic_fetch_and" },
    { insn_amoandd, insn_undef,  UD, "atomic_fetch_and" },
    { insn_amoandq, insn_undef,  IQ, "atomic_fetch_and" },
    { insn_amoandq, insn_undef,  UQ, "atomic_fetch_and" },

    { insn_amoorb, insn_sextb,  IB, "atomic_fetch_or" },
    { insn_amoorb, insn_undef,  UB, "atomic_fetch_or" },
    { insn_amoorh, insn_sexth,  IH, "atomic_fetch_or" },
    { insn_amoorh, insn_undef,  UH, "atomic_fetch_or" },
    { insn_amoorw, insn_undef,  IW, "atomic_fetch_or" },
    { insn_amoorw, insn_undef,  UW, "atomic_fetch_or" },
    { insn_amoord, insn_undef,  ID, "atomic_fetch_or" },
    { insn_amoord, insn_undef,  UD, "atomic_fetch_or" },
    { insn_amoorq, insn_undef,  IQ, "atomic_fetch_or" },
    { insn_amoorq, insn_undef,  UQ, "atomic_fetch_or" },

    { insn_amoxorb, insn_sextb,  IB, "atomic_fetch_xor" },
    { insn_amoxorb, insn_undef,  UB, "atomic_fetch_xor" },
    { insn_amoxorh, insn_sexth,  IH, "atomic_fetch_xor" },
    { insn_amoxorh, insn_undef,  UH, "atomic_fetch_xor" },
    { insn_amoxorw, insn_undef,  IW, "atomic_fetch_xor" },
    { insn_amoxorw, insn_undef,  UW, "atomic_fetch_xor" },
    { insn_amoxord, insn_undef,  ID, "atomic_fetch_xor" },
    { insn_amoxord, insn_undef,  UD, "atomic_fetch_xor" },
    { insn_amoxorq, insn_undef,  IQ, "atomic_fetch_xor" },
    { insn_amoxorq, insn_undef,  UQ, "atomic_fetch_xor" },

    { insn_amominsb, insn_sextb,  IB, "atomic_fetch_min" },
    { insn_amominub, insn_undef,  UB, "atomic_fetch_min" },
    { insn_amominsh, insn_sexth,  IH, "atomic_fetch_min" },
    { insn_amominuh, insn_undef,  UH, "atomic_fetch_min" },
    { insn_amominsw, insn_undef,  IW, "atomic_fetch_min" },
    { insn_amominuw, insn_undef,  UW, "atomic_fetch_min" },
    { insn_amominsd, insn_undef,  ID, "atomic_fetch_min" },
    { insn_amominud, insn_undef,  UD, "atomic_fetch_min" },
    { insn_amominsq, insn_undef,  IQ, "atomic_fetch_min" },
    { insn_amominuq, insn_undef,  UQ, "atomic_fetch_min" },

    { insn_amomaxsb, insn_sextb,  IB, "atomic_fetch_max" },
    { insn_amomaxub, insn_undef,  UB, "atomic_fetch_max" },
    { insn_amomaxsh, insn_sexth,  IH, "atomic_fetch_max" },
    { insn_amomaxuh, insn_undef,  UH, "atomic_fetch_max" },
    { insn_amomaxsw, insn_undef,  IW, "atomic_fetch_max" },
    { insn_amomaxuw, insn_undef,  UW, "atomic_fetch_max" },
    { insn_amomaxsd, insn_undef,  ID, "atomic_fetch_max" },
    { insn_amomaxud, insn_undef,  UD, "atomic_fetch_max" },
    { insn_amomaxsq, insn_undef,  IQ, "atomic_fetch_max" },
    { insn_amomaxuq, insn_undef,  UQ, "atomic_fetch_max" },
};

CODEGEN_HANDLER(AtomicLoadOps)
{
    static const struct {
        MemoryOrdering memory_ordering;
        Ordering ordering;
    } orderings[] = {
        { MemoryOrdering::RELAXED, ATOMIC_RELAXED },
        { MemoryOrdering::ACQUIRE, ATOMIC_ACQUIRE },
        { MemoryOrdering::ACQUIRE, ATOMIC_CONSUME },
        { MemoryOrdering::RELEASE, ATOMIC_RELEASE },
        { MemoryOrdering::ACQ_REL, ATOMIC_ACQ_REL },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST },
    };

    for (const auto& test : LoadOpTests) {
    for (const auto& order : orderings) {
        std::ostringstream name;
        name << "test_" << test.func << "_" << test.type << "_" << GetOrderingPrettyName(order.ordering);

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << insn_alloc << " 4\n";

        if (isOrderingFenced(order.ordering)) {
            out << CHECK_NEXT << insn_fence << ' ' <<  MemoryOrdering2Str( MemoryOrdering::SEQ_CST) << '\n';
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn)
                << " %r1, %r1, %r3, " <<  MemoryOrdering2Str(order.memory_ordering) << "\n";
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
            // atomic loads do zero-extend, add sext if needed
            if (test.sext_insn != insn_undef) {
                out << CHECK_NEXT << test.sext_insn << " %r1, %r1\n";
                out << CHECK_NEXT << insn_retf << " 0\n";
            } else {
                out << CHECK_NEXT << insn_retf << " 0\n";
            }
        } else {
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn)
                << " %r1, %r1, %r3, " <<  MemoryOrdering2Str(order.memory_ordering) << "\n";
            // atomic loads do zero-extend, add sext if needed
            if (test.sext_insn != insn_undef) {
                out << CHECK_NEXT << test.sext_insn << " %r1, %r1\n";
                out << CHECK_EMPTY << '\n';
                out << CHECK_NEXT << "! BUNDLE\n";
                out << CHECK_NEXT << insn_retf << " 0\n";
            } else {
                out << CHECK_NEXT << insn_retf << " 0\n";
            }
        }

        // CHECK-NEXT: amx_add_u32 %r1, %r1, %r3
        out << test.type << " " << name.str()
              << "("  << test.type << " *ptr"
              << ", " << test.type << " *exchange"
              << ", " << test.type << " value) {\n";
        out << "    return __" << test.func << "(ptr, value, " << GetOrderingName(order.ordering) << ");\n";
        out << "}\n";
        out << '\n';
    }}
}

//===----------------------------------------------------------------------===//
// StoreOp doesn't use SEXTs for result (which is ignored)
// StoreOp doesn't use acquire
//===----------------------------------------------------------------------===//
CODEGEN_HANDLER(AtomicStoreOps)
{
    static const struct {
        MemoryOrdering memory_ordering;
        Ordering ordering;
    } orderings[] = {
        { MemoryOrdering::RELAXED, ATOMIC_RELAXED },
        { MemoryOrdering::RELEASE, ATOMIC_RELEASE },
        { MemoryOrdering::RELAXED, ATOMIC_SEQ_CST },
    };

    for (const auto& test : LoadOpTests) {
    for (const auto& order : orderings) {
        std::ostringstream name;
        name << "test_" << test.func << "_" << test.type << "_" << GetOrderingPrettyName(order.ordering);

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << insn_alloc << " 4\n";

        if (isOrderingFenced(order.ordering)) {
            out << CHECK_NEXT << insn_fence << ' ' <<  MemoryOrdering2Str(MemoryOrdering::SEQ_CST) << '\n';
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn)
                << " %r1, %r1, %r3, " << MemoryOrdering2Str(order.memory_ordering) << "\n";
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
            // no ZEXT/SEXT
            out << CHECK_NEXT << insn_retf << " 0\n";
        } else {
            out << CHECK_NEXT << InsnCounter(this, test.atomic_insn)
                << " %r1, %r1, %r3, " << MemoryOrdering2Str(order.memory_ordering) << "\n";
            // no ZEXT/SEXT
            out << CHECK_NEXT << insn_retf << " 0\n";
        }

        // CHECK-NEXT: amx_add_u32 %r1, %r1, %r3
        out << "void " << name.str()
              << "("  << test.type << " *ptr"
              << ", " << test.type << " *exchange"
              << ", " << test.type << " value) {\n";
        out << "    __" << test.func << "(ptr, value, " << GetOrderingName(order.ordering) << ");\n";
        out << "}\n\n";
    }}
}

} // namespace llvm
} // namespace postrisc
