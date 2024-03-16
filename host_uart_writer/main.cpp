#include <iostream>
#include <fstream>

int main() {
    std::ofstream file;
    file.open("/dev/ttyACM0", std::ios::binary);
    file.rdbuf()->pubsetbuf(0, 0);
    file << "Hello from host!\n";
    file.close();
}
