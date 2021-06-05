
/**************************************************************************************
* assembler instruction mnemonics
*
* macrotable must be sorted alphabetically by mnemonics!
*
* X (mnemonic, hw_subset, hw_format, sw_format, iic, hw_template, description)
*
* 1  mnemonic  assembler mnemonic (11 characters maximum)
* 2  hw_subset  instruction subset:
*         common base (base),
*         floating-point extended/quadruple (fpuq),
*         floating-point double scalar/packed (fpud),
*         floating-point single scalar/packed (fpus),
*         dsp-like integer packed (mmx),
*         privileged (kernel),
*         special (system),
*         atomic (atomic)
*         cipher (aes+clmul)
*
* 3  hw_format      instruction slot format
* 4  sw_format      assembler syntax format
* 5  iic            instruction itinerary class (for llvm)
* 6  hw_template    real mnemonic (different for pseudos)
* 7  description    full instruction name
*
**************************************************************************************/

X( absd,        base,    Rabc,   Rabc,     iic_default,       absd,       "absolute difference" )
X( absd4,       base,    Rabc,   Rabc,     iic_default,       absd4,      "absolute difference 4-byte" )
X( add,         base,    Rabc,   Rabc,     iic_default,       add,        "add" )
X( add4,        base,    Rabc,   Rabc,     iic_default,       add4,       "add 4-byte" )
X( addadd,      base,    Rabcd,  Rabcd,    iic_default,       addadd,     "add 3 values" )
X( addaddc,     base,    Rabcd,  Rabcd,    iic_default,       addaddc,    "add & add carry-out" )
X( addc,        base,    Rabc,   Rabc,     iic_default,       addc,       "add carry-out" )
X( addi,        base,    RabI21, RabI21,   iic_default,       addi,       "add immediate" )
X( addi4,       base,    RabI21, RabI21,   iic_default,       addi4,      "add immediate 4-byte" )
X( addo,        base,    Rabc,   Rabc,     iic_default,       addo,       "add overflow" )
X( addsub,      base,    Rabcd,  Rabcd,    iic_default,       addsub,     "add-subtract" )

X( aesdec,      cipher,  Rabc,   Rabc,     iic_default,       aesdec,     "aes decrypt round" )
X( aesdeclast,  cipher,  Rabc,   Rabc,     iic_default,       aesdeclast, "aes decrypt last round" )
X( aesenc,      cipher,  Rabc,   Rabc,     iic_default,       aesenc,     "aes encrypt round" )
X( aesenclast,  cipher,  Rabc,   Rabc,     iic_default,       aesenclast, "aes encrypt last round" )
X( aesimc,      cipher,  Rab,    Rab,      iic_default,       aesimc,     "aes inverse mix columns" )
X( aeskeygen,   cipher,  gmemu,  RabU14,   iic_default,       aeskeygen,  "aes key generation assist" )

X( alloc,       base,    ldar,   brzr,     iic_default,       alloc,      "allocate register frame and setup eip" )

//atomic modifications
X( amoadd4,     atomic,  amo,    amo,      iic_default,       amoadd4,     "atomic modify add 4-byte" )
X( amoadd8,     atomic,  amo,    amo,      iic_default,       amoadd8,     "atomic modify add 8-byte" )
X( amoand4,     atomic,  amo,    amo,      iic_default,       amoand4,     "atomic modify and 4-byte" )
X( amoand8,     atomic,  amo,    amo,      iic_default,       amoand8,     "atomic modify and 8-byte" )

X( amomax4,     atomic,  amo,    amo,      iic_default,       amomax4,     "atomic modify maximum 4-byte" )
X( amomax8,     atomic,  amo,    amo,      iic_default,       amomax8,     "atomic modify maximum 8-byte" )
X( amomaxu4,    atomic,  amo,    amo,      iic_default,       amomaxu4,    "atomic modify maximum unsigned 4-byte" )
X( amomaxu8,    atomic,  amo,    amo,      iic_default,       amomaxu8,    "atomic modify maximum unsigned 8-byte" )
X( amomin4,     atomic,  amo,    amo,      iic_default,       amomin4,     "atomic modify minimum 4-byte" )
X( amomin8,     atomic,  amo,    amo,      iic_default,       amomin8,     "atomic modify minimum 8-byte" )
X( amominu4,    atomic,  amo,    amo,      iic_default,       amominu4,    "atomic modify minimum unsigned 4-byte" )
X( amominu8,    atomic,  amo,    amo,      iic_default,       amominu8,    "atomic modify minimum unsigned 8-byte" )

X( amoor4,      atomic,  amo,    amo,      iic_default,       amoor4,      "atomic modify or 4-byte" )
X( amoor8,      atomic,  amo,    amo,      iic_default,       amoor8,      "atomic modify or 8-byte" )
X( amoswap4,    atomic,  amo,    amo,      iic_default,       amoswap4,    "atomic modify swap 4-byte" )
X( amoswap8,    atomic,  amo,    amo,      iic_default,       amoswap8,    "atomic modify swap 8-byte" )
X( amoxor4,     atomic,  amo,    amo,      iic_default,       amoxor4,     "atomic modify xor 4-byte" )
X( amoxor8,     atomic,  amo,    amo,      iic_default,       amoxor8,     "atomic modify xor 8-byte" )

X( and,         base,    Rabc,   Rabc,     iic_default,       and,        "and bitwise" )
X( anddec,      base,    Rabc,   Rabc,     iic_default,       anddec,     "and bitwise decremented" )
X( andi,        base,    RabI21, RabI21,   iic_default,       andi,       "and bitwise with immediate" )
X( andn,        base,    Rabc,   Rabc,     iic_default,       andn,       "and not bitwise" )
X( andneg,      base,    Rabc,   Rabc,     iic_default,       andneg,     "and bitwise negate" )
X( andni,       base,    RabI21, RabI21,   iic_default,       andni,      "and not bitwise with immediate" )

// branch if condition
X( bbc,         base,    RabB21,   RabB21,    iic_default,       bbc,        "branch if bit clear" )
X( bbci,        base,    RaSbB21,  RaSbB21,   iic_default,       bbci,       "branch if bit clear immediate" )

X( bbs,         base,    RabB21,   RabB21,    iic_default,       bbs,        "branch if bit set" )
X( bbsi,        base,    RaSbB21,  RaSbB21,   iic_default,       bbsi,       "branch if bit set immediate" )

X( beq,         base,    RabB21,   RabB21,    iic_default,       beq,        "branch if equal" )
X( beqi,        base,    RaI14B14, RaI14B14,  iic_default,       beqi,       "branch if equal immediate" )
X( beqz,        base,    ldar,     brzr,      iic_default,       beqz,       "branch if equal zero" )

X( bge,         base,    RabB21,   RbaB21,    iic_default,       ble,        "branch if greater than or equal" )
X( bgeu,        base,    RabB21,   RbaB21,    iic_default,       bleu,       "branch if greater than or equal unsigned" )
X( bgez,        base,    ldar,     brzr,      iic_default,       bgez,       "branch if greater than zero or equal" )

X( bgt,         base,    RabB21,   RbaB21,    iic_default,       blt,        "branch if greater than" )
X( bgti,        base,    RaI14B14, RaI14B14,  iic_default,       bgti,       "branch if greater than immediate" )
X( bgtu,        base,    RabB21,   RbaB21,    iic_default,       bltu,       "branch if greater than unsigned" )
X( bgtui,       base,    RaU14B14, RaU14B14,  iic_default,       bgtui,      "branch if greater than unsigned immediate" )
X( bgtz,        base,    ldar,     brzr,      iic_default,       bgtz,       "branch if greater than zero" )

X( ble,         base,    RabB21,   RabB21,    iic_default,       ble,        "branch if less than or equal" )
X( bleu,        base,    RabB21,   RabB21,    iic_default,       bleu,       "branch if less than or equal unsigned" )
X( blez,        base,    ldar,     brzr,      iic_default,       blez,       "branch if less than zero or equal" )

X( blt,         base,    RabB21,   RabB21,    iic_default,       blt,        "branch if less than" )
X( blti,        base,    RaI14B14, RaI14B14,  iic_default,       blti,       "branch if less than immediate" )
X( bltu,        base,    RabB21,   RabB21,    iic_default,       bltu,       "branch if less than unsigned" )
X( bltui,       base,    RaU14B14, RaU14B14,  iic_default,       bltui,      "branch if less than unsigned immediate" )
X( bltz,        base,    ldar,     brzr,      iic_default,       bltz,       "branch if less than zero" )

X( bne,         base,    RabB21,   RabB21,    iic_default,       bne,        "branch if not equal" )
X( bnei,        base,    RaI14B14, RaI14B14,  iic_default,       bnei,       "branch if not equal immediate" )
X( bnez,        base,    ldar,     brzr,      iic_default,       bnez,       "branch if not equal zero" )

X( br,          base,    br,       br,        iic_default,       br,         "branch relative" )
X( bri,         base,    Rbc,      Rbc,       iic_default,       bri,        "branch register indirect" )

//call
X( callmi,      base,    gmemu,  RabU14,    iic_default,       callmi,     "call memory indirect" )
X( callplt,     base,    RaU28,  gmemr,     iic_default,       callplt,    "call procedure linkage table" )
X( callr,       base,    ldar,   callr,     iic_default,       callr,      "call relative" )
X( callri,      base,    Rabc,   Rabc,      iic_default,       callri,     "call register indirect" )

// CAS
X( cas16,       atomic,  amo,    amo,      iic_default,       cas16,      "compare and swap 16-byte" )
X( cas4,        atomic,  amo,    amo,      iic_default,       cas4,       "compare and swap 4-byte" )
X( cas8,        atomic,  amo,    amo,      iic_default,       cas8,       "compare and swap 8-byte" )

X( clmul,       cipher,  gmemux, clmul,    iic_default,       clmul,      "carry-less multiply" )

