[org 0x7c00]
KERNEL_OFFSET equ 0x1000    ; This is the memory offset to which we will load our kernel
    ; 清屏
    pusha
    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10

    ; 将光标移动道起始位置
    mov ax, 0x0200
    mov bx, 0x0000
    mov dx, 0x0000
    int 0x10
    popa

    mov bp, 0x9000          ; 设置栈的地址
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call println

    call load_kernel

    call switch_to_pm       ; 切换到保护模式

    jmp $

%include "./boot/print_string.asm"
%include "./boot/disk_load.asm"
%include "./boot/gdt.asm"
%include "./boot/print_string_pm.asm"
%include "./boot/switch_to_pm.asm"

[bits 16]
; 读取内核程序
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call println

    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret

; 这是我们切换到并初始化保护模式之后到达的地方。
[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    call KERNEL_OFFSET

    jmp $

; 全局变量
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0

times 510-($-$$) db 0
dw 0xaa55