# Retard Home

A simple IoT project featuring an ESP8266 microcontroller that serves a web interface to monitor temperature/humidity and control LEDs.

## Features

- **Temperature and Humidity Monitoring**: Reads data from DHT11 sensor and displays it on a web interface
- **LED Control**: Toggle 3 LEDs via web buttons or physical push buttons
- **Responsive Web Interface**: Mobile-friendly design with real-time updates

## Hardware Requirements

- ESP8266 NodeMCU
- DHT11 Temperature and Humidity Sensor
- 3 LEDs (connected to D0, D1, D2)
- 3 Push Buttons (connected to D3, D4, D5)
- Resistors (for LEDs and buttons)
- Breadboard and jumper wires

## Wiring

| Component | ESP8266 Pin |
|-----------|-------------|
| LED 1     | D0 (GPIO16) |
| LED 2     | D1 (GPIO5)  |
| LED 3     | D2 (GPIO4)  |
| Button 1  | D3 (GPIO0)  |
| Button 2  | D4 (GPIO2)  |
| Button 3  | D5 (GPIO14) |
| DHT11     | D6 (GPIO12) |

## Software Requirements

- PlatformIO CLI or VSCode extension
- Required Libraries:
  - ESP8266WiFi
  - ESPAsyncWebServer
  - DHT sensor library

## Setup and Installation

1. Clone this repository or download the source code
2. Open the project in PlatformIO
3. Install the required libraries through PlatformIO Library Manager or run `pio run -t compiledb`
4. Connect your ESP8266 to your computer
5. Build and upload the code to your ESP8266 using PlatformIO or run `pio run -t upload`

## Usage

1. Power on your ESP8266
2. Connect to the WiFi access point named "ESP8266" with password "12345678"
3. Open a web browser and navigate to the ESP8266's IP address (typically 192.168.4.1) on port 3000
4. The web interface will display the current temperature and humidity
5. Use the buttons on the web interface to toggle the LEDs
6. Physical buttons can also be used to toggle the LEDs

## License

This project is open source and available under the [MIT](./LICENSE) license.
