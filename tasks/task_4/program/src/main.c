int extern add(int, int);
int extern sub(int, int);

int main(int a, int b, int c) {
    int x = add(a, b);
    int y = sub(x, c);
    return y + 42;
}
