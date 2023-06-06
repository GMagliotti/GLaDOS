#ifndef TRON_H
#define TRON_H

#include "beeper.h"
#include "shell.h"
#include <stdint.h>
#include <stdio.h>
#include <user_syscalls.h>

#define SCREEN_PIXEL_WIDTH 1024
#define SCREEN_PIXEL_HEIGHT 768

void tron();
int hit_player(uint32_t p1_currX, uint32_t p1_currY, uint32_t p2_currX,
               uint32_t p2_currY);
void tron_char(uint8_t scancode);
void game_over(int whoHit);
void tron_menu();
void print_winner();
void print_score(int p1, int p2);
void end_game();
#endif