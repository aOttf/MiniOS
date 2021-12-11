/*
 * ReadyQueue.h
 *
 *  Created on: Mar. 19, 2021
 *      Author: Wang Jingyu
 */

#ifndef READYQUEUE_H_
#define READYQUEUE_H_

#include "pcb.h"
typedef struct ReadyQueueNode {
	struct PCB *cur;
	struct ReadyQueueNode *next;
} RQN;

typedef struct ReadyQueue {
	RQN *head;
	RQN *tail;
} RQ;

//add to tail of the list
void addToReady(RQ*, struct PCB*);
RQN* removeFirst(RQ *);
void freeRQN(RQN *);


#endif /* READYQUEUE_H_ */
