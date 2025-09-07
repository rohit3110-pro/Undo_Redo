#ifndef TUI_H
#define TUI_H

#include "undo_tree.h"

// Colored printing helpers
void tui_print_colored(const char *text, const char *color_code);
void tui_print_node_colored(UndoNode *node);

// Tree rendering
void tui_render_tree(UndoNode *root, int depth, const char *prefix);
void tui_render_full_tree(UndoTree *tree);

// Path display
void tui_render_current_path(UndoTree *tree);

void tui_show_welcome();
void tui_show_goodbye();

// Redo interface functions
void tui_show_redo_options(UndoNode **children, int child_count, int current_node_id);
void tui_show_redo_result(int success, int node_id, const char* content);

int tui_select_branch_with_arrows(UndoNode **children, int child_count, int current_node_id);

#endif 