#include "utils/string.h"

int string_to_int(const char* string) {
    int value = 0;
    int sign = 1;
    if(*string == '-') {
        sign = -1;
        ++string;
    }
    for(char c = *string; c != '\0'; c = *++string) {
        if(c < '0' || c > '9') {
            return 0;
        }
        value *= 10;
        value += c - '0';
    }
    value *= sign;
    return value;
}

unsigned long long hex_to_ull(const char* string) {
    if(*string == '0' && (*(string + 1) == 'x' || *(string + 1) == 'X')) {
        string += 2;
    }
    unsigned long long value = 0;
    for(char c = *string; c != '\0'; c = *++string) {
        value *= 16;
        if(c >= '0' && c <= '9') {
            value += c - '0';
        } else if(c >= 'a' && c <= 'f') {
            value += c - 'a' + 10;
        } else if(c >= 'A' && c <= 'F') {
            value += c - 'A' + 10;
        } else {
            return 0;
        }
    }
    return value;
}
