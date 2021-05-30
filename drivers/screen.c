#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"
#include <stdint.h>

/* 在屏幕的 row 行 col 列打印一个字符 */
int print_char(char character, int col, int row, char attribute_byte) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    if (!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

    int offset;

    if (col >= 0 && row >= 0) {
        offset = get_screen_offset(col, row);
    } else {
        offset = get_cursor();
    }

    if (character == '\n') {
        int rows = offset / (2*MAX_COLS); // 除 2 是因为一个单元格有占了两个字节
        offset = get_screen_offset(0, rows+1); // 在一行的 offset
    } else {
        vidmem[offset] = character;
        vidmem[offset+1] = attribute_byte;
    }

    offset += 2; // 移动到下一个屏幕单元
    offset = handle_scrolling(offset);

    set_cursor(offset);
    return offset;
}

void print_at(char *message, int col, int row) {
    if (col >= 0 && row >= 0) {
        set_cursor(get_screen_offset(col, row));
    }

    int i = 0, offset;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    uint8_t *screen = (uint8_t*) VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor(get_screen_offset(0, 0));
}

int get_cursor() {
    // reg 14: 光标的高位字节
    // reg 15: 光标的低位字节
    // 
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

int handle_scrolling(int cursor_offset) {
    if (cursor_offset < MAX_ROWS * MAX_COLS * 2) {
        return cursor_offset;
    }
    // 将当前行内容复制到上一行
    int i;
    for (i = 1; i < MAX_ROWS; i++) {
        memory_copy((uint8_t*)get_screen_offset(0, i) + VIDEO_ADDRESS, 
                    (uint8_t*)get_screen_offset(0, i-1) + VIDEO_ADDRESS, 
                    MAX_COLS * 2
        );
    }

    // 最后一行是空白行
    char *last_line = (char*) (get_screen_offset(0, MAX_ROWS - 1) + (uint8_t*) VIDEO_ADDRESS);
    for (i = 0; i < MAX_COLS*2; i++){
        last_line[i] = 0;
    }

    cursor_offset -= 2 * MAX_COLS;
    
    return cursor_offset;
}


void set_cursor(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

int get_screen_offset(int col, int row) {
    return 2*(row * MAX_COLS + col);
}

int get_offset_row(int offset) { 
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) { 
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2;
}