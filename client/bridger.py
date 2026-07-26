import socket
import os
import csv
from datetime import datetime
from dotenv import load_dotenv

load_dotenv()

UDP_IP = "0.0.0.0"
UDP_PORT = 5005
EXCEL_FILENAME = '../data/' + os.getenv("EXCEL_FILENAME")


def init_excel():
    if not os.path.exists(EXCEL_FILENAME):
        with open(EXCEL_FILENAME, mode="w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow(["Timestamp", "Temperature (°C)", "Humidity (%)"])
        print(f"[SYS] File '{EXCEL_FILENAME}' created successfully.")


def main():
    init_excel()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"[SYS] Wireless Streamer Online on Port {UDP_PORT}")
    print("[SYS] Listening for ESP32 telemetry...")

    try:
        while True:
            data, addr = sock.recvfrom(1024)
            message = data.decode("utf-8").strip()

            try:
                temp_str, humid_str = message.split(",")
                temp = float(temp_str)
                humid = float(humid_str)
                timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

                print(
                    f"[INF] Received from {addr[0]} -> Temp: {temp} °C | Humid: {humid} %"
                )

                with open(EXCEL_FILENAME, mode="a", newline="", encoding="utf-8") as f:
                    writer = csv.writer(f)
                    writer.writerow([timestamp, temp, humid])

            except ValueError:
                print(f"[ERR] Invalid message format from {addr[0]}: '{message}'")

    except KeyboardInterrupt:
        print("\n[SYS] Server stopped. Telemetry data safely saved.")


if __name__ == "__main__":
    main()
