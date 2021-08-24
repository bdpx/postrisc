.text
    alloc 61
    write "Example of test bit and branch"
    ld_imm %r19, 0x20
    ld_imm %r20, 12+3
    write "%i64(r20)"
    ld_imm %r10, 0
    br_bc_imm %r10, 10, xxx_n
    ld_imm.l %r20, 123456789012345
    ld_imm %r21, 321
    add_i64 %r23, %r20, %r21
    write "%i64(r43)"
xxx_n: write "%i64(r23)"

    ld_imm %r46, 0xabcdef
    br_bc_imm %r46, 56, branch_bit_exit
    br_bs_imm %r46, 56, branch_bit_exit
    ld_imm %r56, 56
    br_bc %r46, %r56, branch_bit_exit
    br_bs %r46, %r56, branch_bit_exit

branch_bit_exit:
    write	"end branch_bit test"
.end
