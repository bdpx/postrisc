#pragma once

#include "assembler.hpp"

namespace postrisc {

class CRandomProvider;

class CBinaryRandomGenerator : public CAssembler {
public:
    CBinaryRandomGenerator(const Decoder&);

    int AssembleGenerate(std::ostream& out, size_t codesize, size_t rodatasize, size_t datasize);

private:
    int CompileRandomCode(CRandomProvider & random, CProgramSection & section);
    int64_t GetRandomBranchDisplacement(uint64_t ip, uint64_t random, EInstructionSize size1, EInstructionFullSize size2);
};

} // namespace postrisc
