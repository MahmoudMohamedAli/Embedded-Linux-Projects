# Embedded Linux Projects

<div align="center">

![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-C51A4A?style=for-the-badge&logo=raspberry-pi&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

**A collection of embedded Linux projects for Raspberry Pi, demonstrating GPIO control, PWM, UART communication, and Qt integration.**

</div>

---

## Table of Contents

- [Overview](#overview)
- [Projects](#projects)
  - [ Blink LED](#1-blink-led)
  - [ PWM Control](#2-pwm-control)
  - [ UART Communication](#3-uart-communication)
  - [ Qt Integration](#4-qt-integration)
- [Hardware Requirements](#hardware-requirements)
- [Getting Started](#getting-started)

---

## Overview

This repository contains a series of embedded Linux projects targeting the **Raspberry Pi**. Each project demonstrates different aspects of hardware interfacing and communication protocols, implemented in **C++** and **Python**.

---

## Projects

### 1. Blink LED

> Toggle an LED using GPIO with multiple implementation approaches.

LED on GPIO 23 toggles every 500ms using C++ (`libgpiod`), Python, or direct sysfs interface.

📁 [**View Project Details →**](Blink_Led/README.md)

---

### 2. PWM Control

> Hardware and software PWM implementations for LED brightness control.

Control LED brightness using hardware PWM (sysfs) or software PWM (libgpiod).

- **Hardware PWM**: Uses the Raspberry Pi's dedicated PWM0 module (sysfs)
- **Software PWM**: GPIO-based PWM implementation with `libgpiod`
- **Boot Service**: Includes a systemd service file for automatic startup

📁 [**View Project Details →**](PWM/README.md)

---

### 3. UART Communication

> Serial communication applications for Raspberry Pi.

Three examples of serial communication implementation:

- **Basic UART**: Simple loopback and data transmission test
- **Threaded UART**: Multi-threaded TX/RX implementation for full-duplex communication
- **LED Control**: Control an LED via serial commands (`ON`, `OFF`, `TOGGLE`)
  **Companion Application:**

A Qt6-based serial terminal application is available for sending commands: [Serial App (Qt6 Framework)](https://github.com/MahmoudMohamedAli/Serial_App_QT/tree/main/Version1)

📁 [**View Project Details →**](UART_APP/README.md)

---

### 4. Qt Integration

> Integrating Qt framework with Raspberry Pi for GUI applications.

**Coming Soon !** - Stay tuned for Qt-based embedded GUI projects!

---

## Hardware Requirements

| Component     | Specification                                        |
| ------------- | ---------------------------------------------------- |
| **Board**     | Raspberry Pi (tested on RPi 3/4)                     |
| **LED**       | Standard LED with appropriate resistor               |
| **Pins Used** | GPIO 23 (Pin 16), GPIO 18 (Pin 12), GPIO 24 (Pin 18) |
| **USB-TTL**   | For UART projects (optional)                         |

### Pin Reference

```
Physical Pin 12 → GPIO 18 (PWM0)
Physical Pin 16 → GPIO 23 (LED Blink)
Physical Pin 18 → GPIO 24 (PWM0 alternate)
```

---

## Getting Started

### Prerequisites

```bash
# Install libgpiod development files
sudo apt-get install libgpiod-dev gpiod

# For Python projects
pip install gpiod
```

### Building C++ Projects

```bash
# Example: Compile Blink LED
g++ Blink_Led/Blink_Led_Gpiod.cpp -o blink_led -lgpiod

# Run the application
sudo ./blink_led
```

### Running Python Projects

```bash
# Run Python LED blink
sudo python3 Blink_Led/Blink_Led_Gpiod.py
```

---

## 📄 License

This project is open source and available for educational purposes.

---

<div align="center">

**Made with ❤️ for Embedded Linux enthusiasts**

</div>
