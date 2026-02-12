#include <gpiod.hpp>
#include <chrono>
#include <thread>
#include <iostream>

int main() {
    // Configuration
    // Use "0" or "gpiochip0" for Pi 4. Use "4" or "gpiochip4" for Pi 5.
    std::string chip_name = "/dev/gpiochip0"; 
    unsigned int line_offset = 23;            // GPIO 23
    double frequency = 100.0;                 // 100 Hz
    double duty_cycle = 0.25;                 // 25% Duty Cycle

    // Calculate timings
    auto period = std::chrono::microseconds(static_cast<int>(1000000 / frequency));
    auto on_time = std::chrono::microseconds(static_cast<int>(period.count() * duty_cycle));
    auto off_time = period - on_time;

    try {
        gpiod::chip  chip = gpiod::chip(chip_name);
        gpiod::line_request request = chip.prepare_request()
            .set_consumer("PWMGpioApp")
            .add_line_settings(
                line_offset,
                gpiod::line_settings()
                    .set_direction(gpiod::line::direction::OUTPUT)
            )
            .do_request();

        std::cout << "PWM started on GPIO " << line_offset << ". Press Ctrl+C to exit." << std::endl;

        while (true) {
            // In v1.x, use 1 for Active and 0 for Inactive
            request.set_value(line_offset, gpiod::line::value::ACTIVE);
            std::this_thread::sleep_for(on_time);
            
            request.set_value(line_offset, gpiod::line::value::INACTIVE);
            std::this_thread::sleep_for(off_time);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}