//compare
X( cmpeq,       base,    Rabc,    Rabc,     iic_default,       cmpeq,      "compare equal" )
X( cmpeqi,      base,    RabI21,  RabI21,   iic_default,       cmpeqi,     "compare equal immediate" )
X( cmpge,       base,    Rabc,    Racb,     iic_default,       cmple,      "compare greater than or equal" )
X( cmpgeu,      base,    Rabc,    Racb,     iic_default,       cmpleu,     "compare greater than or equal unsigned" )
X( cmpgt,       base,    Rabc,    Racb,     iic_default,       cmplt,      "compare greater than" )
X( cmpgti,      base,    RabI21,  RabI21,   iic_default,       cmpgti,     "compare greater than immediate" )
X( cmpgtu,      base,    Rabc,    Racb,     iic_default,       cmpltu,     "compare greater than unsigned" )
X( cmpgtui,     base,    RabU21,  RabU21,   iic_default,       cmpgtui,    "compare greater than unsigned immediate" )
X( cmple,       base,    Rabc,    Rabc,     iic_default,       cmple,      "compare less than or equal" )
X( cmpleu,      base,    Rabc,    Rabc,     iic_default,       cmpleu,     "compare less than or equal unsigned" )
X( cmplt,       base,    Rabc,    Rabc,     iic_default,       cmplt,      "compare less than" )
X( cmplti,      base,    RabI21,  RabI21,   iic_default,       cmplti,     "compare less than immediate" )
X( cmpltu,      base,    Rabc,    Rabc,     iic_default,       cmpltu,     "compare less than unsigned" )
X( cmpltui,     base,    RabU21,  RabU21,   iic_default,       cmpltui,    "compare less than unsigned immediate" )
X( cmpne,       base,    Rabc,    Rabc,     iic_default,       cmpne,      "compare not equal" )
X( cmpnei,      base,    RabI21,  RabI21,   iic_default,       cmpnei,     "compare not equal immediate" )

//nullification
X( cnbc,        base,    r2m2,    r2m2,     iic_default,       cnbc,       "conditional nullify if bit clear" )
X( cnbci,       base,    RaSbm2,  RaSbm2,   iic_default,       cnbci,      "conditional nullify if bit clear immediate" )
X( cnbs,        base,    r2m2,    r2m2,     iic_default,       cnbs,       "conditional nullify if bit set" )
X( cnbsi,       base,    RaSbm2,  RaSbm2,   iic_default,       cnbsi,      "conditional nullify if bit set immediate" )
X( cneq,        base,    r2m2,    r2m2,     iic_default,       cneq,       "conditional nullify if equal" )
X( cneqi,       base,    RaI14m2, RaI14m2,  iic_default,       cneqi,      "conditional nullify if equal immediate" )
X( cngt,        base,    r2m2,    r2m2,     iic_default,       cngt,       "conditional nullify if greater than" )
X( cngti,       base,    RaI14m2, RaI14m2,  iic_default,       cngti,      "conditional nullify if greater than immediate" )
X( cngtu,       base,    r2m2,    r2m2,     iic_default,       cngtu,      "conditional nullify if greater than unsigned" )
X( cngtui,      base,    RaU14m2, RaU14m2,  iic_default,       cngtui,     "conditional nullify if greater than unsigned immediate" )
X( cnlt,        base,    r2m2,    r2m2,     iic_default,       cnlt,       "conditional nullify if less than" )
X( cnlti,       base,    RaI14m2, RaI14m2,  iic_default,       cnlti,      "conditional nullify if less than immediate" )
X( cnltu,       base,    r2m2,    r2m2,     iic_default,       cnltu,      "conditional nullify if less than unsigned" )
X( cnltui,      base,    RaU14m2, RaU14m2,  iic_default,       cnltui,     "conditional nullify if less than unsigned immediate" )
X( cnne,        base,    r2m2,    r2m2,     iic_default,       cnne,       "conditional nullify if not-equal" )
X( cnnei,       base,    RaI14m2, RaI14m2,  iic_default,       cnnei,      "conditional nullify if not-equal immediate" )

//bit counting
X( cntlz,       base,    RabSc,  RabSc,    iic_default,       cntlz,      "count leading zeros" )
X( cntpop,      base,    RabSc,  RabSc,    iic_default,       cntpop,     "count population" )
X( cnttz,       base,    RabSc,  RabSc,    iic_default,       cnttz,      "count trailing zeros" )

X( cpuid,       system,  gmemu,  RabU14,   iic_default,       cpuid,      "cpu identification" )

//conversions
X( cvti2pd,     fpud,    Round,  Round,    iic_default,       cvti2pd,    "convert integer to packed double" )
X( cvti2ps,     fpus,    Round,  Round,    iic_default,       cvti2ps,    "convert integer to packed single" )

X( cvti2sd,     fpud,    Round,  Round,    iic_default,       cvti2sd,    "convert integer to scalar double" )
X( cvti2sq,     fpuq,    Round,  Round,    iic_default,       cvti2sq,    "convert integer to scalar quadruple" )
X( cvti2ss,     fpus,    Round,  Round,    iic_default,       cvti2ss,    "convert integer to scalar single" )

X( cvtpd2i,     fpud,    Round,  Round,    iic_default,       cvtpd2i,    "convert packed double to integers" )
X( cvtpd2u,     fpud,    Round,  Round,    iic_default,       cvtpd2u,    "convert packed double to unsigned integers" )
X( cvtps2i,     fpus,    Round,  Round,    iic_default,       cvtps2i,    "convert packed single to integers" )
X( cvtps2u,     fpus,    Round,  Round,    iic_default,       cvtps2u,    "convert packed single to unsigned integers" )

X( cvtsd2i,     fpud,    Round,  Round,    iic_default,       cvtsd2i,    "convert scalar double to integer" )
X( cvtsd2sq,    fpud,    Round,  Round,    iic_default,       cvtsd2sq,   "convert scalar double to scalar quadruple" )
X( cvtsd2ss,    fpud,    Round,  Round,    iic_default,       cvtsd2ss,   "convert scalar double to scalar single" )
X( cvtsd2u,     fpud,    Round,  Round,    iic_default,       cvtsd2u,    "convert scalar double to unsigned integer" )

X( cvtsq2i,     fpuq,    Round,  Round,    iic_default,       cvtsq2i,    "convert scalar quadruple to integer" )
X( cvtsq2sd,    fpuq,    Round,  Round,    iic_default,       cvtsq2sd,   "convert scalar quadruple to scalar double" )
X( cvtsq2ss,    fpuq,    Round,  Round,    iic_default,       cvtsq2ss,   "convert scalar quadruple to scalar single" )
X( cvtsq2u,     fpuq,    Round,  Round,    iic_default,       cvtsq2u,    "convert scalar quadruple to unsigned integer" )

X( cvtss2i,     fpus,    Round,  Round,    iic_default,       cvtss2i,    "convert scalar single to integer" )
X( cvtss2sd,    fpus,    Round,  Round,    iic_default,       cvtss2sd,   "convert scalar single to scalar double" )
X( cvtss2sq,    fpus,    Round,  Round,    iic_default,       cvtss2sq,   "convert scalar single to scalar quadruple" )
X( cvtss2u,     fpus,    Round,  Round,    iic_default,       cvtss2u,    "convert scalar single to unsigned integer" )

X( cvtu2pd,     fpud,    Round,  Round,    iic_default,       cvtu2pd,    "convert unsigned integers to packed double" )
X( cvtu2ps,     fpus,    Round,  Round,    iic_default,       cvtu2ps,    "convert unsigned integers to packed single" )

X( cvtu2sd,     fpud,    Round,  Round,    iic_default,       cvtu2sd,    "convert unsigned integer to scalar double" )
X( cvtu2sq,     fpuq,    Round,  Round,    iic_default,       cvtu2sq,    "convert unsigned integer to scalar quadruple" )
X( cvtu2ss,     fpus,    Round,  Round,    iic_default,       cvtu2ss,    "convert unsigned integer to scalar single" )

X( dcbf,        system,  RaxbI21, RaxbI21,  iic_default,       dcbf,       "data cache block flush" )
X( dcbi,        kernel,  RaxbI21, RaxbI21,  iic_default,       dcbi,       "data cache block invalidate" )
X( dcbt,        system,  RaxbI21, RaxbI21,  iic_default,       dcbt,       "data cache block touch" )

X( dep,         base,    RabcSde, RabcSde,  iic_default,       dep,        "deposit" )
X( dep16,       base,    RabcSd,  FabcSd,   iic_default,       dep16,      "deposit 16-byte" )
X( depa,        base,    RabScd,  RabScd,   iic_default,       depa,       "deposit alter" )
X( depc,        base,    RabScd,  RabScd,   iic_default,       depc,       "deposit clear" )
X( deps,        base,    RabScd,  RabScd,   iic_default,       deps,       "deposit set" )

X( div,         base,    Rabc,   Rabc,     iic_default,       div,        "divide" )
X( divi,        base,    RabI21, RabI21,   iic_default,       divi,       "divide immediate" )
X( divu,        base,    Rabc,   Rabc,     iic_default,       divu,       "divide unsigned" )
X( divui,       base,    RabU21, RabU21,   iic_default,       divui,      "divide unsigned immediate" )

// software exceptions
X( ehadj,       base,    br,       br,     iic_default,       ehadj,      "exception handler adjust eip" )
X( ehcatch,     base,    ldar,     callr,  iic_default,       ehcatch,    "exception handler catch" )
X( ehnext,      base,    ldar,     callr,  iic_default,       ehnext,     "exception handler next" )
X( ehthrow,     base,    RaxbI21,  RaxbI21,iic_default,       ehthrow,    "exception handler throw" )

X( enter,       base,    RaxbI21,  RaxbI21,iic_default,       enter,      "enter stack frame" )

// floating-point
X( fabsdpd,     fpud,    Rabc,   Rabc,     iic_default,       fabsdpd,    "fp absolute difference packed double" )
X( fabsdps,     fpus,    Rabc,   Rabc,     iic_default,       fabsdps,    "fp absolute difference packed single" )

X( fabsdsd,     fpud,    Rabc,   Rabc,     iic_default,       fabsdsd,    "fp absolute difference scalar double" )
X( fabsdsq,     fpuq,    Rabc,   Rabc,     iic_default,       fabsdsq,    "fp absolute difference scalar quadruple" )
X( fabsdss,     fpus,    Rabc,   Rabc,     iic_default,       fabsdss,    "fp absolute difference scalar single" )

X( fabssd,      fpud,    Rab,    Rab,      iic_default,       fabssd,     "fp absolute value scalar double" )
X( fabssq,      fpuq,    Rab,    Rab,      iic_default,       fabssq,     "fp absolute value scalar quadruple" )
X( fabsss,      fpus,    Rab,    Rab,      iic_default,       fabsss,     "fp absolute value scalar single" )

