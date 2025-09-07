#ifndef UNDO_TREE_H
#define UNDO_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//will mostly work :(
#define MAX_CONTENT_SIZE 10000
#define MAX_CHILDREN 100
#define MAX_DESCRIPTION 256

typedef struct UndoNode {
    char *content;                    
    char description[MAX_DESCRIPTION]; 
    time_t timestamp;                 
    
    struct UndoNode *parent;          
    struct UndoNode **children;      
    int child_count; 
    int child_capacity;           
    
    int node_id;                  
    int is_current;                
} UndoNode;

typedef struct {
    UndoNode *root;                   
    UndoNode *current;     
    int next_id;            
    int total_nodes;               
} UndoTree;

UndoTree* undo_tree_create(const char* initial_content);
void undo_tree_destroy(UndoTree *tree);

// Navigation and modification
UndoNode* undo_tree_add_state(UndoTree *tree, const char* new_content, const char* description);
int undo_tree_undo(UndoTree *tree);
int undo_tree_redo_to_child(UndoTree *tree, int child_index);
int undo_tree_jump_to_node(UndoTree *tree, int node_id);

char* undo_tree_get_current_content(UndoTree *tree);
UndoNode** undo_tree_get_available_children(UndoTree *tree, int *count);

// Tree traversal and search
UndoNode* undo_tree_find_node_by_id(UndoTree *tree, int node_id);

int undo_tree_get_depth(UndoNode *node);
int undo_tree_get_total_nodes(UndoTree *tree);
void undo_tree_get_stats(UndoTree *tree, int *depth, int *branches, int *total_nodes);

#endif 