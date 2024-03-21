#include <thread>
#include <cassert>
#include <algorithm>

#include "CppLinuxSerial/SerialPort.hpp"

#include "../../tasks/libs/utils/include/utils/config.h"

#if  BUILD_FOR_QEMU
const char* device_path = "/dev/pts/6";
constexpr mn::CppLinuxSerial::BaudRate baud_rate = mn::CppLinuxSerial::BaudRate::B_38400;
#else
const char* device_path = "/dev/ttyACM0";
constexpr mn::CppLinuxSerial::BaudRate baud_rate = mn::CppLinuxSerial::BaudRate::B_115200;
#endif

const char* program_path = "../../../cmake-build-debug/tasks/task_6/task6_program.img";

template<typename Rep, typename Period>
std::string wait_and_read(mn::CppLinuxSerial::SerialPort& serial_port, std::chrono::duration<Rep, Period> duration) {
    std::this_thread::sleep_for(duration);
    std::string read;
    serial_port.Read(read);
    read.erase (std::remove(read.begin(), read.end(), '\r'), read.end());
    return read;
}


int main() {
    std::ifstream program_file(program_path);

    std::stringstream str_stream;
    str_stream << program_file.rdbuf();
    auto program_string = str_stream.str();

    mn::CppLinuxSerial::SerialPort serial_port(device_path, baud_rate);
    serial_port.SetTimeout(0);
    serial_port.Open();

    serial_port.Write("s");

    std::cout << wait_and_read(serial_port, std::chrono::seconds(1)) << std::flush;
    serial_port.Write(std::to_string(program_string.size()) + "\n");

    std::cout << wait_and_read(serial_port, std::chrono::seconds(1)) << std::flush;
    serial_port.Write(program_string);

    for(int i = 0; i < 5; ++i) {
        std::cout << wait_and_read(serial_port, std::chrono::seconds(1)) << std::flush;
    }

    serial_port.Close();
}
