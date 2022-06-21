#include "random.h"

#include<stdlib.h>
#include<time.h>

void Random::next_bytes(uint8_t* bytes, int len) {
    srand((int)time(0));

    for (int i = 0; i < len; i++) {
        bytes[i] = random() % 512;
    }
}