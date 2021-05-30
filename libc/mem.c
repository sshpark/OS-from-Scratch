#include "mem.h"

void memory_copy(uint8_t *source, uint8_t *dest, int no_bytes) {
    int i;
    for (i = 0; i < no_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}
