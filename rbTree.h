//
// Created by wojiaojd on 22/5/2020.
//

#ifndef CHATSERVER_RBTREE_H
#define CHATSERVER_RBTREE_H

#endif //CHATSERVER_RBTREE_H

#define rbt_parent(node) ((node)->parent)
#define rbt_set_red(node) (node)->color = RED
#define rbt_set_black(node) if((node)) (node)->color = BLACK
#define rbt_is_red(node) ((node) && (node)->color == RED)
#define rbt_is_black(node) (!(node) || (node)->color == BLACK)
#define rbt_color(node) ((node)?(node)->color:BLACK)
#define rbt_set_parent(c,p) ((c)->parent = (p))

typedef enum COLOR{
    RED,
    BLACK
}RBColor;

typedef struct rbTreeNode {
    RBColor color;
    void *key;
    void *value;
    struct rbTreeNode *left;
    struct rbTreeNode *right;
    struct rbTreeNode *parent;
}Node;

typedef struct rbRoot {
    Node *root;
    int (*cmp)(void*, void*);
}RBTree;

static void rbt_left_rotate(RBTree *rbTree, Node *x);

static void rbt_right_rotate(RBTree *rbTree, Node *y);

//static int rbt_compare(int (*cmp)(void*, void*), void *left, void *right);

void rbt_insert(RBTree *rbTree, Node *node);

static void rbt_insert_fixup(RBTree *rbTree, Node *node);

void rbt_delete(RBTree *rbTree, Node *node);

static void rbt_delete_fixup(RBTree *rbTree, Node *node, Node *parent);

Node *rbt_new_node();

