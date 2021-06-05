#pragma once

#include "assembler.hpp"

namespace postrisc {

class CRandomProvider;

class CBinaryRandomGenerator : public CAssembler {
public:
    CBinaryRandomGenerator(const Decoder&);

    int AssembleGenerate(std::ostream& out, size_t codesize, size_t rodatasize, size_t datasize);

private:
    int CompileRandomCode(CRandomProvider& random, CProgramSection& section);
    i64 GetRandomBranchDisplacement(u64 ip, u64 random, EInstructionSize size1, EInstructionFullSize size2);
};

} // namespace postrisc
