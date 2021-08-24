.text
    alloc 61
    lda_xi64 %r41, %r40, %r12, 4, 52
    lda_xi64 %r41, %r40, %r12, 3, -12
    lda_xi64 %r41, %r40, %r12, 4, 52
    ld_imm.l %r5, -1
    mov2 %r3, %r4, %r4, %r3
    mov2 %r3, %r4, %r4, %r3


.rodata	; open text (read-only data) section
    align 16
text_lbl: ; this is label
    d1 111 ; signed byte
    d1 112
    d1 113
ddd:
    align 4 ; force 4-byte alignment for next data
    d1 6
    d1 7
    d1 8+0x3D ; you may use formulas!!!

.text
    write "test addressing"

; Examples of IP-relative references.
    ld_imm %r45, text_lo(text_lbl)
    write "text_lo(text_lbl)=%i64(r45)"
    ld_imm %r45, text_hi(text_lbl)
    write "text_hi(text_lbl)=%i64(r45)"
    ld_imm %r45, text_lbl
    write "%i64(r45)"

; Example of access to text section.
; First get IP-relative reference to text section (+/- 64 MB from IP).
    lda_r %r45, text_lbl

; Now in r45 we have base address.
; But it IS NOT true address of 'text_lbl'.
; We have in r45 nearest (to 'text_lbl') least address, aligned on 16-bytes boundary.
; Remember add 'text_lo' part of label address at each displacement calculation.
    ld_u8 %r50, %r45, text_lo(text_lbl)+0
    ld_u8 %r51, %r45, text_lo(text_lbl)+1
    ld_u8 %r52, %r45, text_lo(text_lbl)+2
    write "%i64(r50)" ; must be 111
    write "%i64(r51)" ; must be 112
    write "%i64(r52)" ; must be 113

; Example of incorrect access to text section (without bundle alignment)
    ld_u8 %r50, %r45, 0
    write "%i64(r50)" ; must be 101 - start of 16-byte portion
.end
