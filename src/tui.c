#include "../include/tui.h"
#include <stdio.h>
#include <string.h>

#define COLOR_RESET    "\033[0m"
#define COLOR_RED      "\033[31m"
#define COLOR_GREEN    "\033[32m"
#define COLOR_YELLOW   "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_CYAN     "\033[36m"
#define COLOR_BOLD     "\033[1m"

void tui_show_ascii_header() {
    tui_print_colored("╔══════════════════════════════════════════════════════════════════╗\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("    ██╗   ██╗███╗   ██╗██████╗  ██████╗     ████████╗██████╗ ███████╗███████╗", COLOR_GREEN);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("    ██║   ██║████╗  ██║██╔══██╗██╔═══██╗    ╚══██╔══╝██╔══██╗██╔════╝██╔════╝", COLOR_GREEN);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("    ██║   ██║██╔██╗ ██║██║  ██║██║   ██║       ██║   ██████╔╝█████╗  █████╗  ", COLOR_GREEN);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("    ██║   ██║██║╚██╗██║██║  ██║██║   ██║       ██║   ██╔══██╗██╔══╝  ██╔══╝  ", COLOR_GREEN);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("    ╚██████╔╝██║ ╚████║██████╔╝╚██████╔╝       ██║   ██║  ██║███████╗███████╗", COLOR_GREEN);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("     ╚═════╝ ╚═╝  ╚═══╝╚═════╝  ╚═════╝        ╚═╝   ╚═╝  ╚═╝╚══════╝╚══════╝", COLOR_GREEN);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    printf("                                                                    ");
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("              ███████╗██████╗ ██╗████████╗ ██████╗ ██████╗                   ", COLOR_YELLOW);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("              ██╔════╝██╔══██╗██║╚══██╔══╝██╔═══██╗██╔══██╗                  ", COLOR_YELLOW);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("              █████╗  ██║  ██║██║   ██║   ██║   ██║██████╔╝                  ", COLOR_YELLOW);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("              ██╔══╝  ██║  ██║██║   ██║   ██║   ██║██╔══██╗                  ", COLOR_YELLOW);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("              ███████╗██████╔╝██║   ██║   ╚██████╔╝██║  ██║                  ", COLOR_YELLOW);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("║", COLOR_CYAN);
    tui_print_colored("              ╚══════╝╚═════╝ ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝                  ", COLOR_YELLOW);
    tui_print_colored("║\n", COLOR_CYAN);
    tui_print_colored("╚══════════════════════════════════════════════════════════════════╝\n", COLOR_CYAN);
    tui_print_colored("            [Branching History | Tree-Based Undo/Redo]\n", COLOR_BOLD);
}

void tui_print_colored(const char *text, const char *color_code) {
    printf("%s%s%s", color_code, text, COLOR_RESET);
}

void tui_print_node_colored(UndoNode *node) {
    if (!node) return;

    if (node->is_current) {
        printf("-> ");
        tui_print_colored("[CURRENT] ", COLOR_GREEN);
    }

    printf("Node ");
    tui_print_colored(node->description, COLOR_CYAN);
    printf(" [ID: %d]\n", node->node_id);
}

void tui_render_tree(UndoNode *node, int depth, const char *prefix) {
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("│   ");

    if (depth > 0) printf("├── ");

    if (node->is_current)
        tui_print_colored("[*] ", COLOR_GREEN);
    else
        printf("    ");

    printf("Node %d: ", node->node_id);
    tui_print_colored(node->description, COLOR_YELLOW);
    printf("\n");

    for (int i = 0; i < node->child_count; i++) {
        tui_render_tree(node->children[i], depth + 1, prefix);
    }
}

void tui_render_full_tree(UndoTree *tree) {
    if (!tree || !tree->root) return;

    printf("\033[2J\033[H");
    tui_show_tree_header();
    printf("\n");
    tui_render_tree(tree->root, 0, "");
    printf("\n");
}

void tui_render_current_path(UndoTree *tree) {
    if (!tree || !tree->current) return;

    printf("Current path: ");
    UndoNode *node = tree->current;

    UndoNode *path[100];
    int depth = 0;
    while (node && depth < 100) {
        path[depth++] = node;
        node = node->parent;
    }

    for (int i = depth - 1; i >= 0; i--) {
        printf("%d", path[i]->node_id);
        if (i > 0) printf(" -> ");
    }

    printf("\n");
}

void tui_show_welcome() {
    printf("\033[2J\033[H");
    tui_show_ascii_header();
    printf("\n");
    tui_print_colored("Terminal-based text editor with branching undo/redo history\n", COLOR_BOLD);
    printf("Each edit creates a new branch instead of overwriting the future.\n\n");
    tui_print_colored("Features:\n", COLOR_GREEN);
    printf("  - Tree-based history (no edits lost)\n");
    printf("  - Jump to any previous state by ID\n");
    printf("  - Visual tree structure display\n");  
    printf("  - Arrow key branch selection\n\n");
    tui_print_colored("Press Enter key to start...\n", COLOR_CYAN);
}

void tui_show_tree_header() {
    tui_print_colored("    ┌─────────┐\n", COLOR_GREEN);
    tui_print_colored("    │  ROOT   │\n", COLOR_GREEN);
    tui_print_colored("    └────┬────┘\n", COLOR_GREEN);
    tui_print_colored("         ├─── Edit Branch\n", COLOR_YELLOW);
    tui_print_colored("         ├─── Edit Branch\n", COLOR_YELLOW);
    tui_print_colored("         └─── Edit Branch\n", COLOR_YELLOW);
    printf("\n");
}

void tui_show_goodbye() {
    tui_print_colored("Exiting Undo-Redo Editor.👋\n", COLOR_RED);
}

void tui_show_redo_options(UndoNode **children, int child_count, int current_node_id) {
    if (child_count == 0) {
        printf("No redo options available - you're at a leaf node\n");
        return;
    }
    
    printf("\n");
    tui_print_colored("=== Available Redo Branches ===\n", COLOR_BOLD);
    printf("From current node %d, you can branch to:\n\n", current_node_id);
    
    for (int i = 0; i < child_count; i++) {
        UndoNode *child = children[i];
        printf("  [%d] ", i);
        tui_print_colored("Node ", COLOR_CYAN);
        printf("%d: ", child->node_id);
        tui_print_colored(child->description, COLOR_YELLOW);
        printf("\n");
        printf("      Content: \"");
        if (strlen(child->content) > 60) {
            printf("%.60s...", child->content);
        } else {
            printf("%s", child->content);
        }
        printf("\"\n");
        char time_str[64];
        struct tm *tm_info = localtime(&child->timestamp);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        printf("      Created: %s\n", time_str);
        if (i < child_count - 1) printf("\n");
    }
    
    printf("\n");
    tui_print_colored("================================\n", COLOR_BOLD);
}

void tui_show_redo_result(int success, int node_id, const char* content) {
    if (success) {
        printf("\n");
        tui_print_colored("✓ Successfully moved to Node ", COLOR_GREEN);
        printf("%d\n", node_id);
        printf("New content: \"%s\"\n", content);
    } else {
        tui_print_colored("✗ Failed to move to selected node\n", COLOR_RED);
    }
}

int tui_select_branch_with_arrows(UndoNode **children, int child_count, int current_node_id) {
    if (child_count == 0) {
        tui_print_colored("No redo options available - you're at a leaf node\n", "\033[33m");
        return -1;
    }
    
    int selected = 0;
    char key;
    
    while (1) {
        printf("\033[2J\033[H");
        tui_print_colored("=== Select Branch with Arrow Keys ===\n", "\033[1m");
        printf("From current node %d, select your branch:\n\n", current_node_id);
        
        for (int i = 0; i < child_count; i++) {
            UndoNode *child = children[i];
            
            if (i == selected) {
                tui_print_colored("  > ", "\033[32m");
                tui_print_colored("[SELECTED] ", "\033[1;32m");
            } else {
                printf("    ");
            }
            
            tui_print_colored("Node ", "\033[36m");
            printf("%d: ", child->node_id);
            tui_print_colored(child->description, "\033[33m");
            printf("\n");
            printf("      Content: \"");
            if (strlen(child->content) > 50) {
                printf("%.50s...", child->content);
            } else {
                printf("%s", child->content);
            }
            printf("\"\n");
            char time_str[64];
            struct tm *tm_info = localtime(&child->timestamp);
            strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
            printf("      Created: %s\n", time_str);
            if (i < child_count - 1) printf("\n");
        }
        
        printf("\n");
        tui_print_colored("Controls: ↑/↓ to navigate, ENTER to select, 'q' to cancel\n", "\033[1m");
        tui_print_colored("=========================================\n", "\033[1m");
        
        key = getchar();
        
        if (key == 27) {
            char next1 = getchar();
            if (next1 == '[') {
                char arrow = getchar();
                switch (arrow) {
                    case 'A':
                        if (selected > 0) selected--;
                        break;
                    case 'B':
                        if (selected < child_count - 1) selected++;
                        break;
                }
            } else {
                return -1;
            }
            continue;
        }
        
        switch (key) {
            case 'k':
                if (selected > 0) selected--;
                break;
            case 'j':
                if (selected < child_count - 1) selected++;
                break;
            case '\n':
            case '\r':
                return selected;
            case 'q':
            case 'Q':
                return -1;
        }
    }
}
