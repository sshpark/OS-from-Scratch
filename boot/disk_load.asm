; 从驱动 DL 读取 DH 个扇区到 ES:BX
disk_load:
    push dx

    mov ah, 0x02    ; BIOS 读取扇区功能
    mov al, dh      ; 读取 DH 个扇区
    mov ch, 0x00    ; 选择柱面 0
    mov dh, 0x00    ; 选择磁头 0
    mov cl, 0x02    ; 从第二个扇区读取数据，也就是引导扇区之后

    int 0x13        ; BIOS 中断

    jc disk_error   ; 错误处理

    pop dx
    cmp dh, al      ; 如果 AL (已读取的扇区数) != DH (期望读取的扇区数)
    jne disk_error
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $


; Variables
DISK_ERROR_MSG:
    db "Disk read error!", 0