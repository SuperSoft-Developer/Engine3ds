#include "../include/engine3ds/node.h"

void node_add_child(Node *parent, Node *child)
{
    if (parent->child_count >= MAX_CHILDREN)
        return;

    parent->children[parent->child_count++] = child;
}

void node_ready(Node *node)
{
    if (node->ready)
        node->ready(node);

    for (int i = 0; i < node->child_count; i++)
        node_ready(node->children[i]);
}

void node_update(Node *node, float delta)
{
    if (node->update)
        node->update(node, delta);

    for (int i = 0; i < node->child_count; i++)
        node_update(node->children[i], delta);
}

void node_draw(Node *node)
{
    if (node->draw)
        node->draw(node);

    for (int i = 0; i < node->child_count; i++)
        node_draw(node->children[i]);
}