X( faddcpd,     fpud,    Rabc,   Rabc,     iic_default,       faddcpd,    "fp add conjugate packed double" )
X( faddcps,     fpus,    Rabc,   Rabc,     iic_default,       faddcps,    "fp add conjugate packed single" )

X( faddhpd,     fpud,    Rabc,   Rabc,     iic_default,       faddhpd,    "fp add horizontal packed double" )
X( faddhps,     fpus,    Rabc,   Rabc,     iic_default,       faddhps,    "fp add horizontal packed single" )

X( faddpd,      fpud,    Rabc,   Rabc,     iic_default,       faddpd,     "fp add packed double" )
X( faddps,      fpus,    Rabc,   Rabc,     iic_default,       faddps,     "fp add packed single" )

X( faddsd,      fpud,    Rabc,   Rabc,     iic_default,       faddsd,     "fp add scalar double" )
X( faddsq,      fpuq,    Rabc,   Rabc,     iic_default,       faddsq,     "fp add scalar quadruple" )
X( faddss,      fpus,    Rabc,   Rabc,     iic_default,       faddss,     "fp add scalar single" )

X( famaxpd,     fpud,    Rabc,   Rabc,     iic_default,       famaxpd,    "fp absolute maximum packed double" )
X( famaxps,     fpus,    Rabc,   Rabc,     iic_default,       famaxps,    "fp absolute maximum packed single" )

X( famaxsd,     fpud,    Rabc,   Rabc,     iic_default,       famaxsd,    "fp absolute maximum scalar double" )
X( famaxsq,     fpuq,    Rabc,   Rabc,     iic_default,       famaxsq,    "fp absolute maximum scalar quadruple" )
X( famaxss,     fpus,    Rabc,   Rabc,     iic_default,       famaxss,    "fp absolute maximum scalar double" )

X( faminpd,     fpud,    Rabc,   Rabc,     iic_default,       faminpd,    "fp absolute minimum packed double" )
X( faminps,     fpus,    Rabc,   Rabc,     iic_default,       faminps,    "fp absolute minimum packed single" )

X( faminsd,     fpud,    Rabc,   Rabc,     iic_default,       faminsd,    "fp absolute minimum scalar double" )
X( faminsq,     fpuq,    Rabc,   Rabc,     iic_default,       faminsq,    "fp absolute minimum scalar quadruple" )
X( faminss,     fpus,    Rabc,   Rabc,     iic_default,       faminss,    "fp absolute minimum scalar single" )

//fp compares
X( fcmppd,    fpud,    fpcmp,   fpcmp,     iic_default,       fcmppd,   "fp compare packed double" )
X( fcmpps,    fpus,    fpcmp,   fpcmp,     iic_default,       fcmpps,   "fp compare packed single" )
X( fcmpsd,    fpud,    fpcmp,   fpcmp,     iic_default,       fcmpsd,   "fp compare scalar double" )
X( fcmpsq,    fpuq,    fpcmp,   fpcmp,     iic_default,       fcmpsq,   "fp compare scalar quadruple" )
X( fcmpss,    fpus,    fpcmp,   fpcmp,     iic_default,       fcmpss,   "fp compare scalar single" )

X( fdivpd,      fpud,    Rabc,   Rabc,     iic_default,       fdivpd,     "fp divide packed double" )
X( fdivps,      fpus,    Rabc,   Rabc,     iic_default,       fdivps,     "fp divide packed single" )
X( fdivsd,      fpud,    Rabc,   Rabc,     iic_default,       fdivsd,     "fp divide scalar double" )
X( fdivsq,      fpuq,    Rabc,   Rabc,     iic_default,       fdivsq,     "fp divide scalar quadruple" )
X( fdivss,      fpus,    Rabc,   Rabc,     iic_default,       fdivss,     "fp divide scalar single" )

X( fdotpd,      fpud,    Rabc,   Rabc,     iic_default,       fdotpd,     "fp dot-product packed double" )
X( fdotps,      fpus,    Rabc,   Rabc,     iic_default,       fdotps,     "fp dot-product packed single" )

// floating point immediate load pseudos
X( flddi,       fpud,    RaU28,   flddi,    iic_default,       ldi,        "fp load double immediate" )
X( flddri,      fpud,    RaU28,   flddri,   iic_default,       ld8r,       "fp load double relative with immediate" )
X( fldqi,       fpud,    RaU28,   fldqi,    iic_default,       ldi,        "fp load quadruple immediate (low part)" )
X( fldqih,      fpud,    RaU28,   fldqih,   iic_default,       ldih,       "fp load quadruple immediate (high part)" )
X( fldqri,      fpuq,    RaU28,   fldqri,   iic_default,       ld16r,      "fp load quadruple relative with immediate" )
X( fldsi,       fpus,    RaU28,   fldsi,    iic_default,       ldi,        "fp load single immediate" )
X( fldsri,      fpus,    RaU28,   fldsri,   iic_default,       ld4r,       "fp load single relative with immediate" )

X( fmaddapd,    fpud,    Rabcd,  Rabcd,    iic_default,       fmaddapd,   "fp multiply-alternating add-subtract packed double" )
X( fmaddaps,    fpus,    Rabcd,  Rabcd,    iic_default,       fmaddaps,   "fp multiply-alternating add-subtract packed single" )

X( fmaddpd,     fpud,    Rabcd,  Rabcd,    iic_default,       fmaddpd,    "fp multiply-add packed double" )
X( fmaddps,     fpus,    Rabcd,  Rabcd,    iic_default,       fmaddps,    "fp multiply-add packed single" )

X( fmaddsd,     fpud,    Rabcd,  Rabcd,    iic_default,       fmaddsd,    "fp multiply-add scalar double" )
X( fmaddsq,     fpuq,    Rabcd,  Rabcd,    iic_default,       fmaddsq,    "fp multiply-add scalar quadruple" )
X( fmaddss,     fpus,    Rabcd,  Rabcd,    iic_default,       fmaddss,    "fp multiply-add scalar single" )

X( fmaxpd,      fpud,    Rabc,   Rabc,     iic_default,       fmaxpd,     "fp maximum packed double" )
X( fmaxps,      fpus,    Rabc,   Rabc,     iic_default,       fmaxps,     "fp maximum packed single" )

X( fmaxsd,      fpud,    Rabc,   Rabc,     iic_default,       fmaxsd,     "fp maximum scalar double" )
X( fmaxsq,      fpuq,    Rabc,   Rabc,     iic_default,       fmaxsq,     "fp maximum scalar quadruple" )
X( fmaxss,      fpus,    Rabc,   Rabc,     iic_default,       fmaxss,     "fp maximum scalar single" )

X( fmergepd,    fpud,    Rabcd,  Rabcd,    iic_default,       fmergepd,   "fp merge packed double" )
X( fmergeps,    fpus,    Rabcd,  Rabcd,    iic_default,       fmergeps,   "fp merge packed single" )

X( fmergesd,    fpud,    Rabcd,  Rabcd,    iic_default,       fmergesd,   "fp merge scalar double" )
X( fmergesq,    fpuq,    Rabcd,  Rabcd,    iic_default,       fmergesq,   "fp merge scalar quadruple" )
X( fmergess,    fpus,    Rabcd,  Rabcd,    iic_default,       fmergess,   "fp merge scalar single" )

X( fminpd,      fpud,    Rabc,   Rabc,     iic_default,       fminpd,     "fp minimum packed double" )
X( fminps,      fpus,    Rabc,   Rabc,     iic_default,       fminps,     "fp minimum packed single" )

X( fminsd,      fpud,    Rabc,   Rabc,     iic_default,       fminsd,     "fp minimum scalar double" )
X( fminsq,      fpuq,    Rabc,   Rabc,     iic_default,       fminsq,     "fp minimum scalar quadruple" )
X( fminss,      fpus,    Rabc,   Rabc,     iic_default,       fminss,     "fp minimum scalar single" )

X( fmrghpd,     fpud,    Rabc,   Rabc,     iic_default,       fmrghpd,    "fp merge high parts" )
X( fmrghps,     fpus,    Rabc,   Rabc,     iic_default,       fmrghps,    "fp merge high parts" )
X( fmrglpd,     fpud,    Rabc,   Rabc,     iic_default,       fmrglpd,    "fp merge low parts" )
X( fmrglps,     fpus,    Rabc,   Rabc,     iic_default,       fmrglps,    "fp merge low parts" )

X( fmsubapd,    fpud,    Rabcd,  Rabcd,    iic_default,       fmsubapd,   "fp multiply-alternating subtract-add packed double" )
X( fmsubaps,    fpus,    Rabcd,  Rabcd,    iic_default,       fmsubaps,   "fp multiply-alternating subtract-add packed single" )

X( fmsubpd,     fpud,    Rabcd,  Rabcd,    iic_default,       fmsubpd,    "fp multiply-subtract packed double" )
X( fmsubps,     fpus,    Rabcd,  Rabcd,    iic_default,       fmsubps,    "fp multiply-subtract packed single" )

X( fmsubsd,     fpud,    Rabcd,  Rabcd,    iic_default,       fmsubsd,    "fp multiply-subtract scalar double" )
X( fmsubsq,     fpuq,    Rabcd,  Rabcd,    iic_default,       fmsubsq,    "fp multiply-subtract scalar quadruple" )
X( fmsubss,     fpus,    Rabcd,  Rabcd,    iic_default,       fmsubss,    "fp multiply-subtract scalar single" )


X( fmulhpd,     fpud,    Rabc,   Rabc,     iic_default,       fmulhpd,    "fp multiply horizontal packed double" )
X( fmulhps,     fpus,    Rabc,   Rabc,     iic_default,       fmulhps,    "fp multiply horizontal packed single" )

X( fmulpd,      fpud,    Rabc,   Rabc,     iic_default,       fmulpd,     "fp multiply packed double" )
X( fmulps,      fpus,    Rabc,   Rabc,     iic_default,       fmulps,     "fp multiply packed single" )

X( fmulsd,      fpud,    Rabc,   Rabc,     iic_default,       fmulsd,     "fp multiply scalar double" )
X( fmulsq,      fpuq,    Rabc,   Rabc,     iic_default,       fmulsq,     "fp multiply scalar quadruple" )
X( fmulss,      fpus,    Rabc,   Rabc,     iic_default,       fmulss,     "fp multiply scalar single" )


