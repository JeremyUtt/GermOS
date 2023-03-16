ENTRY(_main)
STARTUP("build/boot.o")
OUTPUT_FORMAT("binary")
/* INPUT(File1.o File2.o File3.o ...) */
OUTPUT(kernel.bin)

MEMORY
{
    NO (r) : ORIGIN = 0, LENGTH = 0x4ff
    RAM (wx) : org = 0x500, len = 0x76ff   /* 0x7bff */
    BOOT (wx) : org = 0x7c00, len = 0x512
    NO2 (wx) : org = 0x7E00, len = 0x781FF   /* 7FFFF */  
    NO3 (r) : org = 0x00080000, len = 0x1FFFF  /* 0x0009FFFF */  
    VIDEO (r) : org = 0x000A0000, len = 0x1FFFF  /* 0x0009FFFF */  
}


SECTIONS
{
  .=0x7c00

  .text : { build/boot.o (.text); }

  /* .text.start : { kernel_entry.o( .text )} */
  . = 0x1000;
  .text : { * (.text); }

  . = 0x5000;
  .data : { * (.data); }
}