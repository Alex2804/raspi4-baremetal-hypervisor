# Baremetal Hypervisor on Raspberry Pi 4

## Tasks

### task_0
Basic setup with uart 1 running on core #0.

### task_1
Determine the exception level (should be EL2) and switches to EL1.
Implements the EL2 exception vector table and uses the HVC instruction to switch from EL1 to EL2.

## References
- https://github.com/raspberrypi
- https://github.com/bztsrc/raspi3-tutorial/tree/master
- https://github.com/isometimes/rpi4-osdev/tree/master
- https://github.com/s-matyukevich/raspberry-pi-os/tree/master