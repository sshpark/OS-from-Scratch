[bits 32]
; Define some constants
VIDEO_MEMORY equ 0xb8000
VIDEO_MEMORY_START_OFF equ 0x140
WHITE_ON_BLACK equ 0x0f

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY               ; EDX 设置为 VIDEO_MEMORY 起始地址
    add edx, VIDEO_MEMORY_START_OFF

print_string_pm_loop:
    mov al, [ebx]                       ; 将 EBX 的字符存储在 AL 中
    mov ah, WHITE_ON_BLACK              ; 将字符属性存储在 AH 中

    cmp al, 0
    je print_string_pm_done

    mov [edx], ax           ; 当前字符的属性
                            
    add ebx, 1              ; 将 EBX 递增到字符串中的下一个字符。
    add edx, 2              ; 移到 vid_mem 中的下一个字符单元格。

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret