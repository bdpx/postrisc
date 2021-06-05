#pragma once

namespace postrisc {
namespace llvm {

template<typename T> static constexpr T MinSignedImm(int nbits)   { return -(static_cast<T>(1) << (nbits-1)); }
template<typename T> static constexpr T MaxSignedImm(int nbits)   { return  (static_cast<T>(1) << (nbits-1)) - 1; }
template<typename T> static constexpr T MaxUnsignedImm(int nbits) { return  (static_cast<T>(1) << (nbits)) - 1; }

enum ImmediateMode { SHORT, LONG };

template<typename T> struct imm_data {
    ImmediateMode mode;
    T value;
};

template<typename T> class immediates
{
public:
    static const std::vector<imm_data<T>> load_immediates;
    static const std::vector<imm_data<T>> binimm_immediates;
    static const std::vector<imm_data<T>> branch_immediates;
};

#if !defined(SPECIALIZE_IMMEDIATES)
extern template class immediates<i32>;
extern template class immediates<u32>;

extern template class immediates<i64>;
extern template class immediates<u64>;
#endif

} // namespace llvm
} // namespace postrisc
