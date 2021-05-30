#ifndef __SCREEN_H
#define __SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f

// 定义 IO 设备的端口号
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

int get_screen_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int handle_scrolling(int offset);
int get_cursor();
void set_cursor(int offset);
void clear_screen();
void print_at(char *message, int col, int row);
int print_char(char character, int col, int row, char attribute_byte);

#endif