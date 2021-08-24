.text
  alloc 96
  write "test atomic fetch-op"
  addid  %r5, %sp, -64
  write "atomic base: %x64(r5)"
  ldi %r10, 5
  ldi %r12, 10
  ldi %r56, 5

  write "test amo-add"

  amoaddb %r4, %r5, %r10, relaxed
  amoaddb %r4, %r5, %r10, acquire
  amoaddb %r4, %r5, %r10, release
  amoaddb %r4, %r5, %r10, acq_rel

  amoaddh %r4, %r5, %r10, relaxed
  amoaddh %r4, %r5, %r10, acquire
  amoaddh %r4, %r5, %r10, release
  amoaddh %r4, %r5, %r10, acq_rel

  amoaddw %r4, %r5, %r10, relaxed
  amoaddw %r4, %r5, %r10, acquire
  amoaddw %r4, %r5, %r10, release
  amoaddw %r4, %r5, %r10, acq_rel

  amoaddd %r4, %r5, %r10, relaxed
  amoaddd %r4, %r5, %r10, acquire
  amoaddd %r4, %r5, %r10, release
  amoaddd %r4, %r5, %r10, acq_rel

  amoaddq %r4, %r5, %r10, relaxed
  amoaddq %r4, %r5, %r10, acquire
  amoaddq %r4, %r5, %r10, release
  amoaddq %r4, %r5, %r10, acq_rel

  write "test amo-sub"

  amosubb %r4, %r5, %r10, relaxed
  amosubb %r4, %r5, %r10, acquire
  amosubb %r4, %r5, %r10, release
  amosubb %r4, %r5, %r10, acq_rel

  amosubh %r4, %r5, %r10, relaxed
  amosubh %r4, %r5, %r10, acquire
  amosubh %r4, %r5, %r10, release
  amosubh %r4, %r5, %r10, acq_rel

  amosubw %r4, %r5, %r10, relaxed
  amosubw %r4, %r5, %r10, acquire
  amosubw %r4, %r5, %r10, release
  amosubw %r4, %r5, %r10, acq_rel

  amosubd %r4, %r5, %r10, relaxed
  amosubd %r4, %r5, %r10, acquire
  amosubd %r4, %r5, %r10, release
  amosubd %r4, %r5, %r10, acq_rel

  amosubq %r4, %r5, %r10, relaxed
  amosubq %r4, %r5, %r10, acquire
  amosubq %r4, %r5, %r10, release
  amosubq %r4, %r5, %r10, acq_rel

  write "test amo-and"

  amoandb %r4, %r5, %r10, relaxed
  amoandb %r4, %r5, %r10, acquire
  amoandb %r4, %r5, %r10, release
  amoandb %r4, %r5, %r10, acq_rel

  amoandh %r4, %r5, %r10, relaxed
  amoandh %r4, %r5, %r10, acquire
  amoandh %r4, %r5, %r10, release
  amoandh %r4, %r5, %r10, acq_rel

  amoandw %r4, %r5, %r10, relaxed
  amoandw %r4, %r5, %r10, acquire
  amoandw %r4, %r5, %r10, release
  amoandw %r4, %r5, %r10, acq_rel

  amoandd %r4, %r5, %r10, relaxed
  amoandd %r4, %r5, %r10, acquire
  amoandd %r4, %r5, %r10, release
  amoandd %r4, %r5, %r10, acq_rel

  amoandq %r4, %r5, %r10, relaxed
  amoandq %r4, %r5, %r10, acquire
  amoandq %r4, %r5, %r10, release
  amoandq %r4, %r5, %r10, acq_rel

  write "test amo-or"

  amoorb %r4, %r5, %r10, relaxed
  amoorb %r4, %r5, %r10, acquire
  amoorb %r4, %r5, %r10, release
  amoorb %r4, %r5, %r10, acq_rel

  amoorh %r4, %r5, %r10, relaxed
  amoorh %r4, %r5, %r10, acquire
  amoorh %r4, %r5, %r10, release
  amoorh %r4, %r5, %r10, acq_rel

  amoorw %r4, %r5, %r10, relaxed
  amoorw %r4, %r5, %r10, acquire
  amoorw %r4, %r5, %r10, release
  amoorw %r4, %r5, %r10, acq_rel

  amoord %r4, %r5, %r10, relaxed
  amoord %r4, %r5, %r10, acquire
  amoord %r4, %r5, %r10, release
  amoord %r4, %r5, %r10, acq_rel

  amoorq %r4, %r5, %r10, relaxed
  amoorq %r4, %r5, %r10, acquire
  amoorq %r4, %r5, %r10, release
  amoorq %r4, %r5, %r10, acq_rel

  write "test amo-xor"

  amoxorb %r4, %r5, %r10, relaxed
  amoxorb %r4, %r5, %r10, acquire
  amoxorb %r4, %r5, %r10, release
  amoxorb %r4, %r5, %r10, acq_rel

  amoxorh %r4, %r5, %r10, relaxed
  amoxorh %r4, %r5, %r10, acquire
  amoxorh %r4, %r5, %r10, release
  amoxorh %r4, %r5, %r10, acq_rel

  amoxorw %r4, %r5, %r10, relaxed
  amoxorw %r4, %r5, %r10, acquire
  amoxorw %r4, %r5, %r10, release
  amoxorw %r4, %r5, %r10, acq_rel

  amoxord %r4, %r5, %r10, relaxed
  amoxord %r4, %r5, %r10, acquire
  amoxord %r4, %r5, %r10, release
  amoxord %r4, %r5, %r10, acq_rel

  amoxorq %r4, %r5, %r10, relaxed
  amoxorq %r4, %r5, %r10, acquire
  amoxorq %r4, %r5, %r10, release
  amoxorq %r4, %r5, %r10, acq_rel

  write "test amo-smin"
  amominsb   %r4, %r5, %r10, relaxed
  amominsb   %r4, %r5, %r10, acquire
  amominsb   %r4, %r5, %r10, release
  amominsb   %r4, %r5, %r10, acq_rel

  amominsh  %r4, %r5, %r10, relaxed
  amominsh  %r4, %r5, %r10, acquire
  amominsh  %r4, %r5, %r10, release
  amominsh  %r4, %r5, %r10, acq_rel

  amominsw  %r4, %r5, %r10, relaxed
  amominsw  %r4, %r5, %r10, acquire
  amominsw  %r4, %r5, %r10, release
  amominsw  %r4, %r5, %r10, acq_rel

  amominsd  %r4, %r5, %r10, relaxed
  amominsd  %r4, %r5, %r10, acquire
  amominsd  %r4, %r5, %r10, release
  amominsd  %r4, %r5, %r10, acq_rel

  amominsq  %r4, %r5, %r10, relaxed
  amominsq  %r4, %r5, %r10, acquire
  amominsq  %r4, %r5, %r10, release
  amominsq  %r4, %r5, %r10, acq_rel

  write "test amo-smax"
  amomaxsb  %r4, %r5, %r10, relaxed
  amomaxsb  %r4, %r5, %r10, acquire
  amomaxsb  %r4, %r5, %r10, release
  amomaxsb  %r4, %r5, %r10, acq_rel

  amomaxsh  %r4, %r5, %r10, relaxed
  amomaxsh  %r4, %r5, %r10, acquire
  amomaxsh  %r4, %r5, %r10, release
  amomaxsh  %r4, %r5, %r10, acq_rel

  amomaxsw  %r4, %r5, %r10, relaxed
  amomaxsw  %r4, %r5, %r10, acquire
  amomaxsw  %r4, %r5, %r10, release
  amomaxsw  %r4, %r5, %r10, acq_rel

  amomaxsd  %r4, %r5, %r10, relaxed
  amomaxsd  %r4, %r5, %r10, acquire
  amomaxsd  %r4, %r5, %r10, release
  amomaxsd  %r4, %r5, %r10, acq_rel

  amomaxsq  %r4, %r5, %r10, relaxed
  amomaxsq  %r4, %r5, %r10, acquire
  amomaxsq  %r4, %r5, %r10, release
  amomaxsq  %r4, %r5, %r10, acq_rel

  write "test amo-umin"
  amominub  %r4, %r5, %r10, relaxed
  amominub  %r4, %r5, %r10, acquire
  amominub  %r4, %r5, %r10, release
  amominub  %r4, %r5, %r10, acq_rel

  amominuh  %r4, %r5, %r10, relaxed
  amominuh  %r4, %r5, %r10, acquire
  amominuh  %r4, %r5, %r10, release
  amominuh  %r4, %r5, %r10, acq_rel

  amominuw  %r4, %r5, %r10, relaxed
  amominuw  %r4, %r5, %r10, acquire
  amominuw  %r4, %r5, %r10, release
  amominuw  %r4, %r5, %r10, acq_rel

  amominud  %r4, %r5, %r10, relaxed
  amominud  %r4, %r5, %r10, acquire
  amominud  %r4, %r5, %r10, release
  amominud  %r4, %r5, %r10, acq_rel

  amominuq  %r4, %r5, %r10, relaxed
  amominuq  %r4, %r5, %r10, acquire
  amominuq  %r4, %r5, %r10, release
  amominuq  %r4, %r5, %r10, acq_rel

  write "test amo-umax"
  amomaxub  %r4, %r5, %r10, relaxed
  amomaxub  %r4, %r5, %r10, acquire
  amomaxub  %r4, %r5, %r10, release
  amomaxub  %r4, %r5, %r10, acq_rel

  amomaxuh  %r4, %r5, %r10, relaxed
  amomaxuh  %r4, %r5, %r10, acquire
  amomaxuh  %r4, %r5, %r10, release
  amomaxuh  %r4, %r5, %r10, acq_rel

  amomaxuw  %r4, %r5, %r10, relaxed
  amomaxuw  %r4, %r5, %r10, acquire
  amomaxuw  %r4, %r5, %r10, release
  amomaxuw  %r4, %r5, %r10, acq_rel

  amomaxud  %r4, %r5, %r10, relaxed
  amomaxud  %r4, %r5, %r10, acquire
  amomaxud  %r4, %r5, %r10, release
  amomaxud  %r4, %r5, %r10, acq_rel

  amomaxuq %r4, %r5, %r10, relaxed
  amomaxuq %r4, %r5, %r10, acquire
  amomaxuq %r4, %r5, %r10, release
  amomaxuq %r4, %r5, %r10, acq_rel

  write "test cas"

  amocasb  %r12, %r5, %r56, %r34, relaxed
  amocasb  %r12, %r5, %r56, %r34, acquire
  amocasb  %r12, %r5, %r56, %r34, release
  amocasb  %r12, %r5, %r56, %r34, acq_rel

  amocash %r12, %r5, %r56, %r34, relaxed
  amocash %r12, %r5, %r56, %r34, acquire
  amocash %r12, %r5, %r56, %r34, release
  amocash %r12, %r5, %r56, %r34, acq_rel

  amocasw %r12, %r5, %r56, %r34, relaxed
  amocasw %r12, %r5, %r56, %r34, acquire
  amocasw %r12, %r5, %r56, %r34, release
  amocasw %r12, %r5, %r56, %r34, acq_rel

  amocasd %r12, %r5, %r56, %r34, relaxed
  amocasd %r12, %r5, %r56, %r34, acquire
  amocasd %r12, %r5, %r56, %r34, release
  amocasd %r12, %r5, %r56, %r34, acq_rel

  amocasq %r12, %r5, %r56, %r34, relaxed
  amocasq %r12, %r5, %r56, %r34, acquire
  amocasq %r12, %r5, %r56, %r34, release
  amocasq %r12, %r5, %r56, %r34, acq_rel

  write "test load atomic relaxed"
  amoldb  %r12, %r5, relaxed
  amoldh  %r12, %r5, relaxed
  amoldw  %r12, %r5, relaxed
  amoldd  %r12, %r5, relaxed
  amoldq  %r12, %r5, relaxed

  write "test load atomic acquire"
  amoldb  %r12, %r5, acquire
  amoldh  %r12, %r5, acquire
  amoldw  %r12, %r5, acquire
  amoldd  %r12, %r5, acquire
  amoldq  %r12, %r5, acquire

  write "test store atomic relaxed"
  amostb  %r12, %r5, relaxed
  amosth  %r12, %r5, relaxed
  amostw  %r12, %r5, relaxed
  amostd  %r12, %r5, relaxed
  amostq  %r12, %r5, relaxed

  write "test store atomic release"
  amostb  %r12, %r5, release
  amosth  %r12, %r5, release
  amostw  %r12, %r5, release
  amostd  %r12, %r5, release
  amostq  %r12, %r5, release

.end
