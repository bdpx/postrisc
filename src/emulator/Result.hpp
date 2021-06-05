#pragma once

// for forced union serialization
#include <boost/serialization/level.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>

#include "arch/instruction_fields.hpp"

namespace postrisc {

// interruption vector table
enum {
    bits_per_vector_num         = 8,     // 256 interruption handlers in vector table
    bits_per_vector_handler     = 10,    // 1024 bytes (64 bundles or 192 slots) per vector table handler
    bits_per_iva_alignment      = bits_per_vector_num + bits_per_vector_handler,
// external interrupts
    bits_per_int_vector         = 8,     // 256 external interrupt vectors
    bits_per_processor_id       = 16,
    bits_per_delivery_mode      = 3,
};

/***************************************************************************
*  interruptions in priority order
***************************************************************************/

#define DECLARE_INTERRUPTION_VECTORS(X) \
    X(machine_reset) \
    X(machine_check) \
    X(nonmaskable_interrupt) \
    X(external_interrupt) \
    X(platform_management) \
    X(device_error) \
    X(bus_unimplemented_address) \
    X(instruction_unaligned_address) \
    X(instruction_invalid_address) \
    X(instruction_unimplemented_address) \
    X(instruction_unsupported_reference) \
    X(instruction_tlb_miss) \
    X(instruction_page_not_present) \
    X(instruction_access_right) \
    X(instruction_access_bit) \
    X(instruction_dirty_bit) \
    X(instruction_debug) \
    X(illegal_bundle_template) \
    X(illegal_instruction) \
    X(instruction_limit) \
    X(register_window_protection) \
    X(general_protection) \
    X(invalid_parameter) \
    X(invalid_bundle_slot) \
    X(event_divide_by_zero) \
    X(integer_overflow) \
    X(software_double_exception) \
    X(floating_point_fault) \
    X(backstore_unaligned_address) \
    X(backstore_invalid_address) \
    X(backstore_unimplemented_address) \
    X(backstore_unsupported_reference) \
    X(backstore_tlb_miss) \
    X(backstore_page_not_present) \
    X(backstore_access_right) \
    X(backstore_dirty_bit) \
    X(backstore_access_bit) \
    X(backstore_debug) \
    X(data_unaligned_address) \
    X(data_invalid_address) \
    X(data_unimplemented_address) \
    X(data_unsupported_reference) \
    X(data_tlb_miss) \
    X(data_page_not_present) \
    X(data_access_right) \
    X(data_dirty_bit) \
    X(data_access_bit) /* last error */ \
    X(data_debug) /* first trap */ \
    X(halt) \
    X(floating_point_trap) \
    X(lower_privilege_transfer_trap) \
    X(taken_branch_trap) \
    X(single_step_trap) \
    X(program_trap)

class [[nodiscard]] Result {
public:
enum CEventEnum : int {
    taken_branch = -1,
    continue_execution = 0,
#define INTERRUPTION_VECTORS_ENUM_X(name) name,
    DECLARE_INTERRUPTION_VECTORS(INTERRUPTION_VECTORS_ENUM_X)
    last
};

    Result(void) {}
    // cppcheck-suppress-begin noExplicitConstructor
    constexpr Result(CEventEnum code) : m_Code(code) {}
    // cppcheck-suppress-end noExplicitConstructor

    // Allow conversion to enum
    constexpr operator CEventEnum() const { return m_Code; }
    // Prevent usage as: if(result)
    explicit operator bool() const = delete;

    static Result ConvertFromVector(unsigned vector) { return Result(static_cast<CEventEnum>(vector)); }
    bool continuable(void) const { return m_Code == continue_execution; }
    bool is_taken_branch(void) const { return m_Code == taken_branch; }
    bool success(void) const { return m_Code <= continue_execution; }
    bool IsFault(void) const { return m_Code > continue_execution && m_Code < data_debug; }
    bool IsTrap(void) const { return m_Code >= data_debug; }
    bool halted(void) const { return m_Code == halt; }
    unsigned GetVector(void) const { return static_cast<unsigned>(m_Code); }
    char const *GetInterruptionName(void) const;

    bool operator == (const Result r) const { return m_Code == r.m_Code; }
    bool operator != (const Result r) const { return m_Code != r.m_Code; }
    bool operator == (const Result::CEventEnum r) const { return m_Code == r; }
    bool operator != (const Result::CEventEnum r) const { return m_Code != r; }

    friend std::ostream& operator<<(std::ostream& out, Result r) {
        return out << r.m_Code << '(' << r.GetInterruptionName() << ')';
    }

private:
    CEventEnum m_Code;
};

#define CHECK_RESULT(STMT)                                               \
    do {                                                                 \
        const Result result__ = (STMT);                                  \
        if (unlikely(!result__.continuable())) { return result__; }      \
    } while (0)


static_assert(static_cast<int>(Result::last) <= (1 << bits_per_vector_num));

/***************************************************************************
* cause
***************************************************************************/
union CInterruptionCauseRegister {
    u64    imm;
    struct {
        u64    x      : 1;    // execution
        u64    w      : 1;    // write
        u64    r      : 1;    // read
        u64    a      : 1;    // access
        u64    n      : 1;    // n
        u64    d      : 1;    // dirty
        u64    rs     : 1;    // register stack access
        u64    ei     : bits_per_slot_num;     // excepting instruction slot
        u64    code   : bits_per_call;         // 28 bit code
        u64    vector : bits_per_int_vector;   // 8 bit vector
    } u;

    friend std::ostream& operator<<(std::ostream& out, const CInterruptionCauseRegister & cause);

    template<typename Archive> void serialize(Archive& ar, const unsigned int UNUSED(version)) {
        ar & BOOST_SERIALIZATION_NVP(imm);
    }
};

} // namespace postrisc

BOOST_IS_BITWISE_SERIALIZABLE(postrisc::CInterruptionCauseRegister)
BOOST_CLASS_IMPLEMENTATION(postrisc::CInterruptionCauseRegister, boost::serialization::object_serializable)
