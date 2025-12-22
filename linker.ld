ENTRY(_start)

SECTIONS
{
    . = 0x80000;
    
    .text.boot : {
        KEEP(*(.text.boot))
    }
    
    .text : {
        *(.text*)
    }
    
    .rodata : {
        *(.rodata*)
    }
    
    .data : {
        *(.data*)
    }
    
    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }
    
    . = ALIGN(16);
    __stack_start = .;
    . = . + 0x10000;
    __stack_end = .;
    
    /DISCARD/ : {
        *(.ARM.exidx*)
        *(.comment)
        *(.eh_frame*)
    }
}