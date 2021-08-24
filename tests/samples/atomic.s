.text
    alloc 96
    write "test atomic fetch-op"
    addi %r5, %sp, -64
    write "atomic base: %x64(r5)"
    ldi  %r10, 5
    ldi  %r12, 10
    ldi  %r56, 5

    write "test amo-add"

    ldaddb %r4, %r5, %r10
    ldaddba %r4, %r5, %r10
    ldaddbr %r4, %r5, %r10
    ldaddbar %r4, %r5, %r10

    ldaddh %r4, %r5, %r10
    ldaddha %r4, %r5, %r10
    ldaddhr %r4, %r5, %r10
    ldaddhar %r4, %r5, %r10

    ldaddw %r4, %r5, %r10
    ldaddwa %r4, %r5, %r10
    ldaddwr %r4, %r5, %r10
    ldaddwar %r4, %r5, %r10

    ldaddd %r4, %r5, %r10
    ldaddda %r4, %r5, %r10
    ldadddr %r4, %r5, %r10
    ldadddar %r4, %r5, %r10

    ldaddq %r4, %r5, %r10
    ldaddqa %r4, %r5, %r10
    ldaddqr %r4, %r5, %r10
    ldaddqar %r4, %r5, %r10

    write "test amo-and"

    ldandb %r4, %r5, %r10
    ldandba %r4, %r5, %r10
    ldandbr %r4, %r5, %r10
    ldandbar %r4, %r5, %r10

    ldandh %r4, %r5, %r10
    ldandha %r4, %r5, %r10
    ldandhr %r4, %r5, %r10
    ldandhar %r4, %r5, %r10

    ldandw %r4, %r5, %r10
    ldandwa %r4, %r5, %r10
    ldandwr %r4, %r5, %r10
    ldandwar %r4, %r5, %r10

    ldandd %r4, %r5, %r10
    ldandda %r4, %r5, %r10
    ldanddr %r4, %r5, %r10
    ldanddar %r4, %r5, %r10

    ldandq %r4, %r5, %r10
    ldandqa %r4, %r5, %r10
    ldandqr %r4, %r5, %r10
    ldandqar %r4, %r5, %r10

    write "test amo-or"

    ldorb %r4, %r5, %r10
    ldorba %r4, %r5, %r10
    ldorbr %r4, %r5, %r10
    ldorbar %r4, %r5, %r10

    ldorh %r4, %r5, %r10
    ldorha %r4, %r5, %r10
    ldorhr %r4, %r5, %r10
    ldorhar %r4, %r5, %r10

    ldorw %r4, %r5, %r10
    ldorwa %r4, %r5, %r10
    ldorwr %r4, %r5, %r10
    ldorwar %r4, %r5, %r10

    ldord %r4, %r5, %r10
    ldorda %r4, %r5, %r10
    ldordr %r4, %r5, %r10
    ldordar %r4, %r5, %r10

    ldorq %r4, %r5, %r10
    ldorqa %r4, %r5, %r10
    ldorqr %r4, %r5, %r10
    ldorqar %r4, %r5, %r10

    write "test amo-xor"

    ldxorb %r4, %r5, %r10
    ldxorba %r4, %r5, %r10
    ldxorbr %r4, %r5, %r10
    ldxorbar %r4, %r5, %r10

    ldxorh %r4, %r5, %r10
    ldxorha %r4, %r5, %r10
    ldxorhr %r4, %r5, %r10
    ldxorhar %r4, %r5, %r10

    ldxorw %r4, %r5, %r10
    ldxorwa %r4, %r5, %r10
    ldxorwr %r4, %r5, %r10
    ldxorwar %r4, %r5, %r10

    ldxord %r4, %r5, %r10
    ldxorda %r4, %r5, %r10
    ldxordr %r4, %r5, %r10
    ldxordar %r4, %r5, %r10

    ldxorq %r4, %r5, %r10
    ldxorqa %r4, %r5, %r10
    ldxorqr %r4, %r5, %r10
    ldxorqar %r4, %r5, %r10

    write "test amo-smin"
    ldsminb %r4, %r5, %r10
    ldsminba %r4, %r5, %r10
    ldsminbr %r4, %r5, %r10
    ldsminbar %r4, %r5, %r10

    ldsminh %r4, %r5, %r10
    ldsminha %r4, %r5, %r10
    ldsminhr %r4, %r5, %r10
    ldsminhar %r4, %r5, %r10

    ldsminw %r4, %r5, %r10
    ldsminwa %r4, %r5, %r10
    ldsminwr %r4, %r5, %r10
    ldsminwar %r4, %r5, %r10

    ldsmind %r4, %r5, %r10
    ldsminda %r4, %r5, %r10
    ldsmindr %r4, %r5, %r10
    ldsmindar %r4, %r5, %r10

    ldsminq %r4, %r5, %r10
    ldsminqa %r4, %r5, %r10
    ldsminqr %r4, %r5, %r10
    ldsminqar %r4, %r5, %r10

    write "test amo-smax"
    ldsmaxb %r4, %r5, %r10
    ldsmaxba %r4, %r5, %r10
    ldsmaxbr %r4, %r5, %r10
    ldsmaxbar %r4, %r5, %r10

    ldsmaxh %r4, %r5, %r10
    ldsmaxha %r4, %r5, %r10
    ldsmaxhr %r4, %r5, %r10
    ldsmaxhar %r4, %r5, %r10

    ldsmaxw %r4, %r5, %r10
    ldsmaxwa %r4, %r5, %r10
    ldsmaxwr %r4, %r5, %r10
    ldsmaxwar %r4, %r5, %r10

    ldsmaxd %r4, %r5, %r10
    ldsmaxda %r4, %r5, %r10
    ldsmaxdr %r4, %r5, %r10
    ldsmaxdar %r4, %r5, %r10

    ldsmaxq %r4, %r5, %r10
    ldsmaxqa %r4, %r5, %r10
    ldsmaxqr %r4, %r5, %r10
    ldsmaxqar %r4, %r5, %r10

    write "test amo-umin"
    lduminb %r4, %r5, %r10
    lduminba %r4, %r5, %r10
    lduminbr %r4, %r5, %r10
    lduminbar %r4, %r5, %r10

    lduminh %r4, %r5, %r10
    lduminha %r4, %r5, %r10
    lduminhr %r4, %r5, %r10
    lduminhar %r4, %r5, %r10

    lduminw %r4, %r5, %r10
    lduminwa %r4, %r5, %r10
    lduminwr %r4, %r5, %r10
    lduminwar %r4, %r5, %r10

    ldumind %r4, %r5, %r10
    lduminda %r4, %r5, %r10
    ldumindr %r4, %r5, %r10
    ldumindar %r4, %r5, %r10

    lduminq %r4, %r5, %r10
    lduminqa %r4, %r5, %r10
    lduminqr %r4, %r5, %r10
    lduminqar %r4, %r5, %r10

    write "test amo-umax"
    ldumaxb %r4, %r5, %r10
    ldumaxba %r4, %r5, %r10
    ldumaxbr %r4, %r5, %r10
    ldumaxbar %r4, %r5, %r10

    ldumaxh %r4, %r5, %r10
    ldumaxha %r4, %r5, %r10
    ldumaxhr %r4, %r5, %r10
    ldumaxhar %r4, %r5, %r10

    ldumaxw %r4, %r5, %r10
    ldumaxwa %r4, %r5, %r10
    ldumaxwr %r4, %r5, %r10
    ldumaxwar %r4, %r5, %r10

    ldumaxd %r4, %r5, %r10
    ldumaxda %r4, %r5, %r10
    ldumaxdr %r4, %r5, %r10
    ldumaxdar %r4, %r5, %r10

    ldumaxq %r4, %r5, %r10
    ldumaxqa %r4, %r5, %r10
    ldumaxqr %r4, %r5, %r10
    ldumaxqar %r4, %r5, %r10

    write "test cas"

    casb %r12, %r5, %r56
    casba %r12, %r5, %r56
    casbr %r12, %r5, %r56
    casbar %r12, %r5, %r56

    cash %r12, %r5, %r56
    casha %r12, %r5, %r56
    cashr %r12, %r5, %r56
    cashar %r12, %r5, %r56

    casw %r12, %r5, %r56
    caswa %r12, %r5, %r56
    caswr %r12, %r5, %r56
    caswar %r12, %r5, %r56

    casd %r12, %r5, %r56
    casda %r12, %r5, %r56
    casdr %r12, %r5, %r56
    casdar %r12, %r5, %r56

    casq %r12, %r5, %r56
    casqa %r12, %r5, %r56
    casqr %r12, %r5, %r56
    casqar %r12, %r5, %r56

    write "test load atomic relaxed"
    ldab %r12, %r5
    ldah %r12, %r5
    ldaw %r12, %r5
    ldad %r12, %r5
    ldaq %r12, %r5

    write "test load atomic acquire"
    ldaba %r12, %r5
    ldaha %r12, %r5
    ldawa %r12, %r5
    ldada %r12, %r5
    ldaqa %r12, %r5

    write "test store atomic relaxed"
    stab %r12, %r5
    stah %r12, %r5
    staw %r12, %r5
    stad %r12, %r5
    staq %r12, %r5

    write "test store atomic release"
    stabr %r12, %r5
    stahr %r12, %r5
    stawr %r12, %r5
    stadr %r12, %r5
    staqr %r12, %r5

.end
