// NKU OS Course
// Author: Zhang Shuhao
#include "main.h"
#include "ls.h"
#include "cd.h"

/* Global variables */
char prompt[]        = "\033[01;32m%s@%s\033[00m:\033[01;34m%s\033[00m %s ";
                                       // format string for print out prompt line
char user[STRLENG]   = "zsh";          // username
char host[STRLENG]   = "ubuntu2022";   // hostname
char curDir[PWDLENG] = "/home/zsh";    // current directory
int  verbose         = 0;              // if true, print additional output
int  argcs[MAXCMDS];
char previous[MAXLINE];
/* End global variables */

/*
* main - the shell's main routine
*/
int main(int argc, char** argv) {
    char c;
    char cmdline[MAXLINE];
    // pwd();

    /* Parse the command line options */
    while ((c = (char )getopt(argc, argv, "hv")) != EOF) {
        switch (c) {
            case 'h':      /* print help message */
                usage();
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                usage();
        }
    }

    /* Execute the mini-shell's read/eval loop */
    while (1) {

        /* Read the command line */
        print_prompt();
        fflush(stdout);
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
            app_error("fgets error");
        if (feof(stdin)) {          /* End of file (user presses ctl-d) */
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }
        if (verbose) {
            printf("[log] user input: %s", cmdline);
        }

        /* Evaluate the command line */
        eval(cmdline);
        fflush(stdout);
    }

    return 0;
}

/*
* usage - print a help message
*/
void usage(void) {
    printf("Usage: mini-shell [-h]\n");
    printf("   -h  print this message\n");
    printf("   -v  print additional output\n");
    exit(EXIT_FAILURE);
}

/*
* print_prompt - print prompt information into stdout
*/
void print_prompt(void) {
    pwd();
    printf(prompt, user, host, curDir, "$");
}

/*
* app_error - application-style error routine
*/
void app_error(char* msg) {
    fprintf(stdout, "%s\n", msg);
    exit(EXIT_FAILURE);
}

/*
* pwd - get the current working directory and store string into curDir
*/
void pwd(void) {
    getcwd(curDir, sizeof(curDir));
}

/*
* eval - evaluate the command line that the user has just typed in
*
* If the user has requested a built-in command (quit, cd, pwd, ls)
* then execute it immediately. Otherwise, fork a child process and
* run the job in the context of the child. If the job is running in
* the foreground, wait for it to terminate and then return.  Note:
* each child process must have a unique process group ID so that our
* background children don't receive SIGINT (SIGTSTP) from the kernel
* when we type ctrl-c (ctrl-z) at the keyboard.
*/
void eval(char* cmdline) {
    if (verbose)
        printf("[eval] command line: %s", cmdline);

    int num_cmds = 0;
    // char** cmds[MAXCMDS];
    char* cmds[MAXCMDS][MAXARGS];
    init_argcs();
    cmdline[strlen(cmdline) - 1] = '\0';  // delete the end: '\n' char
    num_cmds = parseline(cmdline, cmds);

    #ifdef DEBUG
    printf("num_cmds = %d\n", num_cmds);
    printf("cmds[0][0] = %s\n", cmds[0][0]);
    printf("argcs[0] = %d\n", argcs[0]);
    #endif

    for (int i = 0; i < num_cmds; i++) {
        if (!eval_builtin(argcs[i], cmds[i])) {  /* If the cmd is not built-in */

        }
    }
}

/*
* parseline - parse the inputted cmdline from user
*
* parameters:
*   cmdline  - inputted string
*   cmds     - trimmed commands           e.g. [["ls", "-l", "-a"], ["ls"], ["pwd"]]
* return:
*   num_cmds - the number of cmds list
* global:
*   argcs    - the number of command args e.g. [3, 1, 1]
*/
int parseline(char* cmdline, char* cmds[MAXCMDS][MAXARGS]) {
    char* cmds0[MAXARGS];
    int numcmds = 0;

    char* cmd = strtok(cmdline, ";");
    while (cmd != NULL) {
        cmds0[numcmds] = cmd;
        if (verbose) {
            printf("[parseline] cmd = [%s]\n", cmd);
        }

        numcmds++;
        cmd = strtok(NULL, ";");
    }

    // int argcs[MAXCMDS];
    for (int i = 0; i < numcmds; i++) {
        int argc = 0;
        char* buf = strtok(cmds0[i], " ");
        while (buf != NULL) {
            if (!strcmp(buf, "\t")) continue;
            cmds[i][argc] = buf;

            argc++;
            buf = strtok(NULL, " ");
        }
        argcs[i] = argc;
    }

    return numcmds;
}

/*
* init_argcs - set all elements of argcs to 0
*/
void init_argcs(void) {
    for (int i = 0; i < MAXCMDS; i++) {
        argcs[i] = 0;
    }
}

int eval_builtin(int argc, char* argv[MAXARGS]) {
    if (!strcmp(argv[0], EXITSIG)) {
        exit(EXIT_SUCCESS);
    } else if (!strcmp(argv[0], "pwd")) {
        pwd_builtin();
    } else if (!strcmp(argv[0], "ls")) {
        ls_builtin(argc, argv);
    } else if (!strcmp(argv[0], "cd")) {
        cd_builtin(argc, argv);
    } else {
        return 0;  // argv is not a built-in command
    }
    return 1;
}

void pwd_builtin(void) {
    pwd();
    printf("%s\n", curDir);
}
