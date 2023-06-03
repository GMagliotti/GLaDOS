/*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <stdint.h>

void _irq00_handler(void);
void _irq01_handler(void);
void _irq02_handler(void);
void _irq03_handler(void);
void _irq04_handler(void);
void _irq05_handler(void);
void _irq60_handler(uint64_t syscall);

void _exception0_handler(void);
void _exception6_handler(void);
void _exception13_handler(void);

void _cli(void);

void _sti(void);

void _hlt(void);

void pic_master_mask(uint8_t mask);

void pic_slave_mask(uint8_t mask);

// Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPS_H_ */