X( fnabsdpd,    fpud,    Rabc,   Rabc,     iic_default,       fnabsdpd,   "fp negate absolute difference packed double" )
X( fnabsdps,    fpus,    Rabc,   Rabc,     iic_default,       fnabsdps,   "fp negate absolute difference packed single" )

X( fnabsdsd,    fpud,    Rabc,   Rabc,     iic_default,       fnabsdsd,   "fp negate absolute difference scalar double" )
X( fnabsdsq,    fpuq,    Rabc,   Rabc,     iic_default,       fnabsdsq,   "fp negate absolute difference scalar quadruple" )
X( fnabsdss,    fpus,    Rabc,   Rabc,     iic_default,       fnabsdss,   "fp negate absolute difference scalar single" )

X( fnabssd,     fpud,    Rab,    Rab,      iic_default,       fnabssd,    "fp negate absolute value scalar double" )
X( fnabssq,     fpuq,    Rab,    Rab,      iic_default,       fnabssq,    "fp negate absolute value scalar quadruple" )
X( fnabsss,     fpus,    Rab,    Rab,      iic_default,       fnabsss,    "fp negate absolute value scalar single" )

X( fnaddpd,     fpud,    Rabc,   Rabc,     iic_default,       fnaddpd,    "fp negate add packed double" )
X( fnaddps,     fpus,    Rabc,   Rabc,     iic_default,       fnaddps,    "fp negate add packed single" )
X( fnaddsd,     fpud,    Rabc,   Rabc,     iic_default,       fnaddsd,    "fp negate add scalar double" )
X( fnaddsq,     fpuq,    Rabc,   Rabc,     iic_default,       fnaddsq,    "fp negate add scalar quadruple" )
X( fnaddss,     fpus,    Rabc,   Rabc,     iic_default,       fnaddss,    "fp negate add scalar single" )

X( fnegpd,      fpud,    Rab,    Rab,      iic_default,       fnegpd,     "fp negate packed double" )
X( fnegps,      fpus,    Rab,    Rab,      iic_default,       fnegps,     "fp negate packed single" )
X( fnegsd,      fpud,    Rab,    Rab,      iic_default,       fnegsd,     "fp negate scalar double" )
X( fnegsq,      fpuq,    Rab,    Rab,      iic_default,       fnegsq,     "fp negate scalar quadruple" )
X( fnegss,      fpus,    Rab,    Rab,      iic_default,       fnegss,     "fp negate scalar single" )

X( fnmaddpd,    fpud,    Rabcd,  Rabcd,    iic_default,       fnmaddpd,   "fp negate multiply-add packed double" )
X( fnmaddps,    fpus,    Rabcd,  Rabcd,    iic_default,       fnmaddps,   "fp negate multiply-add packed single" )

X( fnmaddsd,    fpud,    Rabcd,  Rabcd,    iic_default,       fnmaddsd,   "fp negate multiply-add scalar double" )
X( fnmaddsq,    fpuq,    Rabcd,  Rabcd,    iic_default,       fnmaddsq,   "fp negate multiply-add scalar quadruple" )
X( fnmaddss,    fpus,    Rabcd,  Rabcd,    iic_default,       fnmaddss,   "fp negate multiply-add scalar single" )

X( fnmsubpd,    fpud,    Rabcd,  Rabcd,    iic_default,       fnmsubpd,   "fp negate multiply-subtract packed double" )
X( fnmsubps,    fpus,    Rabcd,  Rabcd,    iic_default,       fnmsubps,   "fp negate multiply-subtract packed single" )

X( fnmsubsd,    fpud,    Rabcd,  Rabcd,    iic_default,       fnmsubsd,   "fp negate multiply-subtract scalar double" )
X( fnmsubsq,    fpuq,    Rabcd,  Rabcd,    iic_default,       fnmsubsq,   "fp negate multiply-subtract scalar quadruple" )
X( fnmsubss,    fpus,    Rabcd,  Rabcd,    iic_default,       fnmsubss,   "fp negate multiply-subtract scalar single" )

X( fnmulpd,     fpud,    Rabc,   Rabc,     iic_default,       fnmulpd,    "fp negate multiply packed double" )
X( fnmulps,     fpus,    Rabc,   Rabc,     iic_default,       fnmulps,    "fp negate multiply packed single" )

X( fnmulsd,     fpud,    Rabc,   Rabc,     iic_default,       fnmulsd,    "fp negate multiply scalar double" )
X( fnmulsq,     fpuq,    Rabc,   Rabc,     iic_default,       fnmulsq,    "fp negate multiply scalar quadruple" )
X( fnmulss,     fpus,    Rabc,   Rabc,     iic_default,       fnmulss,    "fp negate multiply scalar single" )

X( fpkpd,       fpud,    Rabc,   Rabc,     iic_default,       fpkpd,      "fp pack" )
X( fpkps,       fpus,    Rabc,   Rabc,     iic_default,       fpkps,      "fp pack" )

X( frndpd,      fpud,    Round,    Round,  iic_default,       frndpd,     "fp round packed double" )
X( frndps,      fpus,    Round,    Round,  iic_default,       frndps,     "fp round packed single" )
X( frndsd,      fpud,    Round,    Round,  iic_default,       frndsd,     "fp round scalar double" )
X( frndsq,      fpuq,    Round,    Round,  iic_default,       frndsq,     "fp round scalar quadruple" )
X( frndss,      fpus,    Round,    Round,  iic_default,       frndss,     "fp round scalar single" )

X( frsqrtpd,    fpud,    Rab,    Rab,      iic_default,       frsqrtpd,   "fp reciprocal square root packed double" )
X( frsqrtps,    fpus,    Rab,    Rab,      iic_default,       frsqrtps,   "fp reciprocal square root packed single" )

X( frsqrtsd,    fpud,    Rab,    Rab,      iic_default,       frsqrtsd,   "fp reciprocal square root scalar double" )
X( frsqrtsq,    fpuq,    Rab,    Rab,      iic_default,       frsqrtsq,   "fp reciprocal square root scalar quadruple" )
X( frsqrtss,    fpus,    Rab,    Rab,      iic_default,       frsqrtss,   "fp reciprocal square root scalar single" )

X( fscale,      fpuq,    RabSc,  FabSc,    iic_default,       fscale,     "fp scale" )

X( fsqrtpd,     fpud,    Rab,    Rab,      iic_default,       fsqrtpd,    "fp square root packed double" )
X( fsqrtps,     fpus,    Rab,    Rab,      iic_default,       fsqrtps,    "fp square root packed single" )

X( fsqrtsd,     fpud,    Rab,    Rab,      iic_default,       fsqrtsd,    "fp square root scalar double" )
X( fsqrtsq,     fpuq,    Rab,    Rab,      iic_default,       fsqrtsq,    "fp square root scalar quadruple" )
X( fsqrtss,     fpus,    Rab,    Rab,      iic_default,       fsqrtss,    "fp square root scalar single" )

X( fsubcpd,     fpud,    Rabc,   Rabc,     iic_default,       fsubcpd,    "fp subtract conjugate packed double" )
X( fsubcps,     fpus,    Rabc,   Rabc,     iic_default,       fsubcps,    "fp subtract conjugate packed single" )
X( fsubhpd,     fpud,    Rabc,   Rabc,     iic_default,       fsubhpd,    "fp subtract horizontal packed double" )
X( fsubhps,     fpus,    Rabc,   Rabc,     iic_default,       fsubhps,    "fp subtract horizontal packed single" )
X( fsubpd,      fpud,    Rabc,   Rabc,     iic_default,       fsubpd,     "fp subtract packed double" )
X( fsubps,      fpus,    Rabc,   Rabc,     iic_default,       fsubps,     "fp subtract packed single" )

X( fsubsd,      fpud,    Rabc,   Rabc,     iic_default,       fsubsd,     "fp subtract scalar double" )
X( fsubsq,      fpuq,    Rabc,   Rabc,     iic_default,       fsubsq,     "fp subtract scalar quadruple" )
X( fsubss,      fpus,    Rabc,   Rabc,     iic_default,       fsubss,     "fp subtract scalar single" )

X( funpkhpd,    fpud,    Rab,    Rab,      iic_default,       funpkhpd,   "fp unpack high part" )
X( funpkhps,    fpus,    Rab,    Rab,      iic_default,       funpkhps,   "fp unpack high part" )
X( funpklpd,    fpud,    Rab,    Rab,      iic_default,       funpklpd,   "fp unpack low part" )
X( funpklps,    fpus,    Rab,    Rab,      iic_default,       funpklps,   "fp unpack low part" )

X( gtb,         base,    Rab,    Rab,      iic_default,       gtb,        "graycode to binary" )
// system
X( halt,        kernel,  NoArgs, NoArgs,   iic_default,       halt,       "halt processor" )
X( icbi,        system,  RaxbI21, RaxbI21, iic_default,       icbi,       "instruction cache block invalidate" )

X( int,         system,  gmemu,  RabU14,   iic_default,       int,        "interrupt" )

// loads
X( ld1,         base,    RabI21, RabI21,   iic_default,       ld1,       "load 1-byte" )

X( ld16,        base,    RabI21, RabI21,   iic_default,       ld16,      "load 16-byte" )
X( ld16a,       atomic,  aload,  aload,    iic_default,       ld16a,     "load 16-byte atomic" )
X( ld16mia,     base,    gmemu,  gmemu,    iic_default,       ld16mia,   "load 16-byte and modify immediate after" )
X( ld16mib,     base,    gmemu,  gmemu,    iic_default,       ld16mib,   "load 16-byte and modify immediate before" )
X( ld16mxa,     base,    gmemux, gmemux,   iic_default,       ld16mxa,   "load 16-byte and modify indexed after" )
X( ld16r,       base,    RaU28,  gmemr,    iic_default,       ld16r,     "load 16-byte relative" )
X( ld16x,       base,    gmemx,  gmemx,    iic_default,       ld16x,     "load 16-byte indexed" )

X( ld1a,        atomic,  aload,  aload,    iic_default,       ld1a,      "load 1-byte atomic" )
X( ld1mia,      base,    gmemu,  gmemu,    iic_default,       ld1mia,    "load 1-byte and modify immediate after" )
X( ld1mib,      base,    gmemu,  gmemu,    iic_default,       ld1mib,    "load 1-byte and modify immediate before" )
X( ld1mxa,      base,    gmemux, gmemux,   iic_default,       ld1mxa,    "load 1-byte and modify indexed after" )
X( ld1r,        base,    RaU28,  gmemr,    iic_default,       ld1r,      "load 1-byte relative" )
X( ld1x,        base,    gmemx,  gmemx,    iic_default,       ld1x,      "load 1-byte indexed" )

