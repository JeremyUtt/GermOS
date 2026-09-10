

array = bytearray([0x00 for _ in range(0xFFB)])
array.extend(bytearray([48+4, 48+2, 48+0, 48+6, 48+9]))
with open("zeros.bin", "wb") as file:
    file.write(array)