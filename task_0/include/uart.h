#ifndef RASPI4_BAREMETAL_HYPERVISOR_UART_H
#define RASPI4_BAREMETAL_HYPERVISOR_UART_H

void uart_init();

char uart_try_read_char();
char uart_read_char();

void uart_write_newline();
void uart_write_byte(unsigned char c);
void uart_write_char(char c);
void uart_write_string(char *s);
void uart_write_hex(unsigned long long h);
void uart_write_long(long long d);

void uart_dump(void *ptr);

#endif  // RASPI4_BAREMETAL_HYPERVISOR_UART_H