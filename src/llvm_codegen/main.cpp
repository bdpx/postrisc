#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

/***********************************************************************
* llvm codegen tests generator
***********************************************************************/

int main(void)
{
    postrisc::util::setup();

    postrisc::llvm::Generator generator;
    return generator.Run();
}
