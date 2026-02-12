#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1" // Typically i2c-1 on Raspberry Pi
#define DEVICE_ADDR 0x68     // Example: MPU6050 Accelerometer address

int main() {
    int fd;
    
    // 1. Open the I2C bus
    if ((fd = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        return 1;
    }

    // 2. Set the address of the slave device you want to talk to
    if (ioctl(fd, I2C_SLAVE, DEVICE_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        return 1;
    }

    // 3. Write: Tell the device which register we want to read (e.g., 0x3B)
    unsigned int reg = 0x3B;
    if (write(fd, &reg, 1) != 1) {
        perror("Failed to write to the i2c bus");
        return 1;
    }

    // 4. Read: Get data back from the device
    uint8_t buffer[6];
    if (read(fd, buffer, 6) != 6) {
        perror("Failed to read from the i2c bus");
        return 1;
    }

    // Output the raw bytes
    printf("Data received: %02X %02X %02X\n", buffer[0], buffer[1], buffer[2]);

    close(fd);
    return 0;
}