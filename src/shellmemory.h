/*
 * shellmemory.h
 *
 *  Created on: Jan. 23, 2021
 *      Author: Wang Jingyu
 */

#ifndef SHELLMEMORY_H_
#define SHELLMEMORY_H_

#define SIZE 100	//at most stores 100 variables
struct MEM {char *var; char *value;};

int aset(const char *, const char *);
int aprint(const char *);

#endif /* SHELLMEMORY_H_ */