X( ld2,         base,    RabI21, RabI21,   iic_default,       ld2,       "load 2-byte" )
X( ld2a,        atomic,  aload,  aload,    iic_default,       ld2a,      "load 2-byte atomic" )
X( ld2mia,      base,    gmemu,  gmemu,    iic_default,       ld2mia,    "load 2-byte and modify immediate after" )
X( ld2mib,      base,    gmemu,  gmemu,    iic_default,       ld2mib,    "load 2-byte and modify immediate before" )
X( ld2mxa,      base,    gmemux, gmemux,   iic_default,       ld2mxa,    "load 2-byte and modify indexed after" )
X( ld2r,        base,    RaU28,  gmemr,    iic_default,       ld2r,      "load 2-byte relative" )
X( ld2x,        base,    gmemx,  gmemx,    iic_default,       ld2x,      "load 2-byte indexed" )

X( ld4,         base,    RabI21, RabI21,   iic_default,       ld4,       "load 4-byte" )
X( ld4a,        atomic,  aload,  aload,    iic_default,       ld4a,      "load 4-byte atomic" )
X( ld4mia,      base,    gmemu,  gmemu,    iic_default,       ld4mia,    "load 4-byte and modify immediate after" )
X( ld4mib,      base,    gmemu,  gmemu,    iic_default,       ld4mib,    "load 4-byte and modify immediate before" )
X( ld4mxa,      base,    gmemux, gmemux,   iic_default,       ld4mxa,    "load 4-byte and modify indexed after" )
X( ld4r,        base,    RaU28,  gmemr,    iic_default,       ld4r,      "load 4-byte relative" )
X( ld4x,        base,    gmemx,  gmemx,    iic_default,       ld4x,      "load 4-byte indexed" )

X( ld8,         base,    RabI21, RabI21,   iic_default,       ld8,       "load 8-byte" )
X( ld8a,        atomic,  aload,  aload,    iic_default,       ld8a,      "load 8-byte atomic" )
X( ld8mia,      base,    gmemu,  gmemu,    iic_default,       ld8mia,    "load 8-byte and modify immediate after" )
X( ld8mib,      base,    gmemu,  gmemu,    iic_default,       ld8mib,    "load 8-byte and modify immediate before" )
X( ld8mxa,      base,    gmemux, gmemux,   iic_default,       ld8mxa,    "load 8-byte and modify indexed after" )
X( ld8r,        base,    RaU28,  gmemr,    iic_default,       ld8r,      "load 8-byte relative" )
X( ld8x,        base,    gmemx,  gmemx,    iic_default,       ld8x,      "load 8-byte indexed" )

X( ldan,        base,    RabI21, RabI21,   iic_default,       ldan,      "load address near" )
X( ldar,        base,    ldar,   ldar,     iic_default,       ldar,      "load address relative" )
X( ldax,        base,    gmemx,  gmemx,    iic_default,       ldax,      "load address indexed" )

X( ldi,         base,    ldi,    ldi,      iic_default,       ldi,       "load immediate" )
X( ldih,        base,    ldi,    ldi,      iic_default,       ldih,      "load immediate into high 64-bit" )

X( ldr,         base,    RaU28,  gmemr,    iic_default,       ldr,       "load relative" )

X( lds1,        base,    RabI21, RabI21,   iic_default,       lds1,      "load signed 1-byte" )
X( lds1mia,     base,    gmemu,  gmemu,    iic_default,       lds1mia,   "load signed 1-byte and modify immediate after" )
X( lds1mib,     base,    gmemu,  gmemu,    iic_default,       lds1mib,   "load signed 1-byte and modify immediate before" )
X( lds1mxa,     base,    gmemux, gmemux,   iic_default,       lds1mxa,   "load signed 1-byte and modify indexed after" )
X( lds1r,       base,    RaU28,  gmemr,    iic_default,       lds1r,     "load signed 1-byte relative" )
X( lds1x,       base,    gmemx,  gmemx,    iic_default,       lds1x,     "load signed 1-byte indexed" )

X( lds2,        base,    RabI21, RabI21,   iic_default,       lds2,      "load signed 2-byte" )
X( lds2mia,     base,    gmemu,  gmemu,    iic_default,       lds2mia,   "load signed 2-byte and modify immediate after" )
X( lds2mib,     base,    gmemu,  gmemu,    iic_default,       lds2mib,   "load signed 2-byte and modify immediate before" )
X( lds2mxa,     base,    gmemux, gmemux,   iic_default,       lds2mxa,   "load signed 2-byte and modify indexed after" )
X( lds2r,       base,    RaU28,  gmemr,    iic_default,       lds2r,     "load signed 2-byte relative" )
X( lds2x,       base,    gmemx,  gmemx,    iic_default,       lds2x,     "load signed 2-byte indexed" )

X( lds4,        base,    RabI21, RabI21,   iic_default,       lds4,      "load signed 4-byte" )
X( lds4mia,     base,    gmemu,  gmemu,    iic_default,       lds4mia,   "load signed 4-byte and modify immediate after" )
X( lds4mib,     base,    gmemu,  gmemu,    iic_default,       lds4mib,   "load signed 4-byte and modify immediate before" )
X( lds4mxa,     base,    gmemux, gmemux,   iic_default,       lds4mxa,   "load signed 4-byte and modify indexed after" )
X( lds4r,       base,    RaU28,  gmemr,    iic_default,       lds4r,     "load signed 4-byte relative" )
X( lds4x,       base,    gmemx,  gmemx,    iic_default,       lds4x,     "load signed 4-byte indexed" )

X( lds8,        base,    RabI21, RabI21,   iic_default,       lds8,      "load signed 8-byte" )
X( lds8mia,     base,    gmemu,  gmemu,    iic_default,       lds8mia,   "load signed 8-byte and modify immediate after" )
X( lds8mib,     base,    gmemu,  gmemu,    iic_default,       lds8mib,   "load signed 8-byte and modify immediate before" )
X( lds8mxa,     base,    gmemux, gmemux,   iic_default,       lds8mxa,   "load signed 8-byte and modify indexed after" )
X( lds8r,       base,    RaU28,  gmemr,    iic_default,       lds8r,     "load signed 8-byte relative" )
X( lds8x,       base,    gmemx,  gmemx,    iic_default,       lds8x,     "load signed 8-byte indexed" )

X( leave,       base,    Rb,     Rb,       iic_default,       leave,     "leave stack frame" )

X( maxs,        base,    Rabc,   Rabc,     iic_default,       maxs,     "maximum signed" )
X( maxsi,       base,    RabI21, RabI21,   iic_default,       maxsi,    "maximum signed immediate" )
X( maxu,        base,    Rabc,   Rabc,     iic_default,       maxu,     "maximum unsigned" )
X( maxui,       base,    RabU21, RabU21,   iic_default,       maxui,    "maximum unsigned immediate" )
X( mbgath,      base,    Rabc,   Rabc,     iic_default,       mbgath,   "masked bit gather" )
X( mbscat,      base,    Rabc,   Rabc,     iic_default,       mbscat,   "masked bit scatter" )
X( mbsel,       base,    Rabcd,  Rabcd,    iic_default,       mbsel,    "masked bit selection" )

X( mfdbr,       kernel,  Rab,    Rab,      iic_default,       mfdbr,    "move from data breakpoint register" )
X( mfibr,       kernel,  Rab,    Rab,      iic_default,       mfibr,    "move from instruction breakpoint register" )
X( mfmr,        kernel,  Rab,    Rab,      iic_default,       mfmr,     "move from monitor register" )
X( mfspr,       system,  mspr,   mspr,     iic_default,       mfspr,    "move from special-purpose register" )

X( mins,        base,    Rabc,   Rabc,     iic_default,       mins,     "minimum signed" )
X( minsi,       base,    RabI21, RabI21,   iic_default,       minsi,    "minimum signed immediate" )
X( minu,        base,    Rabc,   Rabc,     iic_default,       minu,     "minimum unsigned" )
X( minui,       base,    RabU21, RabU21,   iic_default,       minui,    "minimum unsigned immediate" )

X( mod,         base,    Rabc,   Rabc,     iic_default,       mod,      "module" )
X( modi,        base,    RabI21, RabI21,   iic_default,       modi,     "module immediate" )
X( modu,        base,    Rabc,   Rabc,     iic_default,       modu,     "module unsigned" )
X( modui,       base,    RabU21, RabU21,   iic_default,       modui,    "module unsigned immediate" )

X( mov,         base,    Rab,    Rab,      iic_default,       mov,      "move general register" )

X( mprobe,      system,  Rabc,   Rabc,     iic_default,       mprobe,   "memory probe access" )

X( msync,       atomic,  NoArgs, NoArgs,   iic_default,       msync,    "memory synchronize" )

X( mtdbr,       kernel,  Rbc,    Rbc,      iic_default,       mtdbr,    "move to data breakpoint register" )
X( mtdtr,       kernel,  Rbc,    Rbc,      iic_default,       mtdtr,    "move to data translation register" )
X( mtibr,       kernel,  Rbc,    Rbc,      iic_default,       mtibr,    "move to instruction breakpoint register" )
X( mtitr,       kernel,  Rbc,    Rbc,      iic_default,       mtitr,    "move to instruction translation register" )
X( mtmr,        kernel,  Rbc,    Rbc,      iic_default,       mtmr,     "move to monitor register" )
X( mtspr,       system,  mspr,   mspr,     iic_default,       mtspr,    "move to special-purpose register" )

X( mul,         base,    Rabc,    Rabc,     iic_default,       mul,      "multiply" )
X( muladd,      base,    Rabcd,   Rabcd,    iic_default,       muladd,   "multiply-add" )
X( mulh,        base,    Rabc,    Rabc,     iic_default,       mulh,     "multiply high" )
X( muli,        base,    RabI21,  RabI21,   iic_default,       muli,     "multiply immediate" )
X( mulsub,      base,    Rabcd,   Rabcd,    iic_default,       mulsub,   "multiply-subtract" )
X( mulsubf,     base,    Rabcd,   Rabcd,    iic_default,       mulsubf,  "multiply-subtract from" )

