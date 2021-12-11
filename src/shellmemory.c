/*
 * shellmemory.c
 *
 *
 *      Author: Jacob W
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "interpreter.h"
#include "shellmemory.h"

static struct MEM memory[SIZE] = { { NULL, NULL } };

int aprint(const char *var) {
	int i;
	for (i = 0; i < SIZE && memory[i].var != NULL; i++) {
		if (strcmp(memory[i].var, var) == 0) {
			puts(memory[i].value);
			return 0;
		}
	}

	puts("Variable does not exist.");
	return 1;

}

int aset(const char *var, const char *val){
	int i;
	for (i = 0; i < SIZE && memory[i].var != NULL; i++){
		if (strcmp(memory[i].var, var) == 0) {
					memory[i].value = strdup(val);
					return 0;
				}
	}

	//memory is full
	if (i >= 1000){
		puts("Store new variable failed. Memory is full.");
		return 1;
	}

	memory[i].var = strdup(var);
	memory[i++].value = strdup(val);

	if(i < 1000){
	memory[i].var = NULL;
	memory[i].value = NULL;
	}

	return 0;
}

