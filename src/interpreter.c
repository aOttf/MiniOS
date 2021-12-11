/*
 * interpreter.c
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
#include "ram.h"
#include "pcb.h"
#include "kernel.h"
#include "memorymanager.h"

int unknown(char*);
int help(char*[]);
int quit(char*[]);
int set(char*[]);
int print(char*[]);
int run(char*[]);
int exec(char*[]);

int interpreter(char *words[]) {
	int err = 0;
	if (words[0] == NULL) {
		return 0;
	}

	char *cmd = strdup(words[0]);

	if (strcmp(cmd, "help") == 0) {
		err = help(words);
	} else if (strcmp(cmd, "set") == 0) {
		err = set(words);
	} else if (strcmp(cmd, "print") == 0) {
		err = print(words);
	} else if (strcmp(cmd, "run") == 0) {
		err = run(words);
	} else if (strcmp(cmd, "quit") == 0) {
		err = quit(words);
	} else if (strcmp(cmd, "exec") == 0) {
		err = exec(words);
	} else {
		err = unknown("Command not found.");
	}

	return err;

}

int help(char *words[]) {
	//assume words[0] is "help"
	if (words[1] == NULL) {
		puts("help\tDisplays all the commands");
		puts("quit\tExits / terminates the shell with \"Bye!\"");
		puts("set VAR STRING\tAssigns a value to shell memory");
		puts("print VAR\tDisplays the STRING assigned to VAR");
		puts("run SCRIPT.TXT\tExecutes the file SCRIPT.TXT");
		puts("exec FILE.TXT...\tExecutes at most three files concurrently.");
		return 0;
	}

	return unknown("The 'help' command has 0 argument.");

}

int quit(char *words[]) {
	if (words[1] == NULL) {
		puts("Bye!");
		exit(0);
	}

	return unknown("The 'quit' command has 0 argument.");

}

int set(char *words[]) {
	if (words[1] != NULL && words[2] != NULL && words[3] == NULL) {
		return aset(words[1], words[2]);
	}

	return unknown("The 'set' command has 2 arguments.");
}

int print(char *words[]) {
	if (words[1] != NULL && words[2] == NULL) {
		return aprint(words[1]);
	}

	return unknown("The 'print' command has 1 argument.");
}

int run(char *words[]) {
	if (words[2] != NULL) {
		return unknown("The 'run' command only has 1 argument.");
	}

	FILE *fptr = fopen(words[1], "r+");
	char line[BUFSIZE];

	if (fptr == NULL) {
		return unknown("Script not found.");
	}

	while (fgets(line, BUFSIZE - 1, fptr) != NULL) {
		int i;
		for (i = 0; line[i] != '\n' && line[i] != '\0'; i++)
			;
		line[i] = '\0';

		if (parse(line) != 0) {
			fclose(fptr);
			return 1;
		}
	}

	fclose(fptr);
	return 0;
}

int exec(char *words[]) {
	//number of scripts
	int i = 0;
	for (; words[i + 1] != NULL; i++)
		;

	//if has at least one script
	if (i < 1) {
		return unknown("The 'exec' command should have at least 1 argument.");
	}
	if (i > 3) {
		return unknown("The 'exec' command can have at most 3 arguments.");
	}

	for (int j = 0; j < i; j++) {
		FILE *fptr = fopen(words[j + 1], "r+");
		int err;
		if ((err = launcher(fptr)) == 0) {
			return unknown("File Launched Failure.");
		}
	}

	scheduler();

	return 0;
}

int unknown(char *s) {
	printf("%s\n", s);
	return 1;
}

