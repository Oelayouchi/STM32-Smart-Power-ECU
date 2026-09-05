import argparse
import csv
import time
from collections import deque

import matplotlib.pyplot as plt
import serial

STATE_NAMES = {
    0: "INIT",
    1: "NORMAL",
    2: "WARNING",
    3: "FAULT",
    4: "SHUTDOWN",
}

FAULT_NAMES = {
    0: "NONE",
    1: "UNDERVOLTAGE",
    2: "OVERVOLTAGE",
    3: "OVERCURRENT",
    4: "OVERTEMPERATURE",
    5: "SENSOR",
}


def parse_frame(frame: str) -> dict:
    values = {}
    for item in frame.split(","):
        if "=" not in item:
            continue
        key, value = item.split("=", 1)
        values[key.strip()] = value.strip()
    return values


def main():
    parser = argparse.ArgumentParser(description="STM32 Smart Power ECU serial dashboard")
    parser.add_argument("--port", required=True, help="Serial port, e.g. COM4 or /dev/ttyUSB0")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--log", default="telemetry_log.csv")
    args = parser.parse_args()

    ser = serial.Serial(args.port, args.baud, timeout=1)

    timestamps = deque(maxlen=200)
    voltages = deque(maxlen=200)
    currents = deque(maxlen=200)
    temperatures = deque(maxlen=200)

    with open(args.log, "a", newline="", encoding="utf-8") as csv_file:
        writer = csv.writer(csv_file)
        if csv_file.tell() == 0:
            writer.writerow(["timestamp", "voltage", "current", "temperature", "state", "fault", "relay"])

        plt.ion()
        fig = plt.figure(figsize=(10, 6))
        ax = fig.add_subplot(111)

        while True:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                continue

            frame = parse_frame(line)
            required = {"VBAT", "CURRENT", "TEMP", "STATE", "FAULT", "RELAY"}
            if not required.issubset(frame):
                continue

            try:
                voltage = float(frame["VBAT"])
                current = float(frame["CURRENT"])
                temperature = float(frame["TEMP"])
                state = int(frame["STATE"])
                fault = int(frame["FAULT"])
                relay = int(frame["RELAY"])
            except ValueError:
                continue

            now = time.time()
            timestamps.append(now)
            voltages.append(voltage)
            currents.append(current)
            temperatures.append(temperature)

            writer.writerow([
                time.strftime("%Y-%m-%d %H:%M:%S"),
                voltage,
                current,
                temperature,
                STATE_NAMES.get(state, str(state)),
                FAULT_NAMES.get(fault, str(fault)),
                relay,
            ])
            csv_file.flush()

            ax.clear()
            relative_t = [t - timestamps[0] for t in timestamps]
            ax.plot(relative_t, voltages, label="Battery voltage (V)")
            ax.plot(relative_t, currents, label="Current (A)")
            ax.plot(relative_t, temperatures, label="Temperature (C)")
            ax.set_xlabel("Time (s)")
            ax.set_ylabel("Measured value")
            ax.set_title(
                f"State: {STATE_NAMES.get(state, state)} | "
                f"Fault: {FAULT_NAMES.get(fault, fault)} | Relay: {relay}"
            )
            ax.legend()
            ax.grid(True)
            plt.pause(0.01)


if __name__ == "__main__":
    main()
