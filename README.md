# Sardines Drying System

This project is an automated sardines drying system built on Arduino. It uses sensors and actuators to monitor and control the drying process, with user interaction via buttons and an LCD.

## Features

- **Temperature & Humidity Monitoring:**  
  Uses a DHT11 sensor to measure temperature and humidity inside the cabinet.

- **Weight Measurement:**  
  An HX711 load cell amplifier measures the weight of the sardines, allowing calculation of weight loss during drying.

- **Fan & Heater Control:**  
  The fan and heater are automatically switched on/off based on user-defined temperature thresholds.

- **LCD Display:**  
  A 16x2 I2C LCD shows real-time sensor readings, system status, and allows for interactive settings adjustment.

- **Manual Settings via Buttons:**  
  Three buttons (`SET`, `UP`, `DOWN`) let users enter a settings mode and adjust the maximum and minimum temperature thresholds on-site.

- **Non-blocking Button Monitoring:**  
  Button states are checked continuously, ensuring responsive user interaction at all times.

- **Status Feedback:**  
  Serial output provides debugging information and status updates.

## How It Works

1. **Startup:**

   - Displays boot messages and prompts user to place a reference weight on the scale for calibration.
   - Prompts user to insert the load.

2. **Drying Process:**

   - Continuously reads temperature, humidity, and weight.
   - Controls fan and heater based on temperature thresholds.
   - Updates LCD with sensor readings and drying status.
   - Calculates and displays percentage weight loss.
   - Stops drying when weight loss exceeds 70%.

3. **Manual Settings:**
   - Press `SET` to enter settings mode.
   - Use `UP` or `DOWN` to select and adjust max/min temperature thresholds.
   - Changes are saved and reflected in system operation.

## Hardware Connections

- **DHT11 Sensor:** Pin 7
- **Fan:** Pin 6
- **Heater:** Pin 11
- **HX711 Load Cell:**
  - Data (DT): Pin 13
  - Clock (SCK): Pin 12
- **Buttons:**
  - SET: Pin 2
  - UP: Pin 3
  - DOWN: Pin 4
- **LCD:** I2C (address 0x27, typically SDA=A4, SCL=A5)

## Current Status

- **Simulation circuit:** Fully implemented and tested.
- **PCB design:** Not yet completed.
- **3D cabinet representation:** Not yet completed.

## To Do

- Complete PCB design for hardware implementation.
- Create 3D representation of the drying cabinet.

## Usage

1. Wire up the components as described above.
2. Upload the code to your Arduino.
3. Use the LCD and buttons for on-site configuration and monitoring.
4. Monitor serial output for debugging.

---

_Simulation circuit is ready for implementation. PCB design and 3D cabinet representation are yet to be designed._
