# 🌱 AgroSense Edge

> **From Soil to Cloud — Intelligent Edge Sensing for Precision Agriculture**

[![Build Status](https://github.com/<org>/agrosense-edge/actions/workflows/ci-build.yml/badge.svg)](https://github.com/<org>/agrosense-edge/actions)
[![Yocto](https://img.shields.io/badge/Yocto-Scarthgap%205.0-blue)](https://www.yoctoproject.org/)
[![Platform](https://img.shields.io/badge/platform-Raspberry%20Pi%204%20%7C%203-red)](https://www.raspberrypi.com/)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-brightgreen)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-lightgrey)](LICENSE)

---

## What is AgroSense Edge?

**AgroSense Edge** is a professional-grade, embedded IoT system that continuously reads environmental sensor data — temperature, humidity, and wind speed — from a Raspberry Pi 4/3 over I²C and SPI buses, buffers readings locally in EEPROM, and streams telemetry to a cloud-hosted MQTT broker and Node-RED dashboard in real time.

The project is delivered in two engineering phases:

| Phase       | Target      | Goal                                                                   |
| ----------- | ----------- | ---------------------------------------------------------------------- |
| **Phase 1** | Raspbian OS | Rapid development & validation of C++ sensor drivers and MQTT pipeline |
| **Phase 2** | Yocto Linux | Minimal, production-hardened custom image with systemd auto-start      |

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                     AgroSense Edge Device                       │
│                                                                 │
│  [SHT31 Temp/Humidity]  ──  I²C Bus  ──┐                        │
│  [AT24C256 EEPROM]      ──  I²C Bus  ──┤── C++17 Sensor Daemon  │
│  [Anemometer + MCP3208] ──  SPI Bus  ──┘      │                 │
│                                               │ Paho MQTT/TLS   │
└───────────────────────────────────────────────┼─────────────────┘
                                                │
                              ┌─────────────────▼───────────────────┐
                              │         IoT Cloud Layer             │
                              │                                     │
                              │  Mosquitto ──► Node-RED ──► UI      │
                              │                   └──────► InfluxDB │
                              └─────────────────────────────────────┘
```

---

## Key Features

- **Dual-bus sensing** — SHT31 temperature/humidity over I²C; anemometer via MCP3208 ADC over SPI
- **Local buffering** — AT24C256 EEPROM circular buffer survives network outages without data loss
- **Secure MQTT** — TLS 1.2, QoS 1, Last Will & Testament for offline detection
- **Node-RED dashboard** — Live gauges, 24-hour trend charts, and configurable threshold alerts
- **Production-grade OS** — Phase 2 Yocto image strips everything not needed; systemd manages the daemon
- **OTA-ready topic schema** — Reserved MQTT topic for remote config and firmware update triggers
- **Fully testable** — ISensor interface enables mock injection; Google Test suite included

---

## Hardware Requirements

| Component       | Part                          | Interface       |
| --------------- | ----------------------------- | --------------- |
| Compute         | Raspberry Pi 4 Model B (4 GB) | —               |
| Temp / Humidity | SHT31-D                       | I²C `0x44`      |
| Wind speed      | Davis 6410 + MCP3208 ADC      | SPI `spidev0.0` |
| Local buffer    | AT24C256 EEPROM (32 KB)       | I²C `0x50`      |
| Level shifter   | TXB0108                       | 5V ↔ 3.3V       |

Full wiring diagrams and pinout tables are in the [Wiki → Hardware BOM](../../wiki/Hardware-BOM).

---

## Quick Start — Phase 1 (Raspbian)

```bash
# 1. Flash Raspberry Pi OS Lite (64-bit) and SSH in

# 2. Install dependencies
sudo apt update && sudo apt install -y \
    git cmake build-essential libssl-dev \
    libpaho-mqtt-dev libpaho-mqttpp-dev i2c-tools

# 3. Enable I2C and SPI
sudo raspi-config   # Interface Options → I2C → Enable, SPI → Enable

# 4. Clone and build
git clone https://github.com/<org>/agrosense-edge.git
cd agrosense-edge && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$(nproc)

# 5. Configure and run
cp ../config/agrosense.example.json /etc/agrosense/config.json
# Edit broker URL, credentials, and device ID in config.json
sudo ./agrosense-app --config /etc/agrosense/config.json
```

---

## Quick Start — Phase 2 (Yocto)

```bash
# Clone Poky + layers
git clone --branch scarthgap git://git.yoctoproject.org/poky.git
cd poky
git clone --branch scarthgap git://git.yoctoproject.org/meta-raspberrypi
git clone https://github.com/<org>/agrosense-edge.git

# Init build environment
source oe-init-build-env build-agrosense

# Add layers and set MACHINE = "raspberrypi4-64" in conf/local.conf
# (see Wiki → Phase 2 for full bblayers.conf)

# Build
bitbake agrosense-image

# Flash
sudo bmaptool copy \
  tmp/deploy/images/raspberrypi4-64/agrosense-image-raspberrypi4-64.wic.bz2 \
  /dev/sdX
```

---

## Repository Structure

```
agrosense-edge/
├── src/
│   ├── sensors/          # SHT31 (I²C) and Anemometer (SPI) drivers
│   ├── storage/          # AT24C256 EEPROM circular buffer
│   ├── comms/            # Paho C++ MQTT client + JSON payload builder
│   └── utils/            # Logger, config parser
├── tests/                # Google Test unit tests
├── yocto/
│   └── meta-agrosense/   # Custom Yocto layer
│       ├── recipes-app/  # agrosense_1.0.bb + systemd service
│       ├── recipes-core/ # agrosense-image.bb
│       └── recipes-kernel/ # I²C + SPI kernel bbappend
├── docs/
│   ├── wiring-diagrams/
│   └── node-red-flows/   # agrosense-flow.json (importable)
├── config/
│   └── agrosense.example.json
└── scripts/
    ├── flash-raspbian.sh
    └── deploy-nodered-flow.sh
```

---

## MQTT Topic Schema

```
agrosense/<device_id>/sensors      ← telemetry (JSON, QoS 1)
agrosense/<device_id>/status       ← heartbeat / LWT (online | offline)
agrosense/<device_id>/config/set   → remote config push
agrosense/<device_id>/ota/trigger  → OTA firmware trigger (Phase 2)
```

**Telemetry payload example:**

```json
{
  "device_id": "agrosense-pi4-001",
  "timestamp": "2025-06-01T08:30:00Z",
  "sensors": {
    "temperature": 23.4,
    "humidity": 61.2,
    "wind_speed": 3.7
  },
  "unit": {
    "temperature": "°C",
    "humidity": "%RH",
    "wind_speed": "m/s"
  }
}
```

---

## Documentation

Full documentation lives in the [Wiki Page](https://github.com/aaabdelaziz/Embedded-Linux-Projects/wiki/AgroSense-Edge-%E2%80%94-IOT-Agriculture-Yocto-based-Project):

---

## Running Tests

```bash
cd build
ctest --output-on-failure
```

Tests cover SHT31 CRC verification, EEPROM circular buffer wraparound, and MQTT JSON payload serialization using mock sensor objects.

---

## Team Roles

| Role                   | Responsibilities                          |
| ---------------------- | ----------------------------------------- |
| Hardware lead          | Wiring, sensor calibration, EEPROM layout |
| Embedded software lead | C++ drivers, EEPROM buffer, CMake build   |
| BSP / Yocto engineer   | meta-agrosense layer, BitBake recipes     |
| Cloud / IoT engineer   | MQTT broker, Node-RED flows, InfluxDB     |
| QA / integration       | CI pipelines, HIL tests, documentation    |

---

## Contributing

1. Fork the repo and branch from `develop`: `git checkout -b feat/your-feature`
2. Write or update tests to cover your changes
3. Ensure `ctest` passes locally
4. Open a PR against `develop` with a clear description

Commits follow [Conventional Commits](https://www.conventionalcommits.org/): `feat:`, `fix:`, `docs:`, `chore:`, `test:`.

---

## License

Released under the [MIT License](LICENSE).

Copyright © 2025 AgroSense Edge Team

---

<p align="center">
  <strong>AgroSense Edge</strong> — Intelligent Edge Sensing for Precision Agriculture<br/>
  Built with Raspberry Pi · C++17 · Yocto Linux · Node-RED · MQTT
</p>
