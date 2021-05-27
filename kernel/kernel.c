// 内核程序
// 打印一个 X 在左上角
void kernel_main() {
    char *video_mem = (char *) 0xb8000;
    *video_mem = 'X';
}