//
// NKU OS Course
// Created by eclipse on 11/17/22.
//

#ifndef NKUOS_LAB9DEMO_LS_H
#define NKUOS_LAB9DEMO_LS_H

#include "main.h"

void  ls_builtin(int, char** argv);      // current version: only -> ls -al
char* getperm(char*, struct stat);

#endif //NKUOS_LAB9DEMO_LS_H
