#ifndef PHYLO_H
#define PHYLO_H

#include "stdio.h"
// #include "user_syscalls.h"
#include "funclib.h"

// void phylo(int argc, char **argv);
void phylo();
static int add_phylo(int p_index);
static int remove_phylo(int p_index);
void end_table();
void phylo_process(int argc, char **argv);
static void print_state();
void put_chopstick(int p_index);
void take_chopstick(int p_index);
void update(int p_index);
int left(int p_index);
int right(int p_index);
void do_nothing();

#endif
