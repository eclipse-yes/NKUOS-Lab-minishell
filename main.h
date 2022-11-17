//
// NKU OS Course
// Created by Zhang Shuhao on 11/17/22.
//

#ifndef NKUOS_LAB9DEMO_MAIN_H
#define NKUOS_LAB9DEMO_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>

#define NDEBUG
#define CD_NDEBUG

/* Constants */
#define MAXLINE    1024
#define PWDLENG    128
#define MAXCMDS    128
#define MAXARGS    128
#define STRLENG    128
#define MAXLENARGV 128
#define EXITSIG    "exit"
/* End constants */

/* Function prototypes */
void usage(void);
void print_prompt(void);
void app_error(char*);
void eval(char*);
void pwd(void);
int  parseline(char*, char*[MAXCMDS][MAXARGS]);
void init_argcs(void);
int  eval_builtin(int, char**);
void pwd_builtin(void);
/* End function prototypes */

#endif //NKUOS_LAB9DEMO_MAIN_H
