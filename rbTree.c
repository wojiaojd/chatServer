//
// Created by wojiaojd on 22/5/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include "rbTree.h"

static void rbt_left_rotate(RBTree *rbTree, Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    if(y->left != NULL)
    {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if(x->parent == NULL)
    {
        rbTree->root = y;
    } else {
        if(x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static void rbt_right_rotate(RBTree *rbTree, Node *y)
{
    Node *x = y->left;
    y->left = x->right;
    if(x->right != NULL)
    {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == NULL)
    {
        rbTree->root = x;
    } else {
        if(y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

static int rbt_compare(int (*cmp)(void*, void*), void *left, void *right)
{
    if(!cmp)
        return (int)(left < right);
    else
        return (*cmp)(left, right);
}

static void rbt_insert(RBTree *rbTree, Node *node)
{
    Node *y = NULL;
    Node *x = rbTree->root;
    while(x != NULL)
    {
        y = x;
        if(rbTree->cmp)
        {
            //自定义了键值比较函数
            if(rbt_compare(rbTree->cmp, node->key, x->key))
                x = x->left;
            else
                x = x->right;
        } else {
            //默认键值比较函数
            if(*((char*)node->key) < *((char*)x->key))
                x = x->left;
            else
                x = x->right;
        }

    }
    rbt_parent(node) = y;
    if(y != NULL)
    {
        if(rbTree->cmp)
        {
            if(rbt_compare(rbTree->cmp, node->key, y->key))
                y->left = node;
            else
                y->right = node;
        } else {
            if(*((char*)node->key) < *((char*)y->key))
                y->left = node;
            else
                y->right = node;
        }

    } else {
        rbTree->root = node;
    }
    rbt_set_red(node);
    rbt_insert_fixup(rbTree, node);
}

static void rbt_insert_fixup(RBTree *rbTree, Node *node)
{
    Node *parent, *gparent;
    //若"父节点存在,并且父节点是红色的"
    while((parent = rbt_parent(node)) && rbt_is_red(parent))
    {
        gparent = rbt_parent(parent);
        //若"父节点"是"祖父节点的左孩子"
        if(parent == gparent->left)
        {
            //case 1:叔叔节点是红色
            Node *uncle = gparent->right;
            if(uncle && rbt_is_red(uncle))
            {
                rbt_set_black(uncle);
                rbt_set_black(parent);
                rbt_set_red(gparent);
                node = gparent;
                continue;
            }
            //case 2:叔叔是黑色, 且当前节点是右孩子
            if(parent->right == node)
            {
                Node *temp;
                rbt_left_rotate(rbTree, parent);
                temp = parent;
                parent = node;
                node = temp;
            }
            //case 3:叔叔是黑色, 且当前节点是左孩子
            rbt_set_black(parent);
            rbt_set_red(gparent);
            rbt_right_rotate(rbTree, gparent);
        } else {
            //case 1:叔叔节点是红色
            Node *uncle = gparent->left;
            if(uncle && rbt_is_red(uncle))
            {
                rbt_set_black(uncle);
                rbt_set_black(parent);
                rbt_set_red(gparent);
                continue;
            }
            //case 2:叔叔是黑色, 且当前节点是左孩子
            if(parent->left == node)
            {
                Node *temp;
                rbt_right_rotate(rbTree, parent);
                temp = parent;
                parent = node;
                node = temp;
            }
            //case 3:叔叔是黑色, 且当前节点是右孩子
            rbt_set_black(parent);
            rbt_set_red(gparent);
            rbt_left_rotate(rbTree, gparent);
        }
    }//while
    rbt_set_black(rbTree->root);
}

void rbt_delete(RBTree *rbTree, Node *node)
{
    Node *child, *parent;
    RBColor color;
    if((node->left != NULL) && (node->right != NULL))
    {
        Node *replace = node;
        replace = replace->right;
        while(replace->left != NULL)
            replace = replace->left;

        if(rbt_parent(node))
        {
            if(rbt_parent(node)->left == node)
                rbt_parent(node)->left = replace;
            else
                rbt_parent(node)->right = replace;
        } else {
            rbTree->root = replace;
        }

        child = replace->right;
        parent = rbt_parent(replace);
        color = rbt_color(replace);

        if(parent == node)
        {
            parent = replace;
        } else {
            if(child)
            {
                rbt_set_parent(child, parent);
            }
            parent->left = child;

            replace->right = node->right;
            rbt_set_parent(node->right, replace);
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if(color == BLACK)
            rbt_delete_fixup(rbTree, child, parent);
        free(node);
        return;
    }
    if(node->left != NULL)
    {
        child = node->left;
    } else {
        child = node->right;
    }

    parent = node->parent;
    color = node->color;

    if(child)
        child->parent = parent;

    if(parent)
    {
        if(parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    } else {
        rbTree->root = child;
    }
    if(color == BLACK)
        rbt_delete_fixup(rbTree, child, parent);
    free(node);
}

static void rbt_delete_fixup(RBTree *rbTree, Node *node, Node *parent)
{
    Node *other;
    while((!node || rbt_is_black(node)) && node != rbTree->root)
    {
        if(parent->left == node)
        {
            other = parent->right;
            if(rbt_is_red(other))
            {
                //case 1:x的兄弟w是红色的
                rbt_set_black(other);
                rbt_set_red(parent);
                rbt_left_rotate(rbTree, parent);
                other = parent->right;
            }
            if((!other->left || rbt_is_black(other->left)) &&
               (!other->right || rbt_is_black(other->right)))
            {
                //case 2:x的兄弟w是黑色的, 且w的俩孩子也都是黑色的
                rbt_set_red(other);
                node = parent;
                parent = rbt_parent(node);
            } else {
                if(!other->right || rbt_is_black(other->right))
                {
                    //case 3:x的兄弟w是黑色的, 且w的左孩子是红色, 右孩子为黑色
                    rbt_set_black(other->left);
                    rbt_set_red(other);
                    rbt_right_rotate(rbTree, other);
                    other->parent->right;
                }
                //case 4:x的兄弟w是黑色的, 且w的右孩子是红色, 左孩子任意颜色
                other->color = rbt_color(parent);
                rbt_set_black(parent);
                rbt_set_black(other->right);
                rbt_left_rotate(rbTree, parent);
                node = rbTree->root;
                break;
            }
        } else {
            other = parent->left;
            if(rbt_is_red(other))
            {
                //case 1: x的兄弟w是红色的
                rbt_set_black(other);
                rbt_set_red(parent);
                rbt_right_rotate(rbTree, parent);
                other = parent->left;
            }
            if((!other->left || rbt_is_black(other->left)) &&
               (!other->right || rbt_is_black(other->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rbt_set_red(other);
                node = parent;
                parent = rbt_parent(node);
            } else {
                if(!other->left || rbt_is_black(other->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色
                    rbt_set_black(other->right);
                    rbt_set_red(other);
                    rbt_left_rotate(rbTree, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                other->color = rbt_color(parent);
                rbt_set_black(parent);
                rbt_set_black(other->left);
                rbt_right_rotate(rbTree, parent);
                node = rbTree->root;
                break;
            }
        }
    }
    if(node)
        rbt_set_black(node);
}