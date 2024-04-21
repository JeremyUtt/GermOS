

array = bytearray([0x00 for _ in range(0x7c00)])

with open("nops.bin", "wb") as file:
    file.write(array)