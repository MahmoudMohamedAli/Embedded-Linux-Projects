#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

class HardwarePWM {
private:
    std::string path = "/sys/class/pwm/pwmchip0/pwm0/";

    void write_sysfs(const std::string& file, const std::string& value) {
        std::ofstream fs(path + file);
        if (!fs.is_open()) {
            // If pwm0 doesn't exist, we might need to export it first
            std::ofstream export_file("/sys/class/pwm/pwmchip0/export");
            export_file << "0";
            fs.open(path + file);
        }
        fs << value;
    }

public:
    void set_params(long period_ns, long duty_cycle_ns) {
        write_sysfs("enable", "0");           // Disable to change settings
        write_sysfs("period", std::to_string(period_ns));
        write_sysfs("duty_cycle", std::to_string(duty_cycle_ns));
        write_sysfs("enable", "1");           // Re-enable
    }
};

int main() {
    HardwarePWM pwm;

    // Period and Duty Cycle are in NANOSECONDS
    // 1ms period = 1,000,000 ns (1kHz frequency)
    long period = 1000000; 
    
    std::cout << "Starting Hardware PWM. Scaling duty cycle..." << std::endl;

    try {
        // Smoothly increase brightness
        for (int i = 0; i <= 100; i += 5) {
            long duty = (period * i) / 100;
            pwm.set_params(period, duty);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // Smoothly decrease brightness
        for (int i = 100; i >= 0; i -= 5) {
            long duty = (period * i) / 100;
            pwm.set_params(period, duty);    
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::exception& e) {
        std::cerr << "Ensure you have permission (sudo) and Pwm0 overlay is active." << std::endl;
    }

    return 0;
}