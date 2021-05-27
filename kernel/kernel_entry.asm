global _start;
[bits 32]

_start:
    [extern kernel_main]    ; 定义入口位置
    call kernel_main        ; ld 将知道它在内存中的位置
    jmp $