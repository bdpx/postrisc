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

static const char *GetInstructionPrefix(Ordering ordering)
{
    switch (ordering) {
        case ATOMIC_RELAXED: return "amx";
        case ATOMIC_ACQUIRE: return "amq";
        case ATOMIC_CONSUME: return "amq";
        case ATOMIC_RELEASE: return "amr";
        case ATOMIC_ACQ_REL: return "amz";
        case ATOMIC_SEQ_CST: return "amx"; // +fence
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

        out << "// CHECK-LABEL: @" << name.str() << std::endl;
        out << "// CHECK: " << asm_name[test.insn] << std::endl;
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
    static const Ordering orderings[] = {
        ATOMIC_RELAXED,
        ATOMIC_ACQUIRE,
        ATOMIC_CONSUME,
        ATOMIC_SEQ_CST,
    };

    for (const Ordering ordering : orderings) {
        // signed
        for (const int size : {8, 16, 32, 64, 128}) {
            std::ostringstream type;
            type << "i" << size;

            std::ostringstream name;
            name << "atomic_load_" << type.str() << "_" << GetOrderingPrettyName(ordering);

            out << "// CHECK-LABEL: @" << name.str() << std::endl;
            if (isOrderingFenced(ordering)) {
                out << "// CHECK: " << asm_name[insn_fence_sc] << std::endl;
            }
            out << "// CHECK: " << GetInstructionPrefix(ordering) << "_ld_u" << size << " %r1, %r1" << std::endl;
            out << type.str() << " " << name.str() << "(" << type.str() << " *ptr) {" << std::endl;
            out << "    return __atomic_load_n(ptr, " << GetOrderingName(ordering) << ");" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }

        // unsigned
        for (const int size : {8, 16, 32, 64, 128}) {
            std::ostringstream type;
            type << "u" << size;

            std::ostringstream name;
            name << "atomic_load_" << type.str() << "_" << GetOrderingPrettyName(ordering);

            out << "// CHECK-LABEL: @" << name.str() << std::endl;
            if (isOrderingFenced(ordering)) {
                out << "// CHECK: " << asm_name[insn_fence_sc] << std::endl;
            }
            out << "// CHECK: " << GetInstructionPrefix(ordering) << "_ld_u" << size << " %r1, %r1" << std::endl;
            out << type.str() << " " << name.str() << "(" << type.str() << " *ptr) {" << std::endl;
            out << "    return __atomic_load_n(ptr, " << GetOrderingName(ordering) << ");" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }
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
    static const Ordering orderings[] = {
        ATOMIC_RELAXED,
        ATOMIC_RELEASE,
        ATOMIC_SEQ_CST,
    };

    for (const Ordering ordering : orderings) {
        for (const int size : {8, 16, 32, 64, 128}) {
            std::ostringstream type;
            type << "i" << size;

            std::ostringstream name;
            name << "atomic_store_" << type.str() << "_" << GetOrderingPrettyName(ordering);

            out << "// CHECK-LABEL: @" << name.str() << std::endl;
            if (isOrderingFenced(ordering)) {
                out << "// CHECK: " << asm_name[insn_fence_sc] << std::endl;
            }
            out << "// CHECK: " << GetInstructionPrefix(ordering) << "_st_u" << size << " %r1, %r2" << std::endl;
            out << "void " << name.str() << "(i" << size << " value, i" << size << " *ptr) {" << std::endl;
            out << "    return __atomic_store_n(ptr, value, " << GetOrderingName(ordering) << ");" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }
    }
}

/*
Built-in Function: type __atomic_exchange_n (type *ptr, type val, int memorder)
  This built-in function implements an atomic exchange operation.
  It writes val into *ptr, and returns the previous contents of *ptr.
  All memory order variants are valid.

Built-in Function: void __atomic_exchange (type *ptr, type *val, type *ret, int memorder)
  This is the generic version of an atomic exchange. It stores the contents of *val into *ptr.
  The original value of *ptr is copied into *ret.
*/
static void AtomicSwap(std::ostream& out, const std::string& func, const std::string& type, int size, Ordering ordering, bool is_signed)
{
    out << "// CHECK-LABEL: @" << func << std::endl;
    if (isOrderingFenced(ordering)) {
        out << "// CHECK: " << asm_name[insn_fence_sc] << std::endl;
    }
    out << "// CHECK: " << GetInstructionPrefix(ordering) << "_swap_u" << size << " %r1, %r1, %r2" << std::endl;
    out << type << " " << func << "(" << type << " *ptr, " << type << " value) {" << std::endl;
    out << "    return __atomic_exchange_n(ptr, value, " << GetOrderingName(ordering) << ");" << std::endl;
    out << "}" << std::endl;
    out << std::endl;
}

CODEGEN_HANDLER(AtomicSwaps)
{
    static const Ordering orderings[] = {
        ATOMIC_RELAXED,
        ATOMIC_ACQUIRE,
        ATOMIC_CONSUME,
        ATOMIC_RELEASE,
        ATOMIC_ACQ_REL,
        ATOMIC_SEQ_CST,
    };

    for (const Ordering ordering : orderings) {
        for (const int size : {8, 16, 32, 64, 128}) {
            std::ostringstream name;
            name << "atomic_exchange_i" << size << "_" << GetOrderingPrettyName(ordering);

            std::ostringstream type;
            type << "i" << size;

            AtomicSwap(out, name.str(), type.str(), size, ordering, true);
        }

        for (const int size : {8, 16, 32, 64, 128}) {
            std::ostringstream name;
            name << "atomic_exchange_u" << size << "_" << GetOrderingPrettyName(ordering);

            std::ostringstream type;
            type << "u" << size;

            AtomicSwap(out, name.str(), type.str(), size, ordering, false);
        }
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
    static const struct {
        Ordering success;
        Ordering failure;
    } tests[] = {
        { ATOMIC_RELAXED, ATOMIC_RELAXED },
        { ATOMIC_ACQUIRE, ATOMIC_RELAXED },
        { ATOMIC_ACQUIRE, ATOMIC_ACQUIRE },
        { ATOMIC_ACQUIRE, ATOMIC_CONSUME },
        { ATOMIC_CONSUME, ATOMIC_RELAXED },
        { ATOMIC_CONSUME, ATOMIC_ACQUIRE },
        { ATOMIC_CONSUME, ATOMIC_CONSUME },
        { ATOMIC_RELEASE, ATOMIC_RELAXED },
        { ATOMIC_ACQ_REL, ATOMIC_RELAXED },
        { ATOMIC_ACQ_REL, ATOMIC_CONSUME },
        { ATOMIC_ACQ_REL, ATOMIC_ACQUIRE },
        { ATOMIC_SEQ_CST, ATOMIC_RELAXED },
        { ATOMIC_SEQ_CST, ATOMIC_ACQUIRE },
        { ATOMIC_SEQ_CST, ATOMIC_CONSUME },
        { ATOMIC_SEQ_CST, ATOMIC_SEQ_CST },
    };

    for (const auto& test : tests) {
        for (const int size : {8, 16, 32, 64, 128}) {
            std::ostringstream type;
            type << "i" << size;

            std::ostringstream name;
            name << "atomic_cas_" << type.str() << "_" << GetOrderingPrettyName(test.success) << "_" << GetOrderingPrettyName(test.failure);

            out << "// CHECK-LABEL: @" << name.str() << std::endl;
            if (isOrderingFenced(test.success) || isOrderingFenced(test.failure)) {
                out << "// CHECK: " << asm_name[insn_fence_sc] << std::endl;
            }
            out << "// CHECK: " << GetInstructionPrefix(test.success) << "_cas_u" << size << " %r3, %r1, %r4, %r3" << std::endl;
            out << type.str() << " " << name.str()
                      << "("  << type.str() << " *ptr"
                      << ", " << type.str() << " *exchange"
                      << ", " << type.str() << " value) {" << std::endl;
            out << "    return __atomic_compare_exchange_n(ptr, exchange, value, false, "
                      << GetOrderingName(test.success) << ", " << GetOrderingName(test.failure) << ");" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }
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
    static const Ordering orderings[] = {
        ATOMIC_RELAXED,
        ATOMIC_ACQUIRE,
        ATOMIC_CONSUME,
        ATOMIC_RELEASE,
        ATOMIC_ACQ_REL,
        ATOMIC_SEQ_CST,
    };

    static const struct {
      char type;
      char op[32];
      char func[32];
    } ops[] = {
        { 'u', "ld_add_u", "__atomic_fetch_add" },
        { 'u', "ld_and_u", "__atomic_fetch_and" },
        { 'u', "ld_or_u",  "__atomic_fetch_or"  },
        { 'u', "ld_xor_u", "__atomic_fetch_xor" },
        { 'u', "ld_min_u", "__atomic_fetch_min" },
        { 'u', "ld_max_u", "__atomic_fetch_max" },
        { 'i', "ld_min_i", "__atomic_fetch_min" },
        { 'i', "ld_max_i", "__atomic_fetch_max" },
    };

    for (const auto& op : ops) {
        for (const Ordering ordering : orderings) {
            for (const int size : {8, 16, 32, 64, 128}) {
                std::ostringstream type;
                type << op.type << size;

                std::ostringstream name;
                name << "atomic_" << op.op << "" << size << "_" << GetOrderingPrettyName(ordering);

                out << "// CHECK-LABEL: @" << name.str() << std::endl;
                if (isOrderingFenced(ordering)) {
                    out << "// CHECK: " << asm_name[insn_fence_sc] << std::endl;
                    out << "// CHECK-NEXT: " << GetInstructionPrefix(ordering) << "_" << op.op << size << " %r1, %r1, %r3"<< std::endl;
                } else {
                    out << "// CHECK: " << GetInstructionPrefix(ordering) << "_" << op.op << size << " %r1, %r1, %r3"<< std::endl;
                }
                // CHECK-NEXT: amx_ld_add_u32 %r1, %r1, %r3
                out << type.str() << " " << name.str()
                          << "("  << type.str() << " *ptr"
                          << ", " << type.str() << " *exchange"
                          << ", " << type.str() << " value) {"<< std::endl;
                out << "    return " << op.func << "(ptr, value, " << GetOrderingName(ordering) << ");"<< std::endl;
                out << "}"<< std::endl;
                out << std::endl;
            }
        }
    }
}

} // namespace llvm
} // namespace postrisc
