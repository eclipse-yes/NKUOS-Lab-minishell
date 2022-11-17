//
// NKU OS Course
// Created by Zhang Shuhao on 11/17/22.
//

#include "cd.h"

extern char previous[MAXLINE];
extern char curDir[PWDLENG];

void cd_builtin(int argc, char* argv[MAXARGS]) {
    //chdir(argv[1]);
    // command type: cd [...]

    struct passwd *username;
    username = getpwuid(getuid());

    if (argc == 1) {  /* cd: change directory to /home/username */
        if (!strcmp(username->pw_name, "root")) {
            strcpy(previous, curDir);
            #ifdef CD_DEBUG
            printf("previous: %s\n", previous);
            #endif
            chdir("/root");
        } else {
            char home[MAXLINE];
            sprintf(home, "/home/%s", username->pw_name);
            strcpy(previous, curDir);
            #ifdef CD_DEBUG
            printf("previous: %s\n", previous);
            #endif
            chdir(home);
        }
    }

    if (argc == 2) {
        if (!strcmp(argv[1], "~")) {
            char home[MAXLINE];
            sprintf(home, "/home/%s", username->pw_name);

            strcpy(previous, curDir);
            #ifdef CD_DEBUG
            printf("previous: %s\n", previous);
            #endif
            chdir(home);
        } else if (!strcmp(argv[1], "-")) {
            char temp[MAXLINE];
            strcpy(temp, curDir);

            #ifdef CD_DEBUG
            printf("previous: %s\n", previous);
            #endif
            chdir(previous);
            strcpy(previous, temp);
        } else {
            #ifdef CD_DEBUG
            printf("previous: %s\n", previous);
            #endif
            chdir(argv[1]);
            strcpy(previous, argv[1]);
        }
    }
}
