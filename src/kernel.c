/*
 * kernel.c
 *
 *  Created on: Feb. 14, 2021
 *      Author: Wang Jingyu
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "interpreter.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include "shellmemory.h"
#include "memorymanager.h"
#include "ReadyQueue.h"
#include "straux.h"
#include "kernel.h"

RQ rq;
CPU cpu;
char *ram[MEMSIZE];

int min(int a, int b) {
	if (a <= b)
		return a;

	return b;
}

int scheduler() {
	while (rq.head != NULL) {
		//printf("TIMES : %d\n", i++);
		int err;
		if ((err = cpu_run(QUANTA)) == UNLOAD_READY) {
			//unload program from memory
			struct PCB *curpcb = rq.head->cur;
			for (int i = 0; i < curpcb->pages_max; i++) {
				int frameNumber = curpcb->pageTable[i];
				if (frameNumber != -1){
					unloadFrame(frameNumber);
				//	printRAM();
				}
			}

			//free real memory
			RQN *tmp = rq.head;
			rq.head = tmp->next;
			freeRQN(tmp);
		} else {
			RQN *oldFirst = removeFirst(&rq);
			addToReady(&rq, oldFirst->cur);
			free(oldFirst);
		}
	}
	return 0;
}

int kernel(void) {
	extern RQ rq;
	rq.head = NULL;
	rq.tail = NULL;

	cpu_reset();

	shellUI();

	return 0;
}

int boot() {
	extern char *ram[];
	for (int i = 0; i < MEMSIZE; i++)
		ram[i] = NULL;

	system(DELETE);
	system(CREATE);

	return 0;
}
int main() {
	int err = 0;
	boot();
	err = kernel();
	return err;
}

