.text

.global get_core_id
get_core_id:
    mrs     x0, mpidr_el1
    and     x0, x0, 0x3
    ret