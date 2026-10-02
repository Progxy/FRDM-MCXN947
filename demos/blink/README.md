# GPIO LED Blink Demo

This directory contains a demonstration application that blinks both the onboard RGB LED and an external LED connected to a dedicated GPIO pin.

## Hardware Configuration

* **Onboard RGB LED:** Default system onboard RGB LED.
* **External LED:** Connected to **GPIO4_1** (GPIO Bank 4, Pin 1).

### Wiring for External LED

Connect the external LED to your development board as follows:

```
[ GPIO4_1 Pin ] ---> [ Current Limiting Resistor (220Ω - 1kΩ) ] ---> [ LED Anode (+ long leg) ]
[ LED Cathode (- short leg) ] ---> [ GND Pin ]

```

> **Note:** Always use a current-limiting resistor (e.g., 220Ω to 1kΩ) to prevent damaging the GPIO pin and the LED.

---

## Build and Flash

```bash
make flash

```

---

## Expected Behavior

Upon power-on or reset:

* The onboard LED and the external LED on GPIO4_1 will toggle state synchronously in opposing states every second.
