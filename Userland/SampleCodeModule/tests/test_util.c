#include "test_util.h"

void tests(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: TEST <test_number> <param_1> <param_2>\n");
    printf("Available tests:\n");
    printf("  1 - Test memory manager <max_mem>\n");
    printf("  2 - Test priority\n");
    printf("  3 - Test processes <max_processes>\n");
    printf("  4 - Test synchronization <n> <inc>\n");
    printf("  5 - Test without synchronization <n> <inc>\n");

    return;
  }

  int test_num = string_to_int(argv[1]);

  if (test_num < 1 || test_num > 5) {
    printf("Invalid test number\n");
    return;
  }

  char *argv_aux[3] = {argv[2], "1", "1"};

  if (argc >= 4) {
    argv_aux[0] = argv[2];
    argv_aux[1] = argv[3];
  }

  switch (test_num) {
  case 1:
    test_mm(1, argv_aux);
    break;
  case 2:
    test_prio();
    break;
  case 3:
    test_processes(1, argv_aux);
    break;
  case 4:
    argv_aux[2] = "1";
    test_sync(5, argv_aux);
    break;
  case 5:
    argv_aux[2] = "0";
    test_sync(5, argv_aux);
    break;
  }
}

// Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

// Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
  uint8_t *p = (uint8_t *)start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

// Parameters
int64_t satoi(char *str) {
  uint64_t i = 0;
  int64_t res = 0;
  int8_t sign = 1;

  if (!str)
    return 0;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] != '\0'; ++i) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
    res = res * 10 + str[i] - '0';
  }

  return res * sign;
}

// Dummies
void bussy_wait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endless_loop() {
  while (1)
    ;
}

void endless_loop_print() {
  int pid = my_getpid();

  int wait = 10000;

  while (1) {
    printf("%d ", pid);
    bussy_wait(wait);
  }
}
