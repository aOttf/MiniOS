/*
 * shell.h
 *
 *  Created on: Jan. 22, 2021
 *      Author: Wang Jingyu
 */

#ifndef SHELL_H_
#define SHELL_H_

#define NAME "Jacob Wang"
#define VER 2.0
#define DATE "February 2021"
#define BUFSIZE 1000

//initialize shell
int shellUI(void);

//parses the user input
int parse(char*);

//reads the user input up to 999 characters and cleans the buffer
char* s_gets(char*);
#endif /* SHELL_H_ */
