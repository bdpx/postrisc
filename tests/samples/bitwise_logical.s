.text
    alloc 46
    write "test bitwise logical"
    and %r23,  %r25, %r45
    and_imm    %r23, %r25, 12345
    and_imm.l  %r23, %r25, 1234567890
    andn %r23, %r25, %r45
    andn_imm   %r23, %r25, 12345
    or %r23,   %r25, %r45
    or_imm     %r23, %r25, 12345
    or_imm.l   %r23, %r25, 1234567890
    orn %r23,  %r25, %r45
    orn_imm    %r23, %r25, 12345
    xor %r23,  %r25, %r45
    xor_imm    %r23, %r25, 12345
    xor_imm.l  %r23, %r25, 1234567890
    nor        %r23, %r25, %r45
    nand       %r23, %r25, %r45
    xnor       %r23, %r25, %r45
.end
