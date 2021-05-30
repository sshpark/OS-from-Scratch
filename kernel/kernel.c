#include "../drivers/screen.h"

void kernel_main() {
    
    // 在第三行打印 Hello world
    char *msg = "Hello World";
    print_at(msg, 0, 3);
}