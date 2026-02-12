#include <iostream>
#include <fcntl.h>   // File control definitions
#include <errno.h>   // Error number definitions
#include <termios.h> // POSIX terminal control definitions
#include <unistd.h>  // UNIX standard function definitions
#include <thread>
#include <string>
#include<cstring>
// Shared flag to stop threads
bool keep_running = true;

// --- RX THREAD FUNCTION ---
void rx_worker(int fd) {
    char buf;
    std::string message = "";
    while (keep_running) {
        int n = read(fd, &buf, 1);
        if (n > 0) {
            if (buf == '\n' || buf == '\r') {
                std::cout << "\n[RX]: " << message << std::endl;
                message.clear();
            } else {
                message += buf;
            }
        }
    }
}

// --- TX THREAD FUNCTION ---
void tx_worker(int fd) {
    std::string input;
    while (keep_running) {
        std::getline(std::cin, input);
        if (input == "exit") {
            keep_running = false;
            break;
        }
        input += "\n"; // Add newline for the receiver
        write(fd, input.c_str(), input.length());
    }
}

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
    std::thread rx_thread(rx_worker, serial_port);
    std::thread tx_thread(tx_worker, serial_port);
    
    // Wait for threads to finish
    rx_thread.join();
    tx_thread.join();
    
    close(serial_port);
    return 0;
}