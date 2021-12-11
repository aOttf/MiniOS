/*
 * ram.h
 *
 *  Created on: Feb. 14, 2021
 *      Author: Wang Jingyu
 */

#ifndef RAM_H_
#define RAM_H_

//We assume script size will not be larger than 40 cells i.e. 40 lines

//memory size
#define MEMSIZE 40

//cells per frame
#define FRMSIZE 4
//number of frames(pages) in memory
#define FRMNUM 10

#define PGSIZE 4

void unloadFrame(int);
void loadFrame(char**, int);
#endif /* RAM_H_ */
