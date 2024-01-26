#ifndef REGISTERS_H
#define REGISTERS_H

#define __PRINT_REGISTER_GENERAL(mov_cmd, register_name, output_name) \
    { \
        unsigned long long _register_value; \
        asm volatile(mov_cmd " %0, " #register_name : "=r" (_register_value)); \
        uart_puts(output_name ": "); \
        uart_full_hex(_register_value); \
        uart_send('\n'); \
    }

#define __PRINT_REGISTER_WITH_NAME(register_name, output_name) __PRINT_REGISTER_GENERAL("mov", register_name, output_name)
#define __PRINT_REGISTER(register_name) __PRINT_REGISTER_WITH_NAME(register_name, #register_name)

#define __PRINT_SYSTEM_REGISTER(register_name) __PRINT_REGISTER_GENERAL("mrs", register_name, #register_name)


#define __PRINT_VECTOR_REGISTER(register_name) \
    { \
        unsigned long long _lower_half, _upper_half; \
        asm volatile("mov %0, " #register_name ".2d[0]" : "=r" (_lower_half)); \
        asm volatile("mov %0, " #register_name ".2d[1]" : "=r" (_upper_half)); \
        uart_puts(#register_name ": "); \
        uart_full_hex(_upper_half); \
        uart_full_hex_without_prefix(_lower_half); \
        uart_send('\n'); \
    }

#define PRINT_GENERAL_PURPOSE_REGISTERS() \
    __PRINT_REGISTER(x0); \
    __PRINT_REGISTER(x1); \
    __PRINT_REGISTER(x2); \
    __PRINT_REGISTER(x3); \
    __PRINT_REGISTER(x4); \
    __PRINT_REGISTER(x5); \
    __PRINT_REGISTER(x6); \
    __PRINT_REGISTER(x7); \
    __PRINT_REGISTER(x8); \
    __PRINT_REGISTER(x9); \
    __PRINT_REGISTER(x10); \
    __PRINT_REGISTER(x11); \
    __PRINT_REGISTER(x12); \
    __PRINT_REGISTER(x13); \
    __PRINT_REGISTER(x14); \
    __PRINT_REGISTER(x15); \
    __PRINT_REGISTER(x16); \
    __PRINT_REGISTER(x17); \
    __PRINT_REGISTER(x18); \
    __PRINT_REGISTER(x19); \
    __PRINT_REGISTER(x20); \
    __PRINT_REGISTER(x21); \
    __PRINT_REGISTER(x22); \
    __PRINT_REGISTER(x23); \
    __PRINT_REGISTER(x24); \
    __PRINT_REGISTER(x25); \
    __PRINT_REGISTER(x26); \
    __PRINT_REGISTER(x27); \
    __PRINT_REGISTER(x28); \
    __PRINT_REGISTER_WITH_NAME(x29, "fp"); \
    __PRINT_REGISTER_WITH_NAME(x30, "lr"); \
    __PRINT_REGISTER(sp);


#define PRINT_VECTOR_PROCESSING_REGISTERS() \
    __PRINT_VECTOR_REGISTER(v0); \
    __PRINT_VECTOR_REGISTER(v1); \
    __PRINT_VECTOR_REGISTER(v2); \
    __PRINT_VECTOR_REGISTER(v3); \
    __PRINT_VECTOR_REGISTER(v4); \
    __PRINT_VECTOR_REGISTER(v5); \
    __PRINT_VECTOR_REGISTER(v6); \
    __PRINT_VECTOR_REGISTER(v7); \
    __PRINT_VECTOR_REGISTER(v8); \
    __PRINT_VECTOR_REGISTER(v9); \
    __PRINT_VECTOR_REGISTER(v10); \
    __PRINT_VECTOR_REGISTER(v11); \
    __PRINT_VECTOR_REGISTER(v12); \
    __PRINT_VECTOR_REGISTER(v13); \
    __PRINT_VECTOR_REGISTER(v14); \
    __PRINT_VECTOR_REGISTER(v15); \
    __PRINT_VECTOR_REGISTER(v16); \
    __PRINT_VECTOR_REGISTER(v17); \
    __PRINT_VECTOR_REGISTER(v18); \
    __PRINT_VECTOR_REGISTER(v19); \
    __PRINT_VECTOR_REGISTER(v20); \
    __PRINT_VECTOR_REGISTER(v21); \
    __PRINT_VECTOR_REGISTER(v22); \
    __PRINT_VECTOR_REGISTER(v23); \
    __PRINT_VECTOR_REGISTER(v24); \
    __PRINT_VECTOR_REGISTER(v25); \
    __PRINT_VECTOR_REGISTER(v26); \
    __PRINT_VECTOR_REGISTER(v27); \
    __PRINT_VECTOR_REGISTER(v28); \
    __PRINT_VECTOR_REGISTER(v29); \
    __PRINT_VECTOR_REGISTER(v30); \
    __PRINT_VECTOR_REGISTER(v31);


#define PRINT_MEMORY_MAPPED_REGISTERS()

#define PRINT_EL1_REGISTERS() \
    __PRINT_SYSTEM_REGISTER(spsr_el1); \
    __PRINT_SYSTEM_REGISTER(elr_el1); \
    __PRINT_SYSTEM_REGISTER(sctlr_el1); \
    __PRINT_SYSTEM_REGISTER(vbar_el1); \
    __PRINT_SYSTEM_REGISTER(cpacr_el1);

#define PRINT_EL2_REGISTERS() \
    __PRINT_SYSTEM_REGISTER(spsr_el2); \
    __PRINT_SYSTEM_REGISTER(elr_el2); \
    __PRINT_SYSTEM_REGISTER(sctlr_el2); \
    __PRINT_SYSTEM_REGISTER(hcr_el2); \
    __PRINT_SYSTEM_REGISTER(vbar_el2);


#define PRINT_ALL_REGISTERS(current_el) \
    PRINT_GENERAL_PURPOSE_REGISTERS(); \
    PRINT_MEMORY_MAPPED_REGISTERS(); \
    PRINT_VECTOR_PROCESSING_REGISTERS(); \
    if(current_el == 1) { \
        PRINT_EL1_REGISTERS(); \
    } else if(current_el == 2) { \
        PRINT_EL2_REGISTERS(); \
    }


#endif //REGISTERS_H
