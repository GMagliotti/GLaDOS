#ifndef _BEEPER_H
#define _BEEPER_H

#include <funclib.h>
#include <user_syscalls.h>

void boot_sound();
void victory();
void beeper_songs(int argc, char **argv);
void zelda();
void FFV_theme();
void tetris_intro();
void mortal_kombat();
void mario();

#endif