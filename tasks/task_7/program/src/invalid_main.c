int extern add(int, int);
int extern sub(int, int);

int main(int a, int b, int c) {
    int x = add(a, b);
    asm("udf #0x0");
    int y = sub(x, c);
    return y + 42;
}
