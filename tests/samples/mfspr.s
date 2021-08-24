.text
    alloc 61
    write "\ntest write: special register"
    write "ip      %s(ip)"
    write "eip     %s(eip)"
    write "eca     %s(eca)"
    write "fpcr    %s(fpcr)"
    write "rsc     %s(rsc)"
    write "rsp     %s(rsp)"
    write "bsp     %s(bsp)"
    write "peb     %s(peb)"
    write "teb     %s(teb)"
    write "itc     %s(itc)"
    write "itm     %s(itm)"
    write "psr     %s(psr)"
    write "pta     %s(pta)"
    write "iva     %s(iva)"
    write "kip     %s(kip)"
    write "ksp     %s(ksp)"
    write "krsp    %s(krsp)"
    write "iip     %s(iip)"
    write "iipa    %s(iipa)"
    write "ipsr    %s(ipsr)"
    write "cause   %s(cause)"
    write "ifa     %s(ifa)"
    write "iib     %s(iib)"
    write "tpr     %s(tpr)"
    write "lid     %s(lid)"
    write "irr0    %s(irr0)"
    write "irr1    %s(irr1)"
    write "irr2    %s(irr2)"
    write "irr3    %s(irr3)"
    write "isr0    %s(isr0)"
    write "isr1    %s(isr1)"
    write "isr2    %s(isr2)"
    write "isr3    %s(isr3)"
    write "tsv     %s(tsv)"
    write "cmcv    %s(cmcv)"
    write "pmv     %s(pmv)"

    write "\ntest read special register"

    get_spr %r12, %ip
    write "ip      %x64(r12)"

    get_spr %r12, %eip
    write "eip     %x64(r12)"

    get_spr %r12, %eca
    write "%x64(r12)"

    get_spr %r12, %fpcr
    write "%x64(r12)"

    get_spr %r12, %rsc
    write "%x64(r12)"

    get_spr %r12, %rsp
    write "%x64(r12)"

    get_spr %r12, %bsp
    write "%x64(r12)"

    get_spr %r12, %peb
    write "%x64(r12)"

    get_spr %r12, %teb
    write "%x64(r12)"

    get_spr %r12, %itc
    write "%x64(r12)"

    get_spr %r12, %itm
    write "%x64(r12)"

    get_spr %r12, %psr
    write "%x64(r12)"

    get_spr %r12, %pta
    write "%x64(r12)"

    get_spr %r12, %iva
    write "%x64(r12)"

    get_spr %r12, %kip
    write "%x64(r12)"

    get_spr %r12, %ksp
    write "%x64(r12)"

    get_spr %r12, %krsp
    write "krsp    %x64(r12)"

    get_spr %r12, %iip
    write "iip     %x64(r12)"

    get_spr %r12, %iipa
    write "iipa    %x64(r12)"

    get_spr %r12, %ipsr
    write "ipsr    %x64(r12)"

    get_spr %r12, %cause
    write "cause   %x64(r12)"

    write "%s(ifa)"
    get_spr %r12, %ifa
    write "ifa     %x64(r12)"

    get_spr %r12, %iib
    write "iib     %x128(r12)"

    get_spr %r12, %tpr
    write "tpr     %x64(r12)"

    get_spr %r12, %lid
    write "lid     %x64(r12)"

    get_spr %r12, %irr0
    write "irr0    %x64(r12)"

    get_spr %r12, %irr1
    write "irr1    %x64(r12)"

    get_spr %r12, %irr2
    write "irr2    %x64(r12)"

    get_spr %r12, %irr3
    write "irr3    %x64(r12)"

    get_spr %r12, %isr0
    write "%x64(r12)"

    get_spr %r12, %isr1
    write "%x64(r12)"

    get_spr %r12, %isr2
    write "%x64(r12)"

    get_spr %r12, %isr3
    write "%x64(r12)"

    get_spr %r12, %tsv
    write "%x64(r12)"

    get_spr %r12, %cmcv
    write "%x64(r12)"

    get_spr %r12, %pmv
    write "%x64(r12)"

    write "end test special register read/write"
.end
