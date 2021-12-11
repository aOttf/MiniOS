/*
 * ram.c
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
#include "ram.h"

extern char *ram[];

void loadFrame(char **cmds, int frameNumber) {
	for (int i = 0; i < FRMSIZE && cmds[i] != NULL; i++) {
		ram[frameNumber * FRMSIZE + i] = strdup(cmds[i]);
	}
}

void unloadFrame(int frameNumber) {
	for (int i = 0; i < FRMSIZE && ram[frameNumber * FRMSIZE + i] != NULL; i++) {

		free(ram[frameNumber * FRMSIZE + i]);
		ram[frameNumber * FRMSIZE + i] = NULL;
	}
}
