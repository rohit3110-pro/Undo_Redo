#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include "tui.h"
#include "undo_tree.h"

#define MAX_INPUT 1000

static struct termios orig_termios;
static UndoTree *global_tree = NULL;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void handle_sigtstp(int sig) {
    if (global_tree && undo_tree_undo(global_tree)) {
        printf("\n[UNDO] Moved to previous state\n");
        printf("Current content: %s\n", undo_tree_get_current_content(global_tree));
        printf("Press any key to continue...\n");
        getchar();
    } else {
        printf("\n[UNDO] Cannot undo further\n");
    }
}

void clear_and_show_interface() {
    printf("\033[2J\033[H");
    printf("=== UNDO TREE TEXT EDITOR ===\n");
    printf("Commands:\n");
    printf("  'e' - Edit text\n");
    printf("  'u' - Undo (or Ctrl+Z)\n");
    printf("  'r' - Show redo options\n");
    printf("  't' - Show tree structure\n");
    printf("  'j' - Jump to node by ID\n");
    printf("  'p' - Show current path\n");
    printf("  's' - Show stats\n");
    printf("  'q' - Quit\n");
    printf("==============================\n\n");
}

void show_current_state(UndoTree *tree) {
    char *content = undo_tree_get_current_content(tree);
    printf("Current content: \"%s\"\n", content ? content : "(empty)");
    printf("Current node ID: %d\n", tree->current->node_id);
    
    int child_count;
    UndoNode **children = undo_tree_get_available_children(tree, &child_count);
    if (child_count > 0) {
        printf("Available redo options:\n");
        for (int i = 0; i < child_count; i++) {
            printf("  %d: Node %d - \"%s\"\n", i, children[i]->node_id, children[i]->description);
        }
    }
    printf("\n");
}

void edit_text(UndoTree *tree) {
    printf("Enter new text (max %d chars): ", MAX_INPUT - 1);
    fflush(stdout);
    
    disable_raw_mode();
    
    char input[MAX_INPUT];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;
        
        char description[MAX_DESCRIPTION];
        snprintf(description, sizeof(description), "Edit: %.50s%s", 
                input, strlen(input) > 50 ? "..." : "");
        
        UndoNode *new_node = undo_tree_add_state(tree, input, description);
        if (new_node) {
            printf("Added new state (Node %d)\n", new_node->node_id);
        } else {
            printf("Failed to add new state\n");
        }
    }
    
    enable_raw_mode();
}

void show_redo_options(UndoTree *tree) {
    int child_count;
    UndoNode **children = undo_tree_get_available_children(tree, &child_count);
    
    if (child_count == 0) {
        printf("No redo options available\n");
        printf("Press any key to continue...");
        getchar();
        return;
    }
    
    int selected_index = tui_select_branch_with_arrows(children, child_count, tree->current->node_id);
    
    if (selected_index >= 0) {
        UndoNode *selected = children[selected_index];
        int success = undo_tree_redo_to_child(tree, selected_index);
        tui_show_redo_result(success, selected->node_id, selected->content);
        printf("Press any key to continue...");
        getchar();
    }
}

void jump_to_node(UndoTree *tree) {
    printf("Enter node ID to jump to: ");
    disable_raw_mode();
    
    char input[20];
    if (fgets(input, sizeof(input), stdin)) {
        int node_id = atoi(input);
        if (undo_tree_jump_to_node(tree, node_id)) {
            printf("Jumped to Node %d\n", node_id);
        } else {
            printf("Node %d not found\n", node_id);
        }
    }
    
    enable_raw_mode();
    printf("Press any key to continue...");
    getchar();
}

void show_stats(UndoTree *tree) {
    int depth, branches, total_nodes;
    undo_tree_get_stats(tree, &depth, &branches, &total_nodes);
    
    printf("Tree Statistics:\n");
    printf("  Total nodes: %d\n", total_nodes);
    printf("  Max depth: %d\n", depth);
    printf("  Current node: %d\n", tree->current->node_id);
    printf("  Children of current: %d\n", tree->current->child_count);
    printf("\nPress any key to continue...");
    getchar();
}

int main() {
    tui_show_welcome();
    getchar();
    printf("Initializing Undo Tree Editor...\n");
    
    global_tree = undo_tree_create("Hello World!");
    if (!global_tree) {
        fprintf(stderr, "Failed to create undo tree\n");
        return 1;
    }
    
    signal(SIGTSTP, handle_sigtstp);
    
    enable_raw_mode();
    
    char command;
    while (1) {
        clear_and_show_interface();
        show_current_state(global_tree);
        
        printf("Enter command: ");
        command = getchar();
        printf("\n");
        
        switch (command) {
            case 'e':
            case 'E':
                edit_text(global_tree);
                break;
                
            case 'u':
            case 'U':
                if (undo_tree_undo(global_tree)) {
                    printf("Undo successful\n");
                } else {
                    printf("Cannot undo further\n");
                }
                printf("Press any key to continue...");
                getchar();
                break;
                
            case 'r':
            case 'R':
                show_redo_options(global_tree);
                break;
                
            case 't':
            case 'T':
                tui_render_full_tree(global_tree);
                printf("Press any key to continue...");
                getchar();
                break;
                
            case 'j':
            case 'J':
                jump_to_node(global_tree);
                break;
                
            case 'p':
            case 'P':
                tui_render_current_path(global_tree);
                printf("Press any key to continue...");
                getchar();
                break;
                
            case 's':
            case 'S':
                show_stats(global_tree);
                break;
                
            case 'q':
            case 'Q':
                tui_show_goodbye();
                undo_tree_destroy(global_tree);
                disable_raw_mode();
                return 0;
                
            default:
                printf("Unknown command: %c\n", command);
                printf("Press any key to continue...");
                getchar();
                break;
        }
    }
    
    return 0;
}
