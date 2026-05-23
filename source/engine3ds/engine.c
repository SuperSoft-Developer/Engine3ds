#include <3ds.h>
#include <stdio.h>

#include "../include/engine3ds/engine.h"
#include "../include/engine3ds/input.h"
#include "../include/engine3ds/node.h"

void e3ds_init(void)
{
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
}

void e3ds_run(Scene *scene)
{
    u64 last_time = osGetTime();

    node_ready(&scene->root);

    while (aptMainLoop())
    {
        u64 now = osGetTime();
        float delta = (now - last_time) / 1000.0f;
        last_time = now;

        input_update();

        if (input_down(KEY_START))
            break;

        consoleClear();

        printf("Engine3ds\n");
        printf("Scene: %s\n\n", scene->name);

        node_update(&scene->root, delta);
        node_draw(&scene->root);

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }
}

void e3ds_shutdown(void)
{
    gfxExit();
}