X( nand,        base,    Rabc,    Rabc,     iic_default,       nand,     "bitwise not and" )
X( nop,         base,    RaxU28,  RaxU28,   iic_default,       nop,      "no operation" )
X( nor,         base,    Rabc,    Rabc,     iic_default,       nor,      "bitwise not or" )
X( not,         base,    RabI21,  Rab,      iic_default,       orni,     "bitwise not" )
X( or,          base,    Rabc,    Rabc,     iic_default,       or,       "bitwise or" )
X( ori,         base,    RabI21,  RabI21,   iic_default,       ori,      "bitwise or with immediate" )
X( orn,         base,    Rabc,    Rabc,     iic_default,       orn,      "bitwise or not" )
X( orni,        base,    RabI21,  RabI21,   iic_default,       orni,     "bitwise or not immediate" )

X( perm,        base,    Rabcd,   Rabcd,    iic_default,       perm,     "permute bytes" )
X( permb,       base,    RabSc,   RabSc,    iic_default,       permb,    "permute bits" )

X( ptc,         kernel,  RaxbI21,  RaxbI21,   iic_default,       ptc,      "purge translation cache" )

X( rdrand,      system,  Ra,       Ra,        iic_default,       rdrand,    "read random" )
X( rdseed,      system,  Ra,       Ra,        iic_default,       rdseed,    "read random seed" )

X( repge,       base,    loop,     loop,      iic_default,       repge,     "repeat on greater than or equal" )
X( repgeu,      base,    loop,     loop,      iic_default,       repgeu,    "repeat on greater than or equal unsigned" )
X( repgt,       base,    loop,     loop,      iic_default,       repgt,     "repeat on greater than" )
X( repgtu,      base,    loop,     loop,      iic_default,       repgtu,    "repeat on greater than unsigned" )
X( reple,       base,    loop,     loop,      iic_default,       reple,     "repeat on less than or equal" )
X( repleu,      base,    loop,     loop,      iic_default,       repleu,    "repeat on less than or equal unsigned" )
X( replt,       base,    loop,     loop,      iic_default,       replt,     "repeat on less than" )
X( repltu,      base,    loop,     loop,      iic_default,       repltu,    "repeat on less than unsigned" )

X( ret,         base,    Rb,       Rb,        iic_default,       ret,        "return from subroutine" )
X( rfi,         kernel,  NoArgs,   NoArgs,    iic_default,       rfi,        "return from interruption" )
X( rlmi,        base,    RabScde,  RabScde,   iic_default,       rlmi,       "rotate left and mask immediate" )

X( rscover,     kernel,  NoArgs, NoArgs,   iic_default,       rscover,    "register stack cover" )
X( rsflush,     kernel,  NoArgs, NoArgs,   iic_default,       rsflush,    "register stack flush" )
X( rsload,      kernel,  NoArgs, NoArgs,   iic_default,       rsload,     "register stack load" )

X( sladd,       base,    RabcSd, RabcSd,   iic_default,       sladd,     "shift left and add" )
X( sll,         base,    Rabc,   Rabc,     iic_default,       sll,       "shift left logical" )
X( slli,        base,    RabSc,  RabSc,    iic_default,       slli,      "shift left logical immediate" )
X( slp,         base,    Rabcd,  Rabcd,    iic_default,       slp,       "shift left pair" )
X( slsra,       base,    Rabcd,  Rabcd,    iic_default,       slsra,     "shift left and shift right algebraic" )
X( slsrai,      base,    RabScd, RabScd,   iic_default,       slsrai,    "shift left and shift right algebraic immediate" )
X( slsrl,       base,    Rabcd,  Rabcd,    iic_default,       slsrl,     "shift left and shift right logical" )
X( slsrli,      base,    RabScd, RabScd,   iic_default,       slsrli,    "shift left and shift right logical immediate" )
X( slsub,       base,    RabcSd, RabcSd,   iic_default,       slsub,     "shift left and subtract" )
X( slsubf,      base,    RabcSd, RabcSd,   iic_default,       slsubf,    "shift left and subtract from" )

X( sprsr,       system,  sprsr,  sprsr,    iic_default,       sprsr,     "special register set/reset" )

X( sra,         base,    Rabc,   Rabc,     iic_default,       sra,       "shift right algebraic" )
X( srai,        base,    RabSc,  RabSc,    iic_default,       srai,      "shift right algebraic immediate" )
X( srd,         base,    Rabc,   Rabc,     iic_default,       srd,       "shift right dividing" )
X( srdi,        base,    RabSc,  RabSc,    iic_default,       srdi,      "shift right dividing immediate" )
X( srl,         base,    Rabc,   Rabc,     iic_default,       srl,       "shift right logical" )
X( srli,        base,    RabSc,  RabSc,    iic_default,       srli,      "shift right logical immediate" )
X( srp,         base,    Rabcd,  Rabcd,    iic_default,       srp,       "shift right pair" )
X( srpi,        base,    RabcSd, RabcSd,   iic_default,       srpi,      "shift right pair immediate" )
X( srpi16,      base,    RabcSd, FabcSd,   iic_default,       srpi16,    "shift right pair immediate 16-byte" )

// stores in memory
X( st1,         base,    RabI21, RabI21,   iic_default,       st1,       "store 1-byte" )

X( st16,        base,    RabI21, RabI21,   iic_default,       st16,      "store 16-byte" )
X( st16a,       atomic,  aload,  aload,    iic_default,       st16a,     "store 16-byte atomic" )
X( st16mia,     base,    gmemu,  gmemu,    iic_default,       st16mia,   "store 16-byte and modify immediate after" )
X( st16mib,     base,    gmemu,  gmemu,    iic_default,       st16mib,   "store 16-byte and modify immediate before" )
X( st16mxa,     base,    gmemux, gmemux,   iic_default,       st16mxa,   "store 16-byte and modify indexed after" )
X( st16r,       base,    RaU28,  gmemr,    iic_default,       st16r,     "store 16-byte relative" )
X( st16x,       base,    gmemx,  gmemx,    iic_default,       st16x,     "store 16-byte indexed" )

X( st1a,        atomic,  aload,  aload,    iic_default,       st1a,      "store 1-byte atomic" )
X( st1mia,      base,    gmemu,  gmemu,    iic_default,       st1mia,    "store 1-byte and modify immediate after" )
X( st1mib,      base,    gmemu,  gmemu,    iic_default,       st1mib,    "store 1-byte and modify immediate before" )
X( st1mxa,      base,    gmemux, gmemux,   iic_default,       st1mxa,    "store 1-byte and modify indexed after" )
X( st1r,        base,    RaU28,  gmemr,    iic_default,       st1r,      "store 1-byte relative" )
X( st1x,        base,    gmemx,  gmemx,    iic_default,       st1x,      "store 1-byte indexed" )

X( st2,         base,    RabI21, RabI21,   iic_default,       st2,       "store 2-byte" )
X( st2a,        atomic,  aload,  aload,    iic_default,       st2a,      "store 2-byte atomic" )
X( st2mia,      base,    gmemu,  gmemu,    iic_default,       st2mia,    "store 2-byte and modify immediate after" )
X( st2mib,      base,    gmemu,  gmemu,    iic_default,       st2mib,    "store 2-byte and modify immediate before" )
X( st2mxa,      base,    gmemux, gmemux,   iic_default,       st2mxa,    "store 2-byte and modify indexed after" )
X( st2r,        base,    RaU28,  gmemr,    iic_default,       st2r,      "store 2-byte relative" )
X( st2x,        base,    gmemx,  gmemx,    iic_default,       st2x,      "store 2-byte indexed" )

X( st4,         base,    RabI21, RabI21,   iic_default,       st4,       "store 4-byte" )
X( st4a,        atomic,  aload,  aload,    iic_default,       st4a,      "store 4-byte atomic" )
X( st4mia,      base,    gmemu,  gmemu,    iic_default,       st4mia,    "store 4-byte and modify immediate after" )
X( st4mib,      base,    gmemu,  gmemu,    iic_default,       st4mib,    "store 4-byte and modify immediate before" )
X( st4mxa,      base,    gmemux, gmemux,   iic_default,       st4mxa,    "store 4-byte and modify indexed after" )
X( st4r,        base,    RaU28,  gmemr,    iic_default,       st4r,      "store 4-byte relative" )
X( st4x,        base,    gmemx,  gmemx,    iic_default,       st4x,      "store 4-byte indexed" )

X( st8,         base,    RabI21, RabI21,   iic_default,       st8,       "store 8-byte" )
X( st8a,        atomic,  aload,  aload,    iic_default,       st8a,      "store 8-byte atomic" )
X( st8mia,      base,    gmemu,  gmemu,    iic_default,       st8mia,    "store 8-byte and modify immediate after" )
X( st8mib,      base,    gmemu,  gmemu,    iic_default,       st8mib,    "store 8-byte and modify immediate before" )
X( st8mxa,      base,    gmemux, gmemux,   iic_default,       st8mxa,    "store 8-byte and modify indexed after" )
X( st8r,        base,    RaU28,  gmemr,    iic_default,       st8r,      "store 8-byte relative" )
X( st8x,        base,    gmemx,  gmemx,    iic_default,       st8x,      "store 8-byte indexed" )

// subtractions
X( sub,         base,    Rabc,   Rabc,     iic_default,       sub,      "subtract" )
X( sub4,        base,    Rabc,   Rabc,     iic_default,       sub4,     "subtract 4-byte" )
X( subb,        base,    Rabc,   Rabc,     iic_default,       subb,     "subtract with borrow" )
X( subfi,       base,    RabI21, RabI21,   iic_default,       subfi,    "subtract from immediate" )
X( subfi4,      base,    RabI21, RabI21,   iic_default,       subfi4,   "subtract from immediate 4-byte" )
X( subo,        base,    Rabc,   Rabc,     iic_default,       subo,     "subtract overflow" )
X( subsub,      base,    Rabcd,  Rabcd,    iic_default,       subsub,   "subtract twice" )
X( subsubb,     base,    Rabcd,  Rabcd,    iic_default,       subsubb,  "subtract & subtract with borrow" )

// kernel call/return
X( syscall,     system,  gmemu,  RabU14,   iic_default,       syscall,  "system call" )
X( sysret,      kernel,  NoArgs, NoArgs,   iic_default,       sysret,   "system return" )

