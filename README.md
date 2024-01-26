# Baremetal Hypervisor on Raspberry Pi 4

## Tasks

### task_0
Basic setup with uart 1 running on core #0.

Running with command:
```bash
qemu-system-aarch64 -S -M raspi3b -gdb tcp::58227 -serial null -serial stdio -kernel kernel8.img
```

### task_1
Determine the exception level (should be EL2) and switches to EL1.
Implements the EL2 exception vector table and uses the HVC instruction to switch from EL1 to EL2.

Running with command:
```bash
qemu-system-aarch64 -S -M raspi3b -gdb tcp::58227 -serial null -serial stdio -d int -kernel kernel8.img
```

### task_2
Read general purpose registers, vector processing registers, memory-mapped registers, and control/status registers and print them for each privilege level.

Running with command:
```bash
qemu-system-aarch64 -S -M raspi3b -gdb tcp::58227 -serial null -serial stdio -d int -kernel kernel8.img
```

You can find a small, high level overview of the AArch64 Exception Model [here](task_1/aarch64-exception-model.md).

## References
- https://github.com/raspberrypi
- https://github.com/bztsrc/raspi3-tutorial/tree/master
- https://github.com/isometimes/rpi4-osdev/tree/master
- https://github.com/s-matyukevich/raspberry-pi-os/tree/master