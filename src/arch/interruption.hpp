#pragma once

// for forced union serialization
#include <boost/serialization/level.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>

#include "instruction_fields.hpp"

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
    X(continue_execution) \
    X(taken_branch) \
    X(spurious_interrupt) \
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
    X(illegal_instruction) \
    X(register_window_protection) \
    X(general_protection) \
    X(invalid_parameter) \
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
    X(data_access_bit) \
    X(data_debug) \
    X(halt) \
    X(floating_point_trap) \
    X(lower_privilege_transfer_trap) \
    X(taken_branch_trap) \
    X(single_step_trap) \
    X(program_trap)

class [[nodiscard]] CStatus {
public:
enum CEventEnum : uint8_t {
#define INTERRUPTION_VECTORS_ENUM_X(name) name,
    DECLARE_INTERRUPTION_VECTORS(INTERRUPTION_VECTORS_ENUM_X)
    last
};

    CStatus(void) {}
    explicit CStatus(CEventEnum code) : m_Code(code) {}
    explicit CStatus(unsigned code) : m_Code(static_cast<CEventEnum>(code)) {}

    bool continuable(void) const { return m_Code == continue_execution; }
    bool success(void) const { return m_Code <= taken_branch; }
    bool IsFault(void) const { return m_Code > taken_branch && m_Code < data_debug; }
    bool IsTrap(void) const { return m_Code >= data_debug; }
    bool halted(void) const { return m_Code == halt; }
    unsigned GetVector(void) const { return static_cast<unsigned>(m_Code); }
    const char * GetInterruptionName(void) const;

    bool operator == (const CStatus r) const { return m_Code == r.m_Code; }
    bool operator != (const CStatus r) const { return m_Code != r.m_Code; }
    bool operator == (const CStatus::CEventEnum r) const { return m_Code == r; }
    bool operator != (const CStatus::CEventEnum r) const { return m_Code != r; }

    friend std::ostream & operator << (std::ostream & out, CStatus r)
    {
        return out << static_cast<int>(r.m_Code)
                   << '(' << r.GetInterruptionName() << ')';
    }


private:
    CEventEnum m_Code;
};

#define CHECK_STATUS(status)                                               \
    do {                                                                   \
        CStatus status__ = (status);                                       \
        if (unlikely(!status__.continuable())) { return status__; }        \
    } while (0)


STATIC_ASSERT(static_cast<int>(CStatus::last) <= (1 << bits_per_vector_num));

/***************************************************************************
* cause
***************************************************************************/
union CInterruptionCauseRegister {
    uint64_t    imm;
    struct {
        uint64_t    x      : 1;    // execution
        uint64_t    w      : 1;    // write
        uint64_t    r      : 1;    // read
        uint64_t    a      : 1;    // access
        uint64_t    n      : 1;    // n
        uint64_t    d      : 1;    // dirty
        uint64_t    rs     : 1;    // register stack access
        uint64_t    ei     : bits_per_slot_num;    // excepting instruction slot
        uint64_t    code   : bits_per_branch;      // 27 bit code
        uint64_t    vector : bits_per_int_vector;   // 8 bit vector
    } u;

    friend std::ostream& operator << (std::ostream& out, const CInterruptionCauseRegister & cause);

    template<class Archive>
    void
    serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(imm);
    }
};

} // namespace postrisc

BOOST_IS_BITWISE_SERIALIZABLE(postrisc::CInterruptionCauseRegister)
BOOST_CLASS_IMPLEMENTATION(postrisc::CInterruptionCauseRegister, boost::serialization::object_serializable)
