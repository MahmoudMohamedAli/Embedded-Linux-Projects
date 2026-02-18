cmake_minimum_required(VERSION 3.10)
project(UartApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Basic UART Communication
add_executable(uart_comm cpp/uart_comm.cpp)
target_link_libraries(uart_comm pthread)

# Multi-threaded UART (TX/RX Workers)
add_executable(uart_threaded cpp/uart_threaded.cpp)
target_link_libraries(uart_threaded pthread)

# Find libgpiod
find_library(GPIOD_LIBRARY NAMES gpiodcxx gpiod)
find_path(GPIOD_INCLUDE_DIR gpiod.hpp)

if(NOT GPIOD_LIBRARY OR NOT GPIOD_INCLUDE_DIR)
    message(WARNING "libgpiod not found. uart_led_control will not be built. Install with: sudo apt-get install libgpiod-dev")
else()
    # LED Control via UART
    add_executable(uart_led_control cpp/uart_led_control.cpp)
    target_include_directories(uart_led_control PRIVATE ${GPIOD_INCLUDE_DIR})
    target_link_libraries(uart_led_control pthread ${GPIOD_LIBRARY})
endif()

# Install targets
install(TARGETS uart_comm uart_threaded
        RUNTIME DESTINATION bin)

if(TARGET uart_led_control)
    install(TARGETS uart_led_control RUNTIME DESTINATION bin)
endif()