X( tabc,        base,    Rab,    Rab,      iic_default,       tabc,     "test all bits clear" )
X( tabs,        base,    Rab,    Rab,      iic_default,       tabs,     "test all bits set" )

X( tb1,         base,    Rbc,    Rbc,      iic_default,       tb1,      "table branch 1-byte" )
X( tb2,         base,    Rbc,    Rbc,      iic_default,       tb2,      "table branch 2-byte" )
X( tb4,         base,    Rbc,    Rbc,      iic_default,       tb4,      "table branch 4-byte" )

X( teq,         base,    RabI21,    trap,  iic_default,       teq,      "trap if equal" )
X( tlt,         base,    RabI21,    trap,  iic_default,       tlt,      "trap if less than" )
X( tltu,        base,    RabI21,    trap,  iic_default,       tltu,     "trap if less than unsigned" )
X( tne,         base,    RabI21,    trap,  iic_default,       tne,      "trap if not equal" )

X( tpa,         kernel,  Rab,    Rab,      iic_default,       tpa,      "translate to physical address" )

X( undef,       base,    RaxU28, RaxU28,   iic_default,       undef,    "undefined instruction" )

X( vaddc1,      mmx,     Rabc,   Rabc,     iic_default,       vaddc1,   "vector add carry-out 1-byte" )
X( vaddc2,      mmx,     Rabc,   Rabc,     iic_default,       vaddc2,   "vector add carry-out 2-byte" )
X( vaddc4,      mmx,     Rabc,   Rabc,     iic_default,       vaddc4,   "vector add carry-out 4-byte" )
X( vaddc8,      mmx,     Rabc,   Rabc,     iic_default,       vaddc8,   "vector add carry-out 8-byte" )

X( vaddo1,      mmx,     Rabc,   Rabc,     iic_default,       vaddo1,   "vector add overflow 1-byte" )
X( vaddo2,      mmx,     Rabc,   Rabc,     iic_default,       vaddo2,   "vector add overflow 2-byte" )
X( vaddo4,      mmx,     Rabc,   Rabc,     iic_default,       vaddo4,   "vector add overflow 4-byte" )
X( vaddo8,      mmx,     Rabc,   Rabc,     iic_default,       vaddo8,   "vector add overflow 8-byte" )

X( vaddss1,     mmx,     Rabc,   Rabc,     iic_default,       vaddss1,  "vector add signed saturate 1-byte" )
X( vaddss2,     mmx,     Rabc,   Rabc,     iic_default,       vaddss2,  "vector add signed saturate 2-byte" )
X( vaddss4,     mmx,     Rabc,   Rabc,     iic_default,       vaddss4,  "vector add signed saturate 4-byte" )
X( vaddss8,     mmx,     Rabc,   Rabc,     iic_default,       vaddss8,  "vector add signed saturate 8-byte" )

X( vaddu1,      mmx,     Rabc,   Rabc,     iic_default,       vaddu1,   "vector add unsigned 1-byte" )
X( vaddu2,      mmx,     Rabc,   Rabc,     iic_default,       vaddu2,   "vector add unsigned 2-byte" )
X( vaddu4,      mmx,     Rabc,   Rabc,     iic_default,       vaddu4,   "vector add unsigned 4-byte" )
X( vaddu8,      mmx,     Rabc,   Rabc,     iic_default,       vaddu8,   "vector add unsigned 8-byte" )

X( vaddus1,     mmx,     Rabc,   Rabc,     iic_default,       vaddus1,  "vector add unsigned saturate 1-byte" )
X( vaddus2,     mmx,     Rabc,   Rabc,     iic_default,       vaddus2,  "vector add unsigned saturate 2-byte" )
X( vaddus4,     mmx,     Rabc,   Rabc,     iic_default,       vaddus4,  "vector add unsigned saturate 4-byte" )
X( vaddus8,     mmx,     Rabc,   Rabc,     iic_default,       vaddus8,  "vector add unsigned saturate 8-byte" )

X( vavgs1,      mmx,     Rabc,   Rabc,     iic_default,       vavgs1,   "vector average signed 1-byte" )
X( vavgs2,      mmx,     Rabc,   Rabc,     iic_default,       vavgs2,   "vector average signed 2-byte" )
X( vavgs4,      mmx,     Rabc,   Rabc,     iic_default,       vavgs4,   "vector average signed 4-byte" )
X( vavgs8,      mmx,     Rabc,   Rabc,     iic_default,       vavgs8,   "vector average signed 8-byte" )

X( vavgu1,      mmx,     Rabc,   Rabc,     iic_default,       vavgu1,   "vector average unsigned 1-byte" )
X( vavgu2,      mmx,     Rabc,   Rabc,     iic_default,       vavgu2,   "vector average unsigned 2-byte" )
X( vavgu4,      mmx,     Rabc,   Rabc,     iic_default,       vavgu4,   "vector average unsigned 4-byte" )
X( vavgu8,      mmx,     Rabc,   Rabc,     iic_default,       vavgu8,   "vector average unsigned 8-byte" )

X( vcmpeq1,     mmx,     Rabc,   Rabc,     iic_default,       vcmpeq1,  "vector compare equal 1-byte" )
X( vcmpeq2,     mmx,     Rabc,   Rabc,     iic_default,       vcmpeq2,  "vector compare equal 2-byte" )
X( vcmpeq4,     mmx,     Rabc,   Rabc,     iic_default,       vcmpeq4,  "vector compare equal 4-byte" )
X( vcmpeq8,     mmx,     Rabc,   Rabc,     iic_default,       vcmpeq8,  "vector compare equal 8-byte" )

X( vcmplt1,     mmx,     Rabc,   Rabc,     iic_default,       vcmplt1,  "vector compare less than 1-byte" )
X( vcmplt2,     mmx,     Rabc,   Rabc,     iic_default,       vcmplt2,  "vector compare less than 2-byte" )
X( vcmplt4,     mmx,     Rabc,   Rabc,     iic_default,       vcmplt4,  "vector compare less than 4-byte" )
X( vcmplt8,     mmx,     Rabc,   Rabc,     iic_default,       vcmplt8,  "vector compare less than 8-byte" )

X( vcmpltu1,    mmx,     Rabc,   Rabc,     iic_default,       vcmpltu1, "vector compare less than unsigned 1-byte" )
X( vcmpltu2,    mmx,     Rabc,   Rabc,     iic_default,       vcmpltu2, "vector compare less than unsigned 2-byte" )
X( vcmpltu4,    mmx,     Rabc,   Rabc,     iic_default,       vcmpltu4, "vector compare less than unsigned 4-byte" )
X( vcmpltu8,    mmx,     Rabc,   Rabc,     iic_default,       vcmpltu8, "vector compare less than unsigned 8-byte" )

X( vmaxs1,      mmx,     Rabc,   Rabc,     iic_default,       vmaxs1,   "vector maximum signed 1-byte" )
X( vmaxs2,      mmx,     Rabc,   Rabc,     iic_default,       vmaxs2,   "vector maximum signed 2-byte" )
X( vmaxs4,      mmx,     Rabc,   Rabc,     iic_default,       vmaxs4,   "vector maximum signed 4-byte" )
X( vmaxs8,      mmx,     Rabc,   Rabc,     iic_default,       vmaxs8,   "vector maximum signed 8-byte" )

X( vmaxu1,      mmx,     Rabc,   Rabc,     iic_default,       vmaxu1,   "vector maximum unsigned 1-byte" )
X( vmaxu2,      mmx,     Rabc,   Rabc,     iic_default,       vmaxu2,   "vector maximum unsigned 2-byte" )
X( vmaxu4,      mmx,     Rabc,   Rabc,     iic_default,       vmaxu4,   "vector maximum unsigned 4-byte" )
X( vmaxu8,      mmx,     Rabc,   Rabc,     iic_default,       vmaxu8,   "vector maximum unsigned 8-byte" )

X( vmins1,      mmx,     Rabc,   Rabc,     iic_default,       vmins1,   "vector minimum signed 1-byte" )
X( vmins2,      mmx,     Rabc,   Rabc,     iic_default,       vmins2,   "vector minimum signed 2-byte" )
X( vmins4,      mmx,     Rabc,   Rabc,     iic_default,       vmins4,   "vector minimum signed 4-byte" )
X( vmins8,      mmx,     Rabc,   Rabc,     iic_default,       vmins8,   "vector minimum signed 8-byte" )

X( vminu1,      mmx,     Rabc,   Rabc,     iic_default,       vminu1,   "vector minimum unsigned 1-byte" )
X( vminu2,      mmx,     Rabc,   Rabc,     iic_default,       vminu2,   "vector minimum unsigned 2-byte" )
X( vminu4,      mmx,     Rabc,   Rabc,     iic_default,       vminu4,   "vector minimum unsigned 4-byte" )
X( vminu8,      mmx,     Rabc,   Rabc,     iic_default,       vminu8,   "vector minimum unsigned 8-byte" )

X( vmrgh1,      mmx,     Rabc,   Rabc,     iic_default,       vmrgh1,   "vector merge high 1-byte" )
X( vmrgh2,      mmx,     Rabc,   Rabc,     iic_default,       vmrgh2,   "vector merge high 2-byte" )
X( vmrgh4,      mmx,     Rabc,   Rabc,     iic_default,       vmrgh4,   "vector merge high 4-byte" )
X( vmrgh8,      mmx,     Rabc,   Rabc,     iic_default,       vmrgh8,   "vector merge high 8-byte" )

X( vmrgl1,      mmx,     Rabc,   Rabc,     iic_default,       vmrgl1,   "vector merge low 1-byte" )
X( vmrgl2,      mmx,     Rabc,   Rabc,     iic_default,       vmrgl2,   "vector merge low 2-byte" )
X( vmrgl4,      mmx,     Rabc,   Rabc,     iic_default,       vmrgl4,   "vector merge low 4-byte" )
X( vmrgl8,      mmx,     Rabc,   Rabc,     iic_default,       vmrgl8,   "vector merge low 8-byte" )

X( vpksss2,     mmx,     Rabc,   Rabc,     iic_default,       vpksss2,  "vector pack signed as signed saturate 2-byte" )
X( vpksss4,     mmx,     Rabc,   Rabc,     iic_default,       vpksss4,  "vector pack signed as signed saturate 4-byte" )
X( vpksss8,     mmx,     Rabc,   Rabc,     iic_default,       vpksss8,  "vector pack signed as signed saturate 8-byte" )

