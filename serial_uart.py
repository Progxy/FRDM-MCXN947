import serial

# Configure your parameters here
PORT = '/dev/cu.usbmodem2O0I1ZATL3IGS3'  # Replace with your port name
BAUD = 115200                    # Replace with your baud rate
BYTESIZE =  serial.EIGHTBITS      # Options: EIGHTBITS, SEVENBITS, SIXBITS, FIVEBITS
PARITY =  serial.PARITY_NONE      # Options: PARITY_NONE, PARITY_EVEN, PARITY_ODD
STOPBITS =  serial.STOPBITS_ONE   # Options: STOPBITS_ONE, STOPBITS_TWO

try:
    ser = serial.Serial(
        port=PORT,
        baudrate=BAUD,
        bytesize=BYTESIZE,
        parity=PARITY,
        stopbits=STOPBITS,
        timeout=1
    )
    print(f"Connected to {PORT}. Listening...")

#    print("sending data...")
#    ser.write('a\r\n'.encode("utf-8"))
#    print("data sent...")

    while True:
        if ser.in_waiting > 0:
            print(f"incoming data {ser.in_waiting}")
            line = ser.readline().decode('utf-8').strip()
            print(line)

except KeyboardInterrupt:
    print("\nDisconnected.")
    ser.close()
