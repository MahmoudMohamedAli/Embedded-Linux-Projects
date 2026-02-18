# UART Communication Project

Serial communication examples for Raspberry Pi using C++.

## Project Structure

```
UART_APP/
├── cpp/
│   ├── uart_comm.cpp        # Basic UART loopback/test
│   ├── uart_threaded.cpp    # Multi-threaded TX/RX implementation
│   └── uart_led_control.cpp # LED control via UART commands
├── images/
│   └── 1.png
├── CMakeLists.txt
└── README.md
```

## Hardware Setup

- **UART Interface**: The Raspberry Pi's primary UART (typically `/dev/serial0` or `/dev/ttyS0`).
- **USB-TTL Adapter**: Connect RX to TX and TX to RX for communication with a PC.
- **LED**: For the LED control example, connect an LED to the specified GPIO (check code for pin definition, usually GPIO 23).

## Implementations

### 1. Basic UART (`uart_comm`)

A simple example demonstrating how to configure the serial port and send/receive data.

```bash
# Build
mkdir build && cd build
cmake ..
make

# Run
sudo ./uart_comm
```

### 2. Multi-threaded UART (`uart_threaded`)

Uses separate threads for transmission (TX) and reception (RX) to handle full-duplex communication efficiently.

```bash
# Run
sudo ./uart_threaded
```

### 3. LED Control via UART (`uart_led_control`)

Control an LED on the Raspberry Pi by sending text commands over the serial port.

- **Commands**: `ON`, `OFF`, `TOGGLE`

```bash
# Run
sudo ./uart_led_control
```

## Companion Application

For testing the LED control, you can use our serial terminal app developed by Qt6:
[Serial App (Qt6 Framework)](https://github.com/MahmoudMohamedAli/Serial_App_QT/tree/main/Version1)

## Demo

![UART Demo](images/1.png)
