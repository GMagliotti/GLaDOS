#ifndef SHELL_H
#define SHELL_H

#include "funclib.h"
#include "tests.h"
#include "time.h"
#include "tron.h"
#include "user_syscalls.h"
#include <beeper.h>
#include <stdint.h>
#include "../programs/include/ipc_processes.h"
#include "../programs/include/process_info.h"
#include "../tests/test_util.h"
#include "philosophers.h"
#include <stdbool.h>

#define MAX_PARAMS 8

#define DEFAULT_TEXT_SIZE 2

#define FOREGROUND 0
#define BACKGROUND 1

void set_text_buffer_active(int active);

void save_char(char c);
void check_buffer();

void shell();
void set_bash();
void hvd_clear();
void help();
void show_arqui_commands();
void clear_command_buffer();
void set_size(int argc, char **argv);
void print_registers(); // it must do something...

extern void test_invalid_exc();
extern void test0_div();
void return_to_shell();

int get_params(char *str, char *params[], int max_params);

#endif