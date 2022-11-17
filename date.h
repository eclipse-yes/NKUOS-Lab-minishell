//
// NKU OS Course
// Created by eclipse on 11/17/22.
//

#ifndef NKUOS_LAB9DEMO_DATE_H
#define NKUOS_LAB9DEMO_DATE_H

#include "main.h"

int date_builtin(int argc, char** argv);
void setthetime(char *p, const char *pformat);
char *strptime(const char *restrict s, const char *restrict format,
               struct tm *restrict tm);
void badformat(void );

#endif //NKUOS_LAB9DEMO_DATE_H
