# Onboard Temperature Sensor P3T1755 Demo

This directory contains a demonstration application that reads ambient temperature data from the onboard P3T1755 digital temperature sensor via the I2C interface and outputs the readings to the serial console.

## Hardware Configuration
* **Onboard Sensor**: P3T1755 I2C-bus digital temperature sensor.
* **Interface**: I2C Bus (the sensor is already connected to the bus).

## Sensor Details
The P3T1755 communicates over the I2C-bus interface and provides high-accuracy temperature measurements.

---

## Build and Flash
```bash
make flash
```

---

## Expected Behavior
Upon power-on or reset:
The application initializes the I3C/I2C interface and configures the P3T1755 sensor.
Ambient temperature readings are measured and printed to the serial terminal (UART) at regular intervals (e.g., every second).
In order to read from the UART use the python script in `utilities/debug_console.py`
