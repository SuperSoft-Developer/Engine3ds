#include <string.h>

#include "../include/engine3ds/scene.h"

void scene_init(Scene *scene, const char *name)
{
    memset(scene, 0, sizeof(Scene));

    strncpy(scene->name, name, sizeof(scene->name) - 1);
    strncpy(scene->root.name, "Root", sizeof(scene->root.name) - 1);
}
