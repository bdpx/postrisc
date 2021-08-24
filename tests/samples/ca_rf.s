.text
    alloc 96
    write "test ca.rf"
    lda_iprel %r22, lda_rf_data
    write "ca.rf: %x64(r22)"

    write "end_ca_rf_test"
.data
lda_rf_data:

.end
