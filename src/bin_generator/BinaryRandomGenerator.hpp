#pragma once

#include "assembler/Assembler.hpp"

namespace postrisc {

class RandomProvider;

class BinaryRandomGenerator : public Assembler {
public:
    BinaryRandomGenerator(const Decoder&);

    int AssembleGenerate(std::ostream& out, size_t codesize, size_t rodatasize, size_t datasize);

private:
    int CompileRandomCode(RandomProvider& random, ProgramSection& section);
    i64 GetRandomBranchDisplacement(u64 ip, u64 random, EInstructionSize size1, EInstructionFullSize size2);
};

} // namespace postrisc
