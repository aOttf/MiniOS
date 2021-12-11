/*
 * cpu.c
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
#include "kernel.h"
#include "memorymanager.h"
#include "ReadyQueue.h"

extern CPU cpu;
extern char *ram[];
extern RQ rq;

int cpu_run(int quanta) {
	struct PCB *curpcb = rq.head->cur;
	int err = 0;
	int i = 0;
	//setup cpu IP and offset
	cpu.IP = FRMSIZE * curpcb->pageTable[curpcb->PC_page];
	cpu.offset = curpcb->PC_offset;
	for (;
			cpu.offset < PGSIZE && (ram[cpu.IP + cpu.offset] != NULL)
					&& i < quanta; i++) {
		//run instruction
		strcpy(cpu.IR, ram[cpu.IP + cpu.offset]);
		err = parse(cpu.IR);
		if (err != 0)
			return err;

		//update PCB and CPU
		(curpcb->PC_offset)++;
		updatePC(curpcb);
		cpu.offset++;

	}

	//handle page fault
	if (cpu.offset >= PGSIZE) {
		(curpcb->PC_page)++;
		curpcb->PC_offset = 0;

		if (curpcb->PC_page >= curpcb->pages_max) {
			cpu_reset();
			return UNLOAD_READY;
		}

		//program is not finish yet
		if (curpcb->pageTable[curpcb->PC_page] == -1) {
			//no frame in the ram; need to find a frame
			int frameNumber;
			if ((frameNumber = findFrame()) == -1) {
				//victim frame case
				frameNumber = findVictim(curpcb);
				//update victim page table
				struct PCB *vicpcb = findVictimPCB(frameNumber);
				int vicPageNumber = findPageNumber(vicpcb, frameNumber);
				updatePageTable(vicpcb, vicPageNumber, -1, -1);
				updatePageTable(curpcb, curpcb->PC_page, -1, frameNumber);

			} else {
				updatePageTable(curpcb, curpcb->PC_page, frameNumber, -1);
			}

			//load into memory
			loadPage(curpcb->PC_page, curpcb->f, frameNumber);

		}

		updatePC(curpcb);
		cpu_reset();
		return UNLOAD_NOTYET;
	}

	if (ram[cpu.IP + cpu.offset] == NULL) {
		cpu_reset();
		return UNLOAD_READY;
	}

	//reset CPU
	cpu_reset();
	return UNLOAD_NOTYET;
}

void cpu_reset() {
	cpu.IP = 0;
	cpu.IR[0] = '\0';
	cpu.offset = 0;
}

