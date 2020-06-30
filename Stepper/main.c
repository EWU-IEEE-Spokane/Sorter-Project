#include <stdint.h>

struct controlState {
    uint8_t cState;
    uint8_t next[5];
};

typedef const struct controlState controlState_t;

controlState_t controlFsm = {
    {0, {0, 1, 2, 3, 4} },
    {1, {0, 1, 2, 3, 4} }
};

void controlLoop() {
    uint8_t input, cState;

    while(1) {
        
    }
}