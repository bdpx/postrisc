.text
    alloc 96
    write "test atomic fetch-op"
    addi %r5, %sp, -64
    write "atomic base: %x64(r5)"
    ldi  %r10, 5
    ldi  %r12, 10
    ldi  %r56, 5

    write "test amoadd"

    ldaddb.relaxed %r4, %r5, %r10
    ldaddb.acquire %r4, %r5, %r10
    ldaddb.release %r4, %r5, %r10
    ldaddb.acq_rel %r4, %r5, %r10

    ldaddh.relaxed %r4, %r5, %r10
    ldaddh.acquire %r4, %r5, %r10
    ldaddh.release %r4, %r5, %r10
    ldaddh.acq_rel %r4, %r5, %r10

    ldaddw.relaxed %r4, %r5, %r10
    ldaddw.acquire %r4, %r5, %r10
    ldaddw.release %r4, %r5, %r10
    ldaddw.acq_rel %r4, %r5, %r10

    ldaddd.relaxed %r4, %r5, %r10
    ldaddd.acquire %r4, %r5, %r10
    ldaddd.release %r4, %r5, %r10
    ldaddd.acq_rel %r4, %r5, %r10

    write "test amo-binary"

    ldandw.relaxed %r4, %r5, %r10
    ldandw.acquire %r4, %r5, %r10
    ldandw.release %r4, %r5, %r10
    ldandw.acq_rel %r4, %r5, %r10

    ldorw.release %r4, %r5, %r10
    ldorw.acq_rel %r4, %r5, %r10

    ldxorw.relaxed %r4, %r5, %r10
    ldxorw.relaxed %r4, %r5, %r10

    write "test amomin"
    ldsminw.acquire %r4, %r5, %r10
    ldsmind.acq_rel %r4, %r5, %r10

    ldsmaxb.relaxed %r4, %r5, %r10
    ldsmaxh.acquire %r4, %r5, %r10
    ldsmaxw.release %r4, %r5, %r10
    ldsmaxd.acq_rel %r4, %r5, %r10

    write "test amominu"

    lduminb.relaxed %r4, %r5, %r10
    ldumind.acquire %r4, %r5, %r10
    ldumaxd.release %r4, %r5, %r10
    ldumaxw.release %r4, %r5, %r10

    write "test cas"

    casb.relaxed %r12, %r5, %r56
    casb.acquire %r12, %r5, %r56
    casb.release %r12, %r5, %r56
    casb.acq_rel %r12, %r5, %r56

    cash.relaxed %r12, %r5, %r56
    cash.acquire %r12, %r5, %r56
    cash.release %r12, %r5, %r56
    cash.acq_rel %r12, %r5, %r56

    casw.relaxed %r12, %r5, %r56
    casw.acquire %r12, %r5, %r56
    casw.release %r12, %r5, %r56
    casw.acq_rel %r12, %r5, %r56

    casd.relaxed %r12, %r5, %r56
    casd.acquire %r12, %r5, %r56
    casd.release %r12, %r5, %r56
    casd.acq_rel %r12, %r5, %r56

    write "test load atomic relaxed"
    ldab.relaxed %r12, %r5
    ldah.relaxed %r12, %r5
    ldaw.relaxed %r12, %r5
    ldad.relaxed %r12, %r5
    ldaq.relaxed %r12, %r5

    write "test load atomic acquire"
    ldab.acquire %r12, %r5
    ldah.acquire %r12, %r5
    ldaw.acquire %r12, %r5
    ldad.acquire %r12, %r5
    ldaq.acquire %r12, %r5

    write "test store atomic relaxed"
    stab.relaxed %r12, %r5
    stah.relaxed %r12, %r5
    staw.relaxed %r12, %r5
    stad.relaxed %r12, %r5
    staq.relaxed %r12, %r5

    write "test store atomic release"
    stab.release %r12, %r5
    stah.release %r12, %r5
    staw.release %r12, %r5
    stad.release %r12, %r5
    staq.release %r12, %r5

.end
