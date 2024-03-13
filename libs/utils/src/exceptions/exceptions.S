.text

.globl get_el
get_el:
    mrs     x0, CurrentEL // read CurrentEL into x0
    lsr     x0, x0, #2 // shift right by 2 to get EL
    ret
