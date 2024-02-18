ENTRY(_main)
OUTPUT_ARCH(i386)

OUTPUT(build/full_kernel.o)
/* OUTPUT(build/full_kernel.bin) */
/* OUTPUT_FORMAT("binary") */



MEMORY
{
  NA2 (r) : ORIGIN = 0, LENGTH = 0x4ff
  RAM (wx) : org = 0x1000, len = 0x76ff   /* 0x7bff */
  BOOT (wx) : org = 0x7c00, len = 0x0200   /* 0x7dff */
  RAM2 (wx) : org = 0x7E00, len = 0x781FF   /* 7FFFF */  
  NA3 (r) : org = 0x00080000, len = 0x1FFFF  /* 0x0009FFFF */  
  VIDEO (rw) : org = 0x000A0000, len = 0x1FFFF  /* 0x0009FFFF */  
}


SECTIONS
{
  .btext : {
    *(.btext*) 
  } > BOOT

  /* .bdata : {
    *(.bdata*) 
  } > BOOT */

  .text : 
  {*(.btext)} > RAM2

  .data :
  {*(.data)} > RAM2

  .rodata :
  {*(.rodata)} > RAM2

  .eh_frame :
  {*(.eh_frame);} > RAM2

  .bss :
  {*(.bss)} > RAM2
}
