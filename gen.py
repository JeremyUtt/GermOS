

array = bytearray([0x00 for _ in range(0x50000)])

with open("nops.bin", "wb") as file:
    file.write(array)