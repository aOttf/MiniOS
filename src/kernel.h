/*
 * kernel.h
 *
 *  Created on: Feb. 14, 2021
 *      Author: Wang Jingyu
 */
#ifndef KERNEL_H_
#define KERNEL_H_

#define DELETE "rm -rf BackingStore"
#define CREATE "mkdir BackingStore"
#define QUANTA 2

int min(int, int);
int scheduler();
int kernel();

#endif /* KERNEL_H_ */
