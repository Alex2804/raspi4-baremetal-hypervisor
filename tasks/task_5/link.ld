CORE_COUNT = 4;

SECTIONS
{
    . = 0x80000;
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    .spin_core : {
        . = ALIGN(8);
        __spin_core_0 = .;
        . = . + CORE_COUNT * 8;
    }
    .stack : {
            . = ALIGN(16);  /* Stack must always be aligned to 8 byte boundary AAPCS64 call standard */
            __stack_start__ = .;
            . = . + CORE_COUNT * 1024;  /* EL0 stack size (for 4 cores) */
            __EL0_stack__ = .;
            . = . + CORE_COUNT * 4096;  /* EL1 stack size (for 4 cores) */
            __EL1_stack__ = .;
            . = . + CORE_COUNT * 4096;  /* EL2 stack size (for 4 cores) */
            __EL2_stack__ = .;
            . = ALIGN(8);
            __stack_end__ = .;
    }

    _end = .;

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;
