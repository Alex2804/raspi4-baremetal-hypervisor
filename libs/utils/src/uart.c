#include "utils/config.h"

#if BUILD_FOR_QEMU
#   define MMIO_BASE       0x3F000000
#else
#   define MMIO_BASE       0xFE000000
#endif

// GPIO

enum {
    PERIPHERAL_BASE = MMIO_BASE,
    GPFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4
};

enum {
    GPIO_MAX_PIN       = 53,
    GPIO_FUNCTION_ALT5 = 2,
};

enum {
    Pull_None = 0,
};

void mmio_write(long reg, unsigned int val) { *(volatile unsigned int *)reg = val; }
unsigned int mmio_read(long reg) { return *(volatile unsigned int *)reg; }

unsigned int gpio_call(unsigned int pin_number, unsigned int value, unsigned int base, unsigned int field_size, unsigned int field_max) {
    unsigned int field_mask = (1 << field_size) - 1;

    if (pin_number > field_max) return 0;
    if (value > field_mask) return 0;

    unsigned int num_fields = 32 / field_size;
    unsigned int reg = base + ((pin_number / num_fields) * 4);
    unsigned int shift = (pin_number % num_fields) * field_size;

    unsigned int curval = mmio_read(reg);
    curval &= ~(field_mask << shift);
    curval |= value << shift;
    mmio_write(reg, curval);

    return 1;
}

unsigned int gpio_set     (unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN); }
unsigned int gpio_clear   (unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN); }
unsigned int gpio_pull    (unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN); }
unsigned int gpio_function(unsigned int pin_number, unsigned int value) { return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN); }

void gpio_use_as_alt_5(unsigned int pin_number) {
    gpio_pull(pin_number, Pull_None);
    gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}

// UART

enum {
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK  = 500000000,
    UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)


unsigned int uart_is_write_byte_ready() {
    return mmio_read(AUX_MU_LSR_REG) & 0x20;
}
void uart_write_byte_blocking(unsigned char c) {
    while(!uart_is_write_byte_ready()) {}
    mmio_write(AUX_MU_IO_REG, c);
}


void uart_init() {
    mmio_write(AUX_ENABLES, 1); //enable UART1
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); //8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_IIR_REG, 0xC6); //disable interrupts
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
    gpio_use_as_alt_5(14);
    gpio_use_as_alt_5(15);
    mmio_write(AUX_MU_CNTL_REG, 3); //enable RX/TX
}


char uart_try_read_char() {
    if(mmio_read(AUX_MU_LSR_REG) & 0x01) {  // check if char in buffer
        char r = mmio_read(AUX_MU_IO_REG);  // read char in buffer
        return r == '\r' ? '\n' : r;
    }
    return '\0';  // else return null terminator
}
char uart_read_char() {
    do {
        asm volatile("nop");
    } while(!(mmio_read(AUX_MU_LSR_REG) & 0x01));
    char r = mmio_read(AUX_MU_IO_REG);  // read char in buffer
    return r == '\r' ? '\n' : r;
}


void uart_write_newline() {
    uart_write_byte_blocking('\r');
    uart_write_byte_blocking('\n');
}
void uart_write_byte(unsigned char c) {
    uart_write_byte_blocking(c);
}
void uart_write_char(char c) {
    uart_write_byte_blocking(c);
}
void uart_write_string(const char *buffer) {
    while(*buffer) {
        if(*buffer == '\n') {
            uart_write_byte_blocking('\r');
        }
        uart_write_byte_blocking(*buffer++);
    }
}
void uart_write_hex(unsigned long long h) {
    uart_write_byte_blocking('0');
    uart_write_byte_blocking('x');
    int flag = 0;
    for(int c = 60; c >= 0; c -= 4) {
        unsigned long long n = (h>>c) & 0xF; // get highest tetrad
        if(n != 0 || flag != 0 || c == 0) {
            flag = 1;
            n += n > 9 ? 0x37 : 0x30; // 0-9 => '0'-'9', 10-15 => 'A'-'F'
            uart_write_byte_blocking(n);
        }
    }
}
void uart_write_long(long long d) {
    if(d < 0) {
        uart_write_byte_blocking('-');
        d = -d;
    }

    long flag = 0;
    for(long long c = 1000000000000000000; c >= 1; c /= 10) {
        // get highest digit
        long long n = (d/c) % 10;
        if(n != 0 || flag != 0 || c == 1){
            flag = 1;
            // 0-9 => '0'-'9'
            n += 0x30;
            uart_write_byte_blocking(n);
        }
    }
}


void uart_dump(void *ptr)
{
    unsigned long a, b, d;
    unsigned char c;
    for(a = (unsigned long)ptr; a < (unsigned long)ptr+512; a += 16) {
        uart_write_hex(a);
        uart_write_byte_blocking(':');
        uart_write_byte_blocking(' ');
        for(b = 0; b < 16; ++b) {
            c = *(unsigned char*)(a+b);
            d = (unsigned int)c;
            d >>= 4;
            d &= 0xF;
            d += d > 9 ? 0x37 : 0x30;
            uart_write_byte_blocking(d);
            d = (unsigned int)c;
            d &= 0xF;
            d += d > 9 ? 0x37 : 0x30;
            uart_write_byte_blocking(d);
            uart_write_byte_blocking(' ');
            if(b % 4 == 3)
                uart_write_byte_blocking(' ');
        }
        for(b=0;b<16;b++) {
            c = *(unsigned char*)(a+b);
            uart_write_byte_blocking(c < 32 || c >= 127 ? '.' : c);
        }
        uart_write_byte_blocking('\r');
        uart_write_byte_blocking('\n');
    }
}
