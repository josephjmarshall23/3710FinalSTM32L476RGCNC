#!/usr/bin/env python3
import serial
import time
import re
import sys

# ----------------------------
# Configuration (edit these)
# ----------------------------
PORT = "/dev/ttyACM0"
BAUD = 115200
GCODE_FILE = "output_0002.ngc"

# ----------------------------
# Helper: strip comments
# ----------------------------
def strip_comments(line):
    # Remove ( ... ) comments
    line = re.sub(r'\(.*?\)', '', line)
    # Remove ; comments
    line = line.split(';')[0]
    return line.strip()

# ----------------------------
# Main
# ----------------------------
def main():
    print(f"Opening serial port {PORT} @ {BAUD}...")
    ser = serial.Serial(PORT, BAUD, timeout=1)

    # Flush on startup
    time.sleep(2)
    ser.reset_input_buffer()

    print(f"Sending file: {GCODE_FILE}")

    with open(GCODE_FILE, "r") as f:
        for raw_line in f:
            clean = strip_comments(raw_line)

            if clean == "":
                continue  # Skip empty/comment-only lines

            # Log what we're sending
            print(f">>> {clean}")
            ser.write((clean + "\n").encode("ascii"))

            # Read everything the firmware sends back
            while True:
                response = ser.readline().decode(errors="ignore").strip()
                if response:
                    print(f"<<< {response}")

                    # Most GRBL-like systems end each command with "ok" or "error"
                    if response.startswith("ok") or response.startswith("error"):
                        break
                # else:
                #     break

    print("Done. Closing port.")
    ser.close()


if __name__ == "__main__":
    main()
