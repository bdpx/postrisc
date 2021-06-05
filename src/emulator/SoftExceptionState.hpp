#pragma once

namespace postrisc {

class SoftExceptionState {
public:
    SoftExceptionState(void)
        : context(0), eip(0), reip(0) {}
    Result RaiseException(u64 address);
    Result CatchException(void);
    bool IsException(void) const;
    Result Push(void);
    Result Pop();
    void Clear(void);
    unsigned GetDepth(void) const;
    Result SetEBS(u64 new_val);
    u64 GetEIP(void) const;
    u64 GetREIP(void) const;
    void SetEIP(u64 new_eip);
    void SetREIP(u64 new_eip);
    void SetContext(u64 value);
    u64 GetContext(void) const;
    void reset(void);
    friend std::ostream& operator<<(std::ostream& out, const SoftExceptionState& r)
    {
        return out << "eip=" << fmt::hex(r.GetEIP())
                   << " context=" << fmt::hex(r.GetContext());
    }

private:
    u64 context;
    u64 eip;
    u64 reip;
};

ALWAYS_INLINE u64 SoftExceptionState::GetEIP(void) const
{
    assert((eip & 0xf) == 0);
    return eip;
}

ALWAYS_INLINE u64 SoftExceptionState::GetREIP(void) const
{
    assert((reip & 0xf) == 0);
    return reip;
}

ALWAYS_INLINE void SoftExceptionState::SetEIP(u64 new_eip)
{
    assert((new_eip & 0xf) == 0);
    eip = new_eip;
}

ALWAYS_INLINE void SoftExceptionState::SetREIP(u64 new_reip)
{
    assert((new_reip & 0xf) == 0);
    reip = new_reip;
}

ALWAYS_INLINE bool SoftExceptionState::IsException(void) const
{
    return (context != 0);
}

ALWAYS_INLINE Result SoftExceptionState::RaiseException(u64 address)
{
    context = address;
    return Result::continue_execution;
}

ALWAYS_INLINE void SoftExceptionState::SetContext(u64 value)
{
    context = value;
}

ALWAYS_INLINE u64 SoftExceptionState::GetContext(void) const
{
   return context;
}

ALWAYS_INLINE void SoftExceptionState::reset(void)
{
    eip = 0;
}

} // namespace postrisc
