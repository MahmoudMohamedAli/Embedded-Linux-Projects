#include <iostream>
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <unistd.h>  // UNIX standard function definitions
#include <string.h>

int main() {
    // 1. Open the serial port "ttyS0"
    // O_RDWR: Read/Write, O_NOCTTY: Not the controlling terminal, O_NDELAY: Non-blocking
    int serial_port = open("/dev/ttyS0", O_RDWR | O_NOCTTY);

    if (serial_port < 0) {
        std::cerr << "Error " << errno << " from open: " << strerror(errno) << std::endl;
        return 1;
    }

    // 2. Create termios struct to configure port
    struct termios tty;

    if(tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
        return 1;
    }

    // 3. Set Baud Rate (Bits per second)
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // 4. Configure Control Flags (c_cflag)
    tty.c_cflag &= ~PARENB;        // Clear parity bit (No parity)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field (1 stop bit)
    tty.c_cflag &= ~CSIZE;         // Clear size bits
    tty.c_cflag |= CS8;            // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines

    // 5. Configure Local Flags (c_lflag) - "Raw Mode"
    tty.c_lflag &= ~ICANON; // Disable canonical mode (don't wait for newline)
    tty.c_lflag &= ~ECHO;   // Disable echo
    tty.c_lflag &= ~ISIG;   // Disable interpretation of INTR, QUIT, SUSP

    // 6. Set Timeout settings (vtime/vmin)
    tty.c_cc[VTIME] = 10;   // Wait for up to 1s (10 deciseconds)
    tty.c_cc[VMIN] = 0;     // Return as soon as any data is received

    // 7. Save settings
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        return 1;
    }

    // 8. Write/Read Loop
    const char* msg = "Hello from C++ UART!\n";
    write(serial_port, msg, strlen(msg));

    char read_buf;
    std::string rec_message; 
    while(true) {
        int n = read(serial_port, &read_buf, 1);
        if (n > 0) {
            std::cout << "recived char " << read_buf<<std::endl;
            if(read_buf == '\n' || read_buf == '\r' )
            {
                std::cout << "Received: " << rec_message << std::flush << std::endl;
                rec_message = ""; //reset

            }
            else
            {
                rec_message  += read_buf;
            }
        }
    }

    close(serial_port);
    return 0;
}