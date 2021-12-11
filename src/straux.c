/*
 * straux.c
 *
 *  Created on: Mar. 18, 2021
 *      Author: Wang Jingyu
 */

#include <stdio.h>
#include "straux.h"

//remove newline character in the string if exists
void rmnl(char *str) {
	int i = 0;
	for (; str[i] != '\0' && str[i] != '\n'; i++)
		;
	if (str[i] == '\n')
		str[i] = '\0';
}
