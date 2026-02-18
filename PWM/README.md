# PWM LED Control Project

PWM-controlled LED brightness examples for Raspberry Pi using multiple implementation approaches.

## Project Structure

```
PWM/
├── cpp/
│   └── pwm_led_gpiod.cpp        # Software PWM with libgpiod v2
├── sysfs/
│   └── pwm_led_sysfs.cpp        # Hardware PWM via sysfs interface
├── service/
│   └── pwm-led.service          # systemd service file
├── images/
├── CMakeLists.txt
└── README.md
```

## Hardware Setup

| Component | Connection                                  |
| --------- | ------------------------------------------- |
| LED (+)   | GPIO 23 (Physical Pin 16) via 330Ω resistor |
| LED (-)   | GND (Physical Pin 14)                       |

## Implementations

### 1. Software PWM via libgpiod (Recommended)

Generates a PWM signal by toggling a GPIO pin in software using `libgpiod`.

```bash
# Build
mkdir build && cd build
cmake ..
make

# Run
sudo ./pwm_led_gpiod
```

### 2. Hardware PWM via sysfs

Uses the Raspberry Pi's built-in hardware PWM controller through the sysfs interface. Requires the `pwm` device tree overlay to be enabled.

```bash
# Enable PWM overlay (add to /boot/config.txt)
dtoverlay=pwm,pin=18,func=2

# Build
mkdir build && cd build
cmake ..
make

# Run
sudo ./pwm_led_sysfs
```

### 3. systemd Service

A service file is provided to run the PWM LED control at startup.

```bash
# Install the service
sudo cp service/pwm-led.service /etc/systemd/system/
sudo systemctl enable pwm-led
sudo systemctl start pwm-led
```

## Dependencies

```bash
# C++ development
sudo apt-get install libgpiod-dev
```
