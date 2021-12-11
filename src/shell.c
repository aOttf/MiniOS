/*
 ============================================================================
 Name        : shell.c
 Author      : Jacob W
 Version     :
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "interpreter.h"

int shellUI(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	printf("Welcome to the <%s> shell!\n", NAME);
	printf("Verson %1.1f Created %s\n", VER, DATE);

	char input[BUFSIZE];

	while (1) {
		if (isatty(STDIN_FILENO)) {
			putchar('$');
		}
		parse(s_gets(input));
	}
	return 0;

}

int parse(char *st) {
	int i = 0;
	int j = 0;
	int w = 0;
	char tmp[BUFSIZE];
	char *words[BUFSIZE];

	while (st[i] != '\0') {
		for (; st[i] == ' '; i++)
			;

		if (st[i] == '\0')
			break;

		for (; st[i] != ' ' && st[i] != '\0'; i++, j++) {
			tmp[j] = st[i];
		}

		tmp[j] = '\0';
		words[w++] = strdup(tmp);
		j = 0;
	}

	words[w] = NULL;

	return interpreter(words);
}

char* s_gets(char *st) {
	char *ret_val;
	int i = 0;
	ret_val = fgets(st, BUFSIZE - 1, stdin);
	if (ret_val) {
		for (; ret_val[i] != '\n' && ret_val[i] != '\0'; i++)
			;
		if (ret_val[i] == '\n') {
			ret_val[i] = '\0';
		} else {

			while (getchar() != '\n') {
				continue;

			}
		}
	}
	return ret_val;

}
