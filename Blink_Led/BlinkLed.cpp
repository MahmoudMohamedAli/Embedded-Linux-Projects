#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

class GPIO {
private:
    std::string pin_number;
    std::string path;

    void write_to_file(const std::string& filename, const std::string& value) {
        std::ofstream fs(filename);
        if (!fs.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return;
        }
        fs << value;
        fs.close();
    }

public:
    GPIO(int pin) : pin_number(std::to_string(pin)) {
        path = "/sys/class/gpio/gpio" + pin_number;
        // 1. Export the pin
        write_to_file("/sys/class/gpio/export", pin_number);
        // Small delay to let the OS create the file entries
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // 2. Set direction to 'out'
        write_to_file(path + "/direction", "out");
    }

    ~GPIO() {
        // 4. Unexport the pin on destruction
        write_to_file("/sys/class/gpio/unexport", pin_number);
    }

    void set_value(int val) {
        // 3. Write 1 or 0 to the value file
        write_to_file(path + "/value", std::to_string(val));
    }
};

int main() {
    const int BCM_PIN = 535; // Physical Pin 16 //Gpio 23 //base start at 512 for gpiochip0
    // you can find all info under /sys/class/gpio/gpiochip0/*
    
    GPIO led(BCM_PIN);

    std::cout << "Blinking LED on GPIO " << BCM_PIN << "..." << std::endl;

    for (int i = 0; i < 10; ++i) {
        led.set_value(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        led.set_value(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout<< i << " Iteration Ended" << std::endl;
    }

    return 0;
}