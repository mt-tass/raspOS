.section ".text.boot"
.global _start

_start:
    mrs     x0, mpidr_el1
    and     x0, x0, #3
    cbz     x0, core0
halt:
    wfe
    b       halt

core0:
    ldr     x0, =__stack_end
    mov     sp, x0
    ldr     x0, =__bss_start
    ldr     x1, =__bss_end
    
clear_bss:
    cmp     x0, x1
    b.ge    clear_bss_done
    str     xzr, [x0], #8
    b       clear_bss
    
clear_bss_done:
    bl      kernel_main
    b       halt