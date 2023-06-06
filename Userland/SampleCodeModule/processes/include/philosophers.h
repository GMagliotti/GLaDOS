#ifndef _PHILOSOPHERS_H
#define _PHILOSOPHERS_H

void philo(int argc, char **argv);
void philosopher(int argc, char **argv);
void take_forks(int i);
void put_forks(int i);
void test(int i);
int left(int i);
int right(int i);
void think();
void eat();
void print_state();
int remove_philosopher();

#endif