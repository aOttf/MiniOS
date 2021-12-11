/*
 * cpu.h
 *
 *  Created on: Feb. 14, 2021
 *      Author: Wang Jingyu
 */

#ifndef CPU_H_
#define CPU_H_

typedef struct CPU {
	int IP;
	int offset; //memory address = IP + offset
	char IR[1000];
	int quanta;
}CPU;

#define UNLOAD_READY 1
#define UNLOAD_NOTYET 0

int cpu_run(int quanta);
void cpu_reset();
#endif /* CPU_H_ */
