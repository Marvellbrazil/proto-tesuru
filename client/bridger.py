import socket
import os
import csv
from datetime import datetime
from pathlib import Path
from dotenv import load_dotenv

load_dotenv()

UDP_IP = "0.0.0.0"
UDP_PORT = 5005

DATA_DIR = Path(__file__).resolve().parent.parent / "data"
EXCEL_FILENAME = DATA_DIR / (os.getenv("EXCEL_FILENAME") or "sensordata.csv")


def init_excel():
    DATA_DIR.mkdir(parents=True, exist_ok=True)

    if not EXCEL_FILENAME.exists():
        with open(EXCEL_FILENAME, mode="w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow(
                ["Timestamp", "Sequence", "Temperature (°C)", "Humidity (%)"]
            )
        print(f"[SYS] File '{EXCEL_FILENAME}' created successfully.")


def main():
    init_excel()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print(f"[SYS] Wireless Streamer Online on Port {UDP_PORT}")
    print("[SYS] Listening for ESP32 telemetry...")

    received = 0
    lost = 0
    last_seq = None

    with open(EXCEL_FILENAME, mode="a", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)

        try:
            while True:
                data, addr = sock.recvfrom(1024)
                message = data.decode("utf-8", errors="replace").strip()

                try:
                    seq_str, temp_str, humid_str = message.split(",")
                    seq = int(seq_str)
                    temp = float(temp_str)
                    humid = float(humid_str)
                except ValueError:
                    print(f"[ERR] Invalid message format from {addr[0]}: '{message}'")
                    continue

                timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                received += 1

                if last_seq is not None and seq > last_seq + 1:
                    missing = seq - last_seq - 1
                    lost += missing
                    print(f"[WRN] {missing} packet(s) lost before #{seq}")
                last_seq = seq

                print(
                    f"[INF] Received #{seq} from {addr[0]} -> "
                    f"Temp: {temp} °C | Humid: {humid} %"
                )

                writer.writerow([timestamp, seq, temp, humid])
                f.flush()

        except KeyboardInterrupt:
            total = received + lost
            rate = (lost / total * 100) if total else 0.0
            print("\n[SYS] Server stopped. Telemetry data safely saved.")
            print(f"[SYS] Received: {received} | Lost: {lost} | Loss rate: {rate:.2f}%")


if __name__ == "__main__":
    main()
