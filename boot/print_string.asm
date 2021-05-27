print_string:
    pusha

start:
    mov al, [bx] 
    cmp al, 0       ; 如果当前字符为 0，则输出结束
    je  done

    mov ah, 0x0e    ; 输出 [bx] 中的字符
    int 0x10

    add bx, 1
    jmp start

done:
    popa
    ret

println:
    pusha
    call print_string
    mov bx, newline
    call print_string
    popa
    ret

newline:
    db 13, 10, 0