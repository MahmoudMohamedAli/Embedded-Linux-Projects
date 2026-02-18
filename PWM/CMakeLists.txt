cmake_minimum_required(VERSION 3.10)
project(PWMLed VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find libgpiod
find_library(GPIOD_LIBRARY NAMES gpiodcxx gpiod)
find_path(GPIOD_INCLUDE_DIR gpiod.hpp)

if(NOT GPIOD_LIBRARY OR NOT GPIOD_INCLUDE_DIR)
    message(FATAL_ERROR "libgpiod not found. Install with: sudo apt-get install libgpiod-dev")
endif()

# libgpiod software PWM implementation
add_executable(pwm_led_gpiod cpp/pwm_led_gpiod.cpp)
target_include_directories(pwm_led_gpiod PRIVATE ${GPIOD_INCLUDE_DIR})
target_link_libraries(pwm_led_gpiod ${GPIOD_LIBRARY})

# sysfs hardware PWM implementation (no external dependencies)
add_executable(pwm_led_sysfs sysfs/pwm_led_sysfs.cpp)

# Install targets
install(TARGETS pwm_led_gpiod pwm_led_sysfs
        RUNTIME DESTINATION bin)
