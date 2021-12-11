/*
 * pcb.c
 *
 *  Created on: Feb. 14, 2021
 *      Author: Wang Jingyu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"
#include "ram.h"

struct PCB* makePCB(int pages_max, FILE *f) {
	struct PCB *pcb = (struct PCB*) malloc(sizeof(struct PCB));
	pcb->PC_offset = 0;
	pcb->PC_page = 0;
	pcb->pages_max = pages_max;
	//-1 means no corresponding frame in memory
	for (int i = 0; i < FRMNUM; i++)
		pcb->pageTable[i] = -1;
	pcb->PC = 0;
	pcb->f = f;

	return pcb;
}

void updatePC(struct PCB* pcb){
	pcb->PC = 4 * pcb->pageTable[pcb->PC_page] + pcb->PC_offset;
}

void freePCB(struct PCB* pcb){
	fclose(pcb->f);
	free(pcb);
}
