/*
 * ReadyQueue.c
 *
 *  Created on: Mar. 19, 2021
 *      Author: Wang Jingyu
 */


#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "ReadyQueue.h"

void addToReady(RQ* rq, struct PCB *pcb) {
	RQN *curnode = (RQN*) malloc(sizeof(RQN));

	if (rq->head == NULL) {
		rq->head = curnode;
	} else {
		rq->tail->next = curnode;
	}

	curnode->cur = pcb;
	curnode->next = NULL;
	rq->tail = curnode;
}

RQN* removeFirst(RQ* rq){
	if(rq->head == NULL)
		return NULL;

	RQN* tmp = rq->head;
	rq->head = rq->head->next;

	return tmp;
}

void freeRQN(RQN* rqn){
	freePCB(rqn->cur);
	free(rqn);
}


