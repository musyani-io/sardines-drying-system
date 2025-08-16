# Sardines Drying System

This project is an automated sardines drying system using Arduino. It monitors temperature and humidity using a DHT11 sensor and controls a fan and heater to maintain optimal drying conditions. The system displays real-time data on an I2C LCD and provides status updates via serial output.

## Features

- **Temperature & Humidity Monitoring:** Uses DHT11 sensor.
- **Fan & Heater Control:** Automatically turns on/off based on temperature thresholds (Fan ON above 45°C, Heater ON below 30°C).
- **LCD Display:** Shows boot status and sensor readings.
- **Serial Debugging:** Prints sensor data and device status for troubleshooting.

## Current Status

- **Simulation Circuit:** Fully implemented but not tested yet.
- **PCB Design:** Not yet completed.
- **3D Cabinet Representation:** Not yet completed.

## How It Works

- On startup, the system displays a boot message.
- Continuously reads temperature and humidity.
- Controls fan and heater based on temperature.
- Displays sensor readings and status on the LCD and serial monitor.

## Hardware Used

- Arduino board
- DHT11 temperature and humidity sensor
- LiquidCrystal I2C LCD (16x2)
- Fan and heater (connected to digital pins)
- Supporting components (wires, breadboard, etc.)

## To Do

- Complete PCB design for hardware implementation.
- Create 3D representation of the drying cabinet.

## File Structure

- `src/main.cpp`: Main Arduino source code.

## Usage

1. Connect the components as per the simulation circuit.
2. Upload the code to the Arduino.
3. Monitor the LCD and serial output for system status.

---

\*Simulation circuit is ready for implementation. PCB design and 3D cabinet representation are yet to be designed.