X( vpksus2,     mmx,     Rabc,   Rabc,     iic_default,       vpksus2,  "vector pack signed as unsigned saturate 2-byte" )
X( vpksus4,     mmx,     Rabc,   Rabc,     iic_default,       vpksus4,  "vector pack signed as unsigned saturate 4-byte" )
X( vpksus8,     mmx,     Rabc,   Rabc,     iic_default,       vpksus8,  "vector pack signed as unsigned saturate 8-byte" )

X( vpkuum2,     mmx,     Rabc,   Rabc,     iic_default,       vpkuum2,  "vector pack unsigned as unsigned modulo 2-byte" )
X( vpkuum4,     mmx,     Rabc,   Rabc,     iic_default,       vpkuum4,  "vector pack unsigned as unsigned modulo 4-byte" )
X( vpkuum8,     mmx,     Rabc,   Rabc,     iic_default,       vpkuum8,  "vector pack unsigned as unsigned modulo 8-byte" )

X( vpkuus2,     mmx,     Rabc,   Rabc,     iic_default,       vpkuus2,  "vector pack unsigned as unsigned saturate 2-byte" )
X( vpkuus4,     mmx,     Rabc,   Rabc,     iic_default,       vpkuus4,  "vector pack unsigned as unsigned saturate 4-byte" )
X( vpkuus8,     mmx,     Rabc,   Rabc,     iic_default,       vpkuus8,  "vector pack unsigned as unsigned saturate 8-byte" )


//vector rotates
X( vrol1,       mmx,     Rabc,   Rabc,     iic_default,       vrol1,    "vector rotate left 1-byte" )
X( vrol2,       mmx,     Rabc,   Rabc,     iic_default,       vrol2,    "vector rotate left 2-byte" )
X( vrol4,       mmx,     Rabc,   Rabc,     iic_default,       vrol4,    "vector rotate left 4-byte" )
X( vrol8,       mmx,     Rabc,   Rabc,     iic_default,       vrol8,    "vector rotate left 8-byte" )

X( vror1,       mmx,     Rabc,   Rabc,     iic_default,       vror1,    "vector rotate right 1-byte" )
X( vror2,       mmx,     Rabc,   Rabc,     iic_default,       vror2,    "vector rotate right 2-byte" )
X( vror4,       mmx,     Rabc,   Rabc,     iic_default,       vror4,    "vector rotate right 4-byte" )
X( vror8,       mmx,     Rabc,   Rabc,     iic_default,       vror8,    "vector rotate right 8-byte" )

X( vsll1,       mmx,     Rabc,   Rabc,     iic_default,       vsll1,    "vector shift left logical 1-byte" )
X( vsll2,       mmx,     Rabc,   Rabc,     iic_default,       vsll2,    "vector shift left logical 2-byte" )
X( vsll4,       mmx,     Rabc,   Rabc,     iic_default,       vsll4,    "vector shift left logical 4-byte" )
X( vsll8,       mmx,     Rabc,   Rabc,     iic_default,       vsll8,    "vector shift left logical 8-byte" )

X( vslli1,      mmx,     RabSc,  FabSc,    iic_default,       vslli1,   "vector shift left logical immediate 1-byte" )
X( vslli2,      mmx,     RabSc,  FabSc,    iic_default,       vslli2,   "vector shift left logical immediate 2-byte" )
X( vslli4,      mmx,     RabSc,  FabSc,    iic_default,       vslli4,   "vector shift left logical immediate 4-byte" )
X( vslli8,      mmx,     RabSc,  FabSc,    iic_default,       vslli8,   "vector shift left logical immediate 8-byte" )

X( vsra1,       mmx,     Rabc,   Rabc,     iic_default,       vsra1,    "vector shift right algebraic 1-byte" )
X( vsra2,       mmx,     Rabc,   Rabc,     iic_default,       vsra2,    "vector shift right algebraic 2-byte" )
X( vsra4,       mmx,     Rabc,   Rabc,     iic_default,       vsra4,    "vector shift right algebraic 4-byte" )
X( vsra8,       mmx,     Rabc,   Rabc,     iic_default,       vsra8,    "vector shift right algebraic 8-byte" )

X( vsrai1,      mmx,     RabSc,  FabSc,    iic_default,       vsrai1,   "vector shift right algebraic immediate 1-byte" )
X( vsrai2,      mmx,     RabSc,  FabSc,    iic_default,       vsrai2,   "vector shift right algebraic immediate 2-byte" )
X( vsrai4,      mmx,     RabSc,  FabSc,    iic_default,       vsrai4,   "vector shift right algebraic immediate 4-byte" )
X( vsrai8,      mmx,     RabSc,  FabSc,    iic_default,       vsrai8,   "vector shift right algebraic immediate 8-byte" )

X( vsrl1,       mmx,     Rabc,   Rabc,     iic_default,       vsrl1,    "vector shift right logical 1-byte" )
X( vsrl2,       mmx,     Rabc,   Rabc,     iic_default,       vsrl2,    "vector shift right logical 2-byte" )
X( vsrl4,       mmx,     Rabc,   Rabc,     iic_default,       vsrl4,    "vector shift right logical 4-byte" )
X( vsrl8,       mmx,     Rabc,   Rabc,     iic_default,       vsrl8,    "vector shift right logical 8-byte" )

X( vsrli1,      mmx,     RabSc,  FabSc,    iic_default,       vsrli1,   "vector shift right logical immediate 1-byte" )
X( vsrli2,      mmx,     RabSc,  FabSc,    iic_default,       vsrli2,   "vector shift right logical immediate 2-byte" )
X( vsrli4,      mmx,     RabSc,  FabSc,    iic_default,       vsrli4,   "vector shift right logical immediate 4-byte" )
X( vsrli8,      mmx,     RabSc,  FabSc,    iic_default,       vsrli8,   "vector shift right logical immediate 8-byte" )

//vector subtract
X( vsubb1,      mmx,     Rabc,   Rabc,     iic_default,       vsubb1,   "vector subtract borrow 1-byte" )
X( vsubb2,      mmx,     Rabc,   Rabc,     iic_default,       vsubb2,   "vector subtract borrow 2-byte" )
X( vsubb4,      mmx,     Rabc,   Rabc,     iic_default,       vsubb4,   "vector subtract borrow 4-byte" )
X( vsubb8,      mmx,     Rabc,   Rabc,     iic_default,       vsubb8,   "vector subtract borrow 8-byte" )

X( vsubo1,      mmx,     Rabc,   Rabc,     iic_default,       vsubo1,   "vector subtract overflow 1-byte" )
X( vsubo2,      mmx,     Rabc,   Rabc,     iic_default,       vsubo2,   "vector subtract overflow 2-byte" )
X( vsubo4,      mmx,     Rabc,   Rabc,     iic_default,       vsubo4,   "vector subtract overflow 4-byte" )
X( vsubo8,      mmx,     Rabc,   Rabc,     iic_default,       vsubo8,   "vector subtract overflow 8-byte" )

X( vsubss1,     mmx,     Rabc,   Rabc,     iic_default,       vsubss1,  "vector subtract signed saturated 1-byte" )
X( vsubss2,     mmx,     Rabc,   Rabc,     iic_default,       vsubss2,  "vector subtract signed saturated 2-byte" )
X( vsubss4,     mmx,     Rabc,   Rabc,     iic_default,       vsubss4,  "vector subtract signed saturated 4-byte" )
X( vsubss8,     mmx,     Rabc,   Rabc,     iic_default,       vsubss8,  "vector subtract signed saturated 8-byte" )

X( vsubu1,      mmx,     Rabc,   Rabc,     iic_default,       vsubu1,   "vector subtract unsigned 1-byte" )
X( vsubu2,      mmx,     Rabc,   Rabc,     iic_default,       vsubu2,   "vector subtract unsigned 2-byte" )
X( vsubu4,      mmx,     Rabc,   Rabc,     iic_default,       vsubu4,   "vector subtract unsigned 4-byte" )
X( vsubu8,      mmx,     Rabc,   Rabc,     iic_default,       vsubu8,   "vector subtract unsigned 8-byte" )

X( vsubus1,     mmx,     Rabc,   Rabc,     iic_default,       vsubus1,  "vector subtract unsigned saturated 1-byte" )
X( vsubus2,     mmx,     Rabc,   Rabc,     iic_default,       vsubus2,  "vector subtract unsigned saturated 2-byte" )
X( vsubus4,     mmx,     Rabc,   Rabc,     iic_default,       vsubus4,  "vector subtract unsigned saturated 4-byte" )
X( vsubus8,     mmx,     Rabc,   Rabc,     iic_default,       vsubus8,  "vector subtract unsigned saturated 8-byte" )

X( vupkhs1,     mmx,     Rab,    Rab,      iic_default,       vupkhs1,  "vector unpack high signed 1-byte" )
X( vupkhs2,     mmx,     Rab,    Rab,      iic_default,       vupkhs2,  "vector unpack high signed 2-byte" )
X( vupkhs4,     mmx,     Rab,    Rab,      iic_default,       vupkhs4,  "vector unpack high signed 4-byte" )

X( vupkls1,     mmx,     Rab,    Rab,      iic_default,       vupkls1,  "vector unpack low signed 1-byte" )
X( vupkls2,     mmx,     Rab,    Rab,      iic_default,       vupkls2,  "vector unpack low signed 2-byte" )
X( vupkls4,     mmx,     Rab,    Rab,      iic_default,       vupkls4,  "vector unpack low signed 4-byte" )

X( write,       base,    RaU28,  write,    iic_default,       ldr,      "write string formatted" )

X( xchg,        base,    Rab,    xchg,     iic_default,       xchg,     "exchange registers" )

X( xnor,        base,    Rabc,   Rabc,     iic_default,       xnor,     "bitwise exclusive nor" )
X( xor,         base,    Rabc,   Rabc,     iic_default,       xor,      "bitwise exclusive or" )
X( xordec,      base,    Rabc,   Rabc,     iic_default,       xordec,   "bitwise exclusive-or decremented" )
X( xori,        base,    RabI21, RabI21,   iic_default,       xori,     "bitwise exclusive or with immediate" )


/**************************************************************************************
* end macrotable
**************************************************************************************/
