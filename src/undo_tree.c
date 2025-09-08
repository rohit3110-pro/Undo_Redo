#include "../include/undo_tree.h"

static UndoNode* create_node(const char* content, const char* description, UndoNode* parent, int node_id) {
    UndoNode *node = malloc(sizeof(UndoNode));
    if (!node) return NULL;
    
    node->content = malloc(strlen(content) + 1);
    if (!node->content) {
        free(node);
        return NULL;
    }
    strcpy(node->content, content);
    
    strncpy(node->description, description, MAX_DESCRIPTION - 1);
    node->description[MAX_DESCRIPTION - 1] = '\0';
    
    node->timestamp = time(NULL);
    node->parent = parent;
    node->children = malloc(sizeof(UndoNode*) * 4);
    node->child_count = 0;
    node->child_capacity = 4;
    node->node_id = node_id;
    node->is_current = 0;
    
    return node;
}

static int add_child_to_node(UndoNode *parent, UndoNode *child) {
    if (parent->child_count >= parent->child_capacity) {
        int new_capacity = parent->child_capacity * 2;
        UndoNode **new_children = realloc(parent->children, sizeof(UndoNode*) * new_capacity);
        if (!new_children) return 0;
        
        parent->children = new_children;
        parent->child_capacity = new_capacity;
    }
    
    parent->children[parent->child_count++] = child;
    return 1;
}

UndoTree* undo_tree_create(const char* initial_content) {
    UndoTree *tree = malloc(sizeof(UndoTree));
    if (!tree) return NULL;
    
    tree->root = create_node(initial_content, "Initial state", NULL, 0);
    if (!tree->root) {
        free(tree);
        return NULL;
    }
    
    tree->current = tree->root;
    tree->current->is_current = 1;
    tree->next_id = 1;
    tree->total_nodes = 1;
    
    return tree;
}

static void destroy_node(UndoNode *node) {
    if (!node) return;
    
    for (int i = 0; i < node->child_count; i++) {
        destroy_node(node->children[i]);
    }
    
    free(node->content);
    free(node->children);
    free(node);
}

void undo_tree_destroy(UndoTree *tree) {
    if (!tree) return;
    
    destroy_node(tree->root);
    free(tree);
}

UndoNode* undo_tree_add_state(UndoTree *tree, const char* new_content, const char* description) {
    if (!tree || !tree->current || !new_content) return NULL;
    
    UndoNode *new_node = create_node(new_content, description, tree->current, tree->next_id++);
    if (!new_node) return NULL;
    
    if (!add_child_to_node(tree->current, new_node)) {
        destroy_node(new_node);
        return NULL;
    }
    
    tree->current->is_current = 0;
    tree->current = new_node;
    tree->current->is_current = 1;
    tree->total_nodes++;
    
    return new_node;
}

int undo_tree_undo(UndoTree *tree) {
    if (!tree || !tree->current || !tree->current->parent) return 0;
    
    tree->current->is_current = 0;
    tree->current = tree->current->parent;
    tree->current->is_current = 1;
    
    return 1;
}

int undo_tree_redo_to_child(UndoTree *tree, int child_index) {
    if (!tree || !tree->current) return 0;
    if (child_index < 0 || child_index >= tree->current->child_count) return 0;
    
    tree->current->is_current = 0;
    tree->current = tree->current->children[child_index];
    tree->current->is_current = 1;
    
    return 1;
}

int undo_tree_jump_to_node(UndoTree *tree, int node_id) {
    if (!tree) return 0;
    
    UndoNode *target = undo_tree_find_node_by_id(tree, node_id);
    if (!target) return 0;
    
    tree->current->is_current = 0;
    tree->current = target;
    tree->current->is_current = 1;
    
    return 1;
}

char* undo_tree_get_current_content(UndoTree *tree) {
    if (!tree || !tree->current) return NULL;
    return tree->current->content;
}

UndoNode** undo_tree_get_available_children(UndoTree *tree, int *count) {
    if (!tree || !tree->current || !count) return NULL;
    
    *count = tree->current->child_count;
    return tree->current->children;
}

static UndoNode* find_node_by_id_recursive(UndoNode *node, int node_id) {
    if (!node) return NULL;
    if (node->node_id == node_id) return node;
    
    for (int i = 0; i < node->child_count; i++) {
        UndoNode *found = find_node_by_id_recursive(node->children[i], node_id);
        if (found) return found;
    }
    
    return NULL;
}

UndoNode* undo_tree_find_node_by_id(UndoTree *tree, int node_id) {
    if (!tree || !tree->root) return NULL;
    return find_node_by_id_recursive(tree->root, node_id);
}

void undo_tree_get_stats(UndoTree *tree, int *depth, int *branches, int *total_nodes) {
    if (!tree) return;
    
    *total_nodes = tree->total_nodes;
    *depth = undo_tree_get_depth(tree->root);
    *branches = 0;
}

int undo_tree_get_depth(UndoNode *node) {
    if (!node) return 0;
    
    int max_child_depth = 0;
    for (int i = 0; i < node->child_count; i++) {
        int child_depth = undo_tree_get_depth(node->children[i]);
        if (child_depth > max_child_depth) {
            max_child_depth = child_depth;
        }
    }
    
    return 1 + max_child_depth;
}

int undo_tree_get_total_nodes(UndoTree *tree) {
    return tree ? tree->total_nodes : 0;
}