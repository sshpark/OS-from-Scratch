[bits 16]
; 切换到保护模式
switch_to_pm:
    cli

    lgdt [gdt_descriptor]   ; 读取 GDT

    mov eax, cr0            ; 为了使开关进入保护模式，我们将 CR0 的第一位（控制寄存器）置 1
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm

[bits 32]
; 在保护模式中初始化寄存器和堆栈
init_pm:
    mov ax, DATA_SEG        ; 保护模式下，旧段寄存器已经没有用
    mov ds, ax              ; 因此我们需要将其指向 GDT 中定义的位置
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000     ; 更新我们的堆栈位置
    mov esp, ebp

    call BEGIN_PM        ; 最后，调用到定义好的标签位置