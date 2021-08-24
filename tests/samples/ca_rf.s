.text
    alloc	96
    write	"test ca.rf"
    ca.rf	%r22, ca_rf_data
    write	"ca.rf: %x64(r22)"

    write	"end_ca_rf_test"
.data
ca_rf_data:

.end
