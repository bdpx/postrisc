.text
    alloc 96
    write "test ca.rf"
    ldard %r22, ldarcf_data
    write "ca.rf: %x64(r22)"

    write "end_ca_rf_test"
.data
ldarcf_data:

.end
