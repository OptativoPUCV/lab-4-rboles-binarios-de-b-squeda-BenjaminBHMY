#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));
    if (map == NULL) return NULL;
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree->root == NULL) {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        return;
    }

    TreeNode *temp = tree->root;

    while (temp != NULL) {
        if (is_equal(tree, key, temp->pair->key) == 1) return;
        
        else if (tree->lower_than(key, temp->pair->key) == 1) {
            if (temp->left == NULL) {
                temp->left = createTreeNode(key, value);
                temp->left->parent = temp;
                tree->current = temp->left;
                return;
            }
            temp = temp->left;
        }

        else {
            if (temp->right == NULL) {
                temp->right = createTreeNode(key, value);
                temp->right->parent = temp;
                tree->current = temp->right;
                return;
            }
            temp = temp->right;
        }
    }
}

TreeNode * minimum(TreeNode * x){
    TreeNode *temp = x;
    while (1) {
        if (temp->left == NULL) return temp;
        temp = temp->left;
    }
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent != NULL) {
            if (node->parent->left == node) node->parent->left = NULL;
            else node->parent->right = NULL;
        }
        else tree->root = NULL;
        free(node->pair);
        free(node);
    }
    else if (node->left == NULL || node->right == NULL) {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if (node->parent != NULL) {
            if (node->parent->left == node) node->parent->left = child;
            else node->parent->right = child;
        }
        else tree->root = child;
        child->parent = node->parent;
        free(node->pair);
        free(node);
    }
    else {
        TreeNode *successor = minimum(node->right);
        free(node->pair);
        node->pair = successor->pair;
        removeNode(tree, successor);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *temp = tree->root;
    while (temp != NULL)
    {
        if (is_equal(tree, key, temp->pair->key) == 1) {
            tree->current = temp;
            return temp->pair;
        }

        else if (tree->lower_than(key, temp->pair->key) == 1) {
            temp = temp->left;
        }
        else temp = temp->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *minNode = minimum(tree->root);
    tree->current = minNode;
    return minNode->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
