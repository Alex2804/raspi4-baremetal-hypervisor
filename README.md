# Baremetal Hypervisor on Raspberry Pi 4

This repository was part of a study course at TU Darmstadt. You can find the final Report [here](Laboratory_System_and_IoT_Security_Final_Report.pdf).

## Tasks

### task_0
Basic setup with uart 1 running on core #0.

### task_1
Determine the exception level (should be EL2) and switches to EL1.
Implements the EL2 exception vector table and uses the HVC instruction to switch from EL1 to EL2.
After the HVC instruction returns, the code steps down to EL0.

You can find a high level overview of the AArch64 Exception Model [here](tasks/task_1/aarch64-exception-model.md).

### task_2
Read general purpose registers, vector processing registers, memory-mapped registers, and control/status registers and print them for each privilege level.

### task_3
Reads *count* from uart. The following typed in chars from uart get written to memory until the *count* is reached, then the *count* input chars are read from memory and echoed back to uart.

### task_4
Creates a simple program that is written into memory and executed.

### task_5
Starts all cores and executes task_1 on all of them.

### task_6
Receives the program over uart, saves it to memory and executes it on all cores.

## References
- https://github.com/raspberrypi
- https://github.com/bztsrc/raspi3-tutorial/tree/master
- https://github.com/isometimes/rpi4-osdev/tree/master
- https://github.com/s-matyukevich/raspberry-pi-os/tree/master
