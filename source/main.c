#include <3ds.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHILDREN 32

typedef struct Node {
    char name[32];

    void (*ready)(struct Node *self);
    void (*update)(struct Node *self, float delta);
    void (*draw)(struct Node *self);

    struct Node *children[MAX_CHILDREN];
    int child_count;
} Node;

typedef struct Scene {
    char name[32];
    Node root;
} Scene;

static void node_add_child(Node *parent, Node *child)
{
    if (parent->child_count >= MAX_CHILDREN) return;
    parent->children[parent->child_count++] = child;
}

static void node_ready(Node *node)
{
    if (node->ready) node->ready(node);

    for (int i = 0; i < node->child_count; i++) {
        node_ready(node->children[i]);
    }
}

static void node_update(Node *node, float delta)
{
    if (node->update) node->update(node, delta);

    for (int i = 0; i < node->child_count; i++) {
        node_update(node->children[i], delta);
    }
}

static void node_draw(Node *node)
{
    if (node->draw) node->draw(node);

    for (int i = 0; i < node->child_count; i++) {
        node_draw(node->children[i]);
    }
}

/* ---- Exemplo de nó Player ---- */

typedef struct Player {
    Node base;
    int x;
    int y;
} Player;

static void player_ready(Node *self)
{
    printf("[ready] %s\n", self->name);
}

static void player_update(Node *self, float delta)
{
    Player *player = (Player *)self;

    u32 keys = hidKeysHeld();

    if (keys & KEY_LEFT)  player->x--;
    if (keys & KEY_RIGHT) player->x++;
    if (keys & KEY_UP)    player->y--;
    if (keys & KEY_DOWN)  player->y++;

    (void)delta;
}

static void player_draw(Node *self)
{
    Player *player = (Player *)self;

    printf("\x1b[6;1HPlayer: %s", self->name);
    printf("\x1b[7;1HX: %d   Y: %d   ", player->x, player->y);
}

/* ---- Engine loop ---- */

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    printf("Engine3ds\n");
    printf("Mini Godot-like engine em C para Nintendo 3DS\n\n");
    printf("D-Pad move o Player\n");
    printf("START sai\n\n");

    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    strncpy(scene.name, "Main Scene", sizeof(scene.name) - 1);
    strncpy(scene.root.name, "Root", sizeof(scene.root.name) - 1);

    Player player;
    memset(&player, 0, sizeof(Player));
    strncpy(player.base.name, "Player", sizeof(player.base.name) - 1);
    player.base.ready = player_ready;
    player.base.update = player_update;
    player.base.draw = player_draw;
    player.x = 10;
    player.y = 10;

    node_add_child(&scene.root, (Node *)&player);
    node_ready(&scene.root);

    u64 last_time = osGetTime();

    while (aptMainLoop()) {
        u64 now = osGetTime();
        float delta = (now - last_time) / 1000.0f;
        last_time = now;

        hidScanInput();

        if (hidKeysDown() & KEY_START)
            break;

        consoleClear();

        printf("Engine3ds\n");
        printf("Scene: %s\n\n", scene.name);

        node_update(&scene.root, delta);
        node_draw(&scene.root);

        printf("\x1b[28;1HSTART para sair");

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
