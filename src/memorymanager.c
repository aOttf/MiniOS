/*
 * memorymanager.c
 *
 *  Created on: Mar. 17, 2021
 *      Author: Wang Jingyu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "straux.h"
#include "shell.h"
#include "interpreter.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include "kernel.h"
#include "ReadyQueue.h"
#include "memorymanager.h"

extern char *ram[];
extern RQ rq;

int launcher(FILE *p) {
	static int pid = 0;

	int err = 0;
	if (p == NULL)
		return err;

	//system calls to copy file into backing store
	FILE *fptr = cp(p);
	fclose(p);

	//File can be launched; initialize pcb
	int totpgs = countTotalPages(fptr);
	struct PCB *pcb = makePCB(totpgs, fptr);
	addToReady(&rq, pcb);
	//printf("TOTAL PAGES : %d\n", totpgs);
	//load pages into memory
	int frameno;
	for (int i = 0; i < min(totpgs, 2); i++) {
		//printf("TIMES : %d\n", i);
		if ((frameno = findFrame()) == -1) {
			frameno = findVictim(pcb);
			//find victim and update pagetables
			struct PCB *vicpcb = findVictimPCB(frameno);
			int pgnum = findPageNumber(vicpcb, frameno);
			updatePageTable(vicpcb, pgnum, -1, -1);
			updatePageTable(pcb, i, -1, frameno);
		} else {
			updatePageTable(pcb, i, frameno, -1);
		}
		//printf("FRAMENUM %d \n", frameno);
		loadPage(i, fptr, frameno);
	}

	pid++;
	return 1;
}

//-------helper methods ------------------------//

//count total pages needed for the script
int countTotalPages(FILE *f) {
	rewind(f);
	int nl = 0;
	int np = 0;

	char tmp[BUFSIZE];
	while (fgets(tmp, BUFSIZE, f) != NULL) {
		if (!isEmpty(tmp))
			nl++;
	}

//round up division
	np = (nl - 1) / PGSIZE + 1;
	return np;

}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
	//reset file pointer to start
	rewind(f);

	char *cmds[PGSIZE] = { NULL };
	char tmp[BUFSIZE];

	int nl = 0;
	while (nl < pageNumber * PGSIZE) {
		fgets(tmp, BUFSIZE, f);
		if (!isEmpty(tmp))
			nl++;
	}

	int i = 0;
	while (fgets(tmp, BUFSIZE, f) && i < PGSIZE) {
		if (!isEmpty(tmp)) {
			rmnl(tmp);
			cmds[i++] = strdup(tmp);
		}
	}

	loadFrame(cmds, frameNumber);

}

//find next available frame
int findFrame() {
//FIFO based on Physical Address
	for (int i = 0; i < FRMNUM; i++)
		if (ram[i * FRMSIZE] == NULL)
			return i;

	return -1;
}

//used when findFrame() fails i.e. returns -1
int findVictim(struct PCB *p) {
	srand(time(0));
	int frmind = rand() % FRMNUM;

	while (1) {
		if (!containsFrame(p, frmind) && !FrameInUse(frmind))
			return frmind;

		frmind = (frmind + 1) % FRMNUM;
	}
	return 0;
}

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber,
		int victimFrame) {
	if (victimFrame == -1) {
		p->pageTable[pageNumber] = frameNumber;
	} else {
		p->pageTable[pageNumber] = victimFrame;
	}

	//upadte program counter
	updatePC(p);

	return 0;
}

struct PCB* findVictimPCB(int framenumber) {
	RQN *cur = rq.head;
	while (!containsFrame(cur->cur, framenumber))
		cur = cur->next;

	return cur->cur;
}

//check whether a given value is in the array
int containsFrame(struct PCB *p, int val) {
	for (int i = 0; i < FRMNUM; i++) {
		if (p->pageTable[i] == val)
			return 1;
	}
	return 0;
}

//find pcb's corresponding page number according to frame index
int findPageNumber(struct PCB *p, int frameNumber) {
	for (int i = 0; i < FRMNUM; i++) {
		if (p->pageTable[i] == frameNumber)
			return i;
	}

	//Not Found
	return -1;
}

//this function is used when trying to replace a victim frame
//the victim frame cant be selected if another program's Page Pointer is in the middle of that frame
int FrameInUse(int frameNumber) {
	//traverse ready list
	RQN *cur = rq.head;
	while (cur != NULL) {
		struct PCB *curpcb = cur->cur;
		int *curtbl = curpcb->pageTable;

		if (curtbl[curpcb->PC_page] == frameNumber)
			return 0;

		cur = cur->next;
	}

	return 1;
}

//copy file to backing store directory
FILE* cp(FILE *src) {
	static int pid = 0;
	char fname[50];

	//reset file pointer
	rewind(src);

	sprintf(fname, "./BackingStore/%d", pid);

	FILE *tgt = fopen(fname, "w+");
	int c;
	while ((c = getc(src)) != EOF)
		putc(c, tgt);

	rewind(tgt);
	pid++;
	return tgt;
}

void printRAM() {
	puts("----------------");
	for (int i = 0; i < MEMSIZE; i++) {
		if (ram[i] != NULL)
			printf("Instruction: %s  Address: %d \n", ram[i], i);
	}
	puts("----------------");
}

int isEmpty(char *line) {
	int i = 0;
	for (; line[i] == ' '; i++)
		;
	if (line[i] == '\n' || line[i] == '\0')
		return 1;
	return 0;

}
