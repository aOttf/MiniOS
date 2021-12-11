/*
 * memorymanager.h
 *
 *  Created on: Mar. 17, 2021
 *      Author: Wang Jingyu
 */

#include <stdio.h>

//main method
int launcher(FILE *p);

//helper methods
int countTotalPages(FILE *f);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame();
int findVictim(struct PCB *p);
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber,
		int victimFrame);
struct PCB* findVictimPCB(int framenumber);
int containsFrame(struct PCB *p, int val);
int findPageNumber(struct PCB *p, int frameNumber);
int FrameInUse(int);
FILE* cp(FILE*);
void printRAM();
int isEmpty(char*);
