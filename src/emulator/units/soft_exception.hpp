#pragma once

namespace postrisc {

class SoftExceptionState {
public:
    SoftExceptionState(void)
        : context(0), eip(0) {}
    CStatus RaiseException(uint64_t address);
    CStatus CatchException(void);
    bool IsException(void) const;
    CStatus Push(void);
    CStatus Pop();
    void Clear(void);
    unsigned GetDepth(void) const;
    CStatus SetEBS(uint64_t new_val);
    uint64_t GetEIP(void) const;
    void SetEIP(uint64_t new_eip);
    void SetContext(uint64_t value);
    uint64_t GetContext(void) const;
    void reset(void);
    friend std::ostream& operator<<(std::ostream& out, const SoftExceptionState& r);

private:
    uint64_t context;
    uint64_t eip;
};

ALWAYS_INLINE uint64_t SoftExceptionState::GetEIP(void) const
{
    return eip;
}

ALWAYS_INLINE void SoftExceptionState::SetEIP(uint64_t new_eip)
{
    eip = new_eip;
}

ALWAYS_INLINE bool SoftExceptionState::IsException(void) const
{
    return (context != 0);
}

ALWAYS_INLINE CStatus SoftExceptionState::RaiseException(uint64_t address)
{
    context = address;
    return CStatus(CStatus::continue_execution);
}

ALWAYS_INLINE void SoftExceptionState::SetContext(uint64_t value)
{
    context = value;
}

ALWAYS_INLINE uint64_t SoftExceptionState::GetContext(void) const
{
   return context;
}

ALWAYS_INLINE void SoftExceptionState::reset(void)
{
    eip = 0;
}

} // namespace postrisc
