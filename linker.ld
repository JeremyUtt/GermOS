ENTRY(_main)
/* STARTUP("build/boot.o") */
OUTPUT(build/full_kernel.o)

STARTUP(build/boot.o)
OUTPUT_ARCH(i386)
OUTPUT_FORMAT("binary")




MEMORY
{
    NA2 (r) : ORIGIN = 0, LENGTH = 0x4ff
    RAM (wx) : org = 0x500, len = 0x76ff   /* 0x7bff */
    BOOT (wx) : org = 0x7c00, len = 0x0200   /* 0x7dff */
    RAM2 (wx) : org = 0x7E00, len = 0x781FF   /* 7FFFF */  
    NA3 (r) : org = 0x00080000, len = 0x1FFFF  /* 0x0009FFFF */  
    VIDEO (rw) : org = 0x000A0000, len = 0x1FFFF  /* 0x0009FFFF */  
}


SECTIONS
  {
    .btext : AT(0x7c00){
      *(.btext) _ebtext = . ; 
    } > BOOT
  
    .text : AT (0x7E00)
    {*(.btext); _etext = . ;  } > RAM2

    .data : AT ( ADDR (.text) + SIZEOF (.text) )
    {*(.data); _edata = . ;  } 

    .rodata : AT ( ADDR (.data) + SIZEOF (.data) )
    {*(.rodata); _erodata = . ;  } > RAM2

    .eh_frame : AT ( ADDR (.rodata) + SIZEOF (.rodata) )
    {*(.eh_frame); _eeh_frame = . ;  } > RAM2
  
    .bss : AT ( ADDR (.eh_frame) + SIZEOF (.eh_frame) )
    { _bstart = . ;  *(.bss) *(COMMON) ; _bend = . ;}
}