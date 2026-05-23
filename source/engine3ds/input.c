#include "../include/engine3ds/input.h"

static u32 keys_down;
static u32 keys_held;

void input_update(void)
{
    hidScanInput();

    keys_down = hidKeysDown();
    keys_held = hidKeysHeld();
}

int input_down(u32 key)
{
    return (keys_down & key) != 0;
}

int input_held(u32 key)
{
    return (keys_held & key) != 0;
}
