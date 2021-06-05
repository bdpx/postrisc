#pragma once

#include "binary_format/file_header.hpp"

namespace postrisc {

class TestData_binop {
public:
    GeneralRegister s1;
    GeneralRegister s2;
    GeneralRegister result;
};

class TestData_binimm {
public:
    GeneralRegister s1;
    i64              imm;
    GeneralRegister result;
};

class TestData_RaAssignRbShamt {
public:
    GeneralRegister s1;
    unsigned shamt;
    GeneralRegister result;
};

class TestData_clmul {
public:
    GeneralRegister s1;
    GeneralRegister s2;
    char insn[16];
    GeneralRegister result;
};

class TestData_CompareAndBranch {
public:
    GeneralRegister  s1;
    GeneralRegister  s2;
    u32              isTaken; // 0 or 1
};

class TestData_CompareWithZeroAndBranch {
public:
    GeneralRegister  s1;
    u32              isTaken; // 0 or 1
};

class emulator_test_fixture {
public:
    Core *prepare(TargetSystem& targetSystem, const std::string& program);
    void run(AddressSpace& as, Core *core);

    void test_binop(const char *insn_name, const TestData_binop& data);
    void test_binimm(const char *insn_name, const TestData_binimm& data);
    void test_bin_shamt(const char *insn_name, const TestData_RaAssignRbShamt& data);
    void test_clmul(const char *insn_name, const TestData_clmul& data);
    void test_compare_and_branch(const char *insn_name, const TestData_CompareAndBranch& data);
    void TestCompareWithZeroAndBranch(const char *insn_name, const TestData_CompareWithZeroAndBranch& data);
    void test_alloc();

public:
    static const u64 vbase = U64C(0x400000);

public:
    ExeFileHeader  header;
};

} // namespace postrisc
