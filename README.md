# ESP32 IoT Temperature & Humidity Telemetry System

A prototype IoT monitoring system built with ESP32 (PlatformIO) and Python. It reads temperature and humidity from a DHT22 sensor, displays real-time data on an I2C 16x2 LCD display, controls status LEDs, allows user pausing via the BOOT button, and streams telemetry data over UDP to a Python client receiver that logs the records to a CSV file.

---

## 🛠️ Hardware Requirements

| Component | Pin / Address | Notes |
| :--- | :--- | :--- |
| **ESP32 DevKit V1** | — | Core Microcontroller |
| **DHT22 Sensor** | Pin `33` | Temperature & Humidity Sensor |
| **I2C LCD Display (16x2)** | SDA Pin `14`, SCL Pin `27` | Address `0x27` |
| **Status LED** | Pin `32` | Indicates active sensing & pause status |
| **BOOT Button** | Pin `0` | Press to toggle Pause / Resume |

---

## 📁 Repository Structure

```text
proto-tugas-akhir/
├── client/
│   ├── bridger.py        # Python UDP receiver & CSV logger script
│   └── requirements.txt  # Python package dependencies
├── data/                 # Directory where logged telemetry data CSV files are saved
├── include/              # Header files for ESP32 firmware modules
├── src/                  # Source files for ESP32 firmware
│   ├── DisplayManager.cpp
│   ├── LEDManager.cpp
│   ├── SensorManager.cpp
│   ├── WiFiStreamer.cpp
│   └── main.cpp
├── .env                  # Configuration file (included by main.cpp & read by Python)
├── .env.example          # Template for environment configuration
├── platformio.ini        # PlatformIO configuration
└── README.md             # Project documentation
```

---

## ⚙️ Configuration

Create a `.env` file in the root project directory by copying `.env.example`:

```bash
cp .env.example .env
```

Set your Wi-Fi credentials, destination IP address, and output file name inside `.env`:

```env
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"
#define LAPTOP_IP "192.168.1.X"
EXCEL_FILENAME="sensordata.csv"
```

> [!NOTE]
> The ESP32 C++ firmware includes `../.env` directly as a header (`#include "../.env"`), requiring `#define` statements for Wi-Fi and IP settings. Python's `python-dotenv` loads `EXCEL_FILENAME` for logging target specification.

---

## 🚀 How to Run

### Step 1: Upload Firmware to ESP32
1. Open the project in VS Code with the **PlatformIO** extension installed.
2. Connect your ESP32 board via USB.
3. Ensure `.env` is created in the root directory.
4. Upload and monitor the firmware:
   ```bash
   pio run -t upload
   pio device monitor
   ```

### Step 2: Start Python UDP Receiver
1. Navigate to the `client/` directory and install Python dependencies:
   ```bash
   cd client
   pip install -r requirements.txt
   ```
2. Run the receiver script:
   ```bash
   python bridger.py
   ```
3. The script will listen on UDP port `5005` and automatically log incoming telemetry data to `data/<EXCEL_FILENAME>`.

---

## ✨ Features

- **Sensors & Telemetry**: Periodic reading from DHT22 every 2 seconds sent over UDP (Port 5005).
- **LCD Display Management**: Displays current temperature, humidity, connectivity status, and custom running text animations.
- **Interactive Control**: Toggle system pause state using the built-in BOOT button (GPIO 0).
- **Data Logging**: Python client automatically logs received timestamped sensor readings into a CSV file.
