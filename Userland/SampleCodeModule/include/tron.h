#ifndef TRON_H
#define TRON_H

#include <stdint.h>
#include <user_syscalls.h>
#include <shell.h>
#include <stdio.h>
#include <beeper.h>

#define SCREEN_PIXEL_WIDTH 1024
#define SCREEN_PIXEL_HEIGHT 768

void tron();
int hitPlayer(uint32_t p1CurrX, uint32_t p1CurrY, uint32_t p2CurrX, uint32_t p2CurrY);
void tronChar(uint8_t scancode);
void gameOver(int whoHit);
void tronMenu();
void printWinner();
void printScore(int p1, int p2);
void endGame();
#endif