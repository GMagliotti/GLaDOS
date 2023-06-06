// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/shell.h"
#include "include/process_info.h"
#include <stdio.h>
#include <user_syscalls.h>

#define MAX_CHARS_PER_COMMAND 128

int min_char_size = 1;
int max_char_size = 6;

static char command_buffer[256] = {0}; // local buffer
static int command_buffer_pos = 0;     // current position of command_buffer
void save_char(char c) {
  // if received key is an enter, check what I already had, if it is a
  // backspace delete last char, else save in local buffer
  if (c == '\n') {
    printf("\n");
    check_buffer();
  } else if (c == '\b') {
    if (command_buffer_pos > 0) {
      command_buffer[--command_buffer_pos] = 0;
      printf("\b");
    }
  } else if (c == '\0') {
    return;
  } else {
    command_buffer[command_buffer_pos++] = c;
    putc(c);
  }

  if (command_buffer_pos >= MAX_CHARS_PER_COMMAND) {
    clear_command_buffer();
    printf(
        "\nMax command length reached. Type HELP to see available commands\n>");
  }
}

// checks if whats in buffer before the \n is a valid command, and also
//  clears the buffer
char *valid_commands[32][2] = {
    {"HELP", "Provides a list of available programs"},
    {"CLEAR", "Clears the screen"},
    {"ARQUI", "Shows old commands"},
    {"TIME", "Shows current system time (GMT-3)"},
    {"TRON", "Runs Tron game"},
    {"REGISTERS", "Prints value of all the registers (SHIFT + TAB)"},
    {"TEST0DIV", "Tests the division by 0 exception"},
    {"TESTINVALIDEXC", "Tests an invalid exception"},
    {"MEMORYAT", "Displays the content at the desired location"},
    {"SETSIZE", "Sets the letter size (Default 2)"},
    {"SONGS", "Choose from a variety of available songs"},
    {"MEM", "Prints the current memory status"},
    {"GETPID", "Displays the current process ID"},
    {"PS", "Prints the list of active processes"},
    {"LOOP", "Loops a new process printing its pid"},
    {"PKILL", "Kills a valid process"},
    {"NICE", "Changes the priority of a process"},
    {"BLOCK", "Block a process (YMMV)"},
    {"UNBLOCK", "Unblock a process (YMMV)"},
    {"CAT", "Prints the stdin it recieves"},
    {"WC", "Counts the amount of lines in input"},
    {"FILTER", "Filters the vocals in the input"},
    {"PRINTSCHEDULER", "Sets the scheduler on print mode"},
    {"PHYLO", "Runs Philosophers dilemma"},
    {"TEST", "Runs one of the available tests"}};

void (*command_functions[32])(int, char **) = {help,
                                               return_to_shell,
                                               show_arqui_commands,
                                               time,
                                               tron,
                                               print_registers,
                                               test0_div,
                                               test_invalid_exc,
                                               print_memory_at,
                                               set_size,
                                               beeper_songs,
                                               call_to_print_mem,
                                               getpid,
                                               ps,
                                               loop_process,
                                               kill_process,
                                               nice_process,
                                               block_process,
                                               unblock_process,
                                               cat,
                                               wc,
                                               filter,
                                               call_to_set_print_mode,
                                               philo,
                                               tests};

int find_pipe(char *params[], int argc) {
  for (int i = 0; i < argc; i++) {
    if (string_equals(params[i], "|")) {
      return i;
    }
  }
  return -1;
}

int is_valid_command(char *command) {
  for (int i = 0; valid_commands[i][0] != 0; i++) {
    if (string_equals(command, valid_commands[i][0])) {
      return i;
    }
  }
  return -1;
}

void check_buffer() {
  int found = 0;
  int command_pos = -1;
  int pid1 = -1;

  char *params[MAX_PARAMS] = {NULL};

  int argc = get_params(command_buffer, params, MAX_PARAMS);

  int we_piping = find_pipe(params, argc);

  if (we_piping == 0 || (we_piping == argc - 1 && argc > 1)) {
    printf("Invalid pipe position, %d, argc: %d\n", we_piping, argc);
    found = 1;
  }

  if (we_piping > 0 && we_piping < argc - 1) {
    int found2 = 0;

    int argc2 = argc - we_piping - 1;
    argc = we_piping;

    char *params2[MAX_PARAMS] = {NULL};

    for (int i = 0; i < argc2; i++) {
      params2[i] = params[we_piping + i + 1];
    }

    int pipe_id = call_to_pipe_open("|");
    if (pipe_id == -1)
      printf("Error opening pipe");
    int fd[2] = {pipe_id, 0};

    if ((command_pos = is_valid_command(params[0])) != -1) {
      pid1 = call_to_create_process(argc, params,
                                    command_functions[command_pos], fd);
      found = 1;
    }

    fd[0] = 0;
    fd[1] = pipe_id;

    if ((command_pos = is_valid_command(params[we_piping + 1])) != -1 &&
        found) {
      params2[argc2++] = "&";
      call_to_create_process(argc2, params2, command_functions[command_pos],
                             fd);
      found2 = 1;
    }

    if (!found2) {
      call_to_pkill_process(pid1);
      printf("Cmd 2 not found, killing process 1\n");
    }

  } else {
    if ((command_pos = is_valid_command(params[0])) != -1) {
      pid1 = call_to_create_process(argc, params,
                                    command_functions[command_pos], NULL);
      found = 1;
    }
  }

  if (pid1 > 0 && !string_equals(params[argc - 1], "&")) {
    call_to_waitpid(pid1);
  }

  if (!found && !is_only_space(command_buffer)) {
    printf("Invalid option. Type HELP for more information.\n");
  }

  printf(">");

  clear_command_buffer(); // clear local buffer, set current position at 0

  call_to_clear_buffer(); // clear kernel buffer through syscall
}

void clear_command_buffer() {
  for (int i = 0; i < 256; i++)
    command_buffer[i] = 0;
  command_buffer_pos = 0;
}

void shell() {
  call_to_set_size(DEFAULT_TEXT_SIZE);
  printf("Welcome to the command line! Type HELP for more information.\n");
  putc('>');
  while (1) {
    char c = get_char();
    save_char(c);
  }
}

void set_bash() {
  call_to_setptrx(0);
  call_to_setptry(0);
}

void hvd_clear() {
  call_to_fillrectangle(0, 0, 0, SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT);
  set_bash();
}

void help() {

  printf("The available commands are:\n\n");

  for (int i = 0; valid_commands[i][0] != 0 && valid_commands[i][1] != 0; i++) {
    if (i < 3 || i > 10)
      printf("\t%s - %s\n", valid_commands[i][0], valid_commands[i][1]);
    if (i == 9)
      printf("\n");
  }
}

void show_arqui_commands() {

  printf("Implemented previously:\n\n");

  for (int i = 3;
       valid_commands[i][0] != 0 && valid_commands[i][1] != 0 && i < 11; i++) {
    printf("\t%s - %s\n", valid_commands[i][0], valid_commands[i][1]);
  }
}

void return_to_shell() {
  hvd_clear(); // clear screen
}

void print_registers() { call_to_print_registers(); }

void set_size(int argc, char **argv) {

  if (argc < 2) {
    printf("Usage: set_size <size>\n");
    return;
  }

  int new_size = string_to_int(argv[1]);

  if (new_size < min_char_size || new_size > max_char_size) {
    printf("Size must be between %d and %d\n", min_char_size, max_char_size);
  } else {
    call_to_set_size(new_size);
    hvd_clear();
    printf("New screen size: %d\n", new_size);
  }
}

int get_params(char *str, char *params[], int max_params) {
  if (str == NULL || params == NULL || max_params <= 0) {
    return 0;
  }

  int count = 0;
  int i = 0;
  int param_index = 0;
  bool inside_param = false;

  while (str[i] != '\0') {
    if (!inside_param && !is_space(str[i])) {
      // Start of a new parameter
      inside_param = true;
      params[param_index] = str + i;
      param_index++;

      if (param_index == max_params) {
        break; // Reached the maximum number of parameters
      }
    } else if (inside_param && is_space(str[i])) {
      // End of a parameter
      inside_param = false;
      str[i] = '\0';
      count++;
    }

    i++;
  }

  if (param_index < max_params) {
    // Fill the remaining slots with NULL
    for (int j = param_index; j < max_params; j++) {
      params[j] = NULL;
    }
  }

  return param_index;
}
