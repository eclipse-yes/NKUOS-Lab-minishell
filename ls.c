//
// NKU OS Course
// Created by Zhang Shuhao on 11/17/22.
//

#include "ls.h"

extern char curDir[PWDLENG];

void ls_builtin(int argc, char* argv[MAXARGS]) {
    DIR   *dir;
    struct dirent  *dp;
    struct stat     statbuf;
    struct passwd  *pwd;
    struct group   *grp;
    struct tm      *tm;
    char            datestring[256];
    char            modestr[11];

    dir = opendir(curDir);
    if(NULL == dir)
    {
        printf("opendir is NULL\n");
        exit(EXIT_FAILURE);
    }
    /* 循环遍历目录条目 */
    while ((dp = readdir(dir)) != NULL)
    {
        /* 获取条目信息  */
        if (stat(dp->d_name, &statbuf) == -1)
            continue;

        /* 打印出链接的类型、权限和数量*/
        printf("%10.10s", getperm(modestr,statbuf));
        printf("%4lu", statbuf.st_nlink);

        /* 如果使用 getpwuid() 找到所有者的名称，则打印出所有者的名称。  */
        if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
            printf(" %-8.8s", pwd->pw_name);
        else
            printf(" %-8d", statbuf.st_uid);

        /* 如果使用 getgrgid() 找到组名，则打印出组名。 */
        if ((grp = getgrgid(statbuf.st_gid)) != NULL)
            printf(" %-8.8s", grp->gr_name);
        else
            printf(" %-8d", statbuf.st_gid);

        /* 打印文件的大小。  */
        printf(" %9jd", (intmax_t)statbuf.st_size);

        tm = localtime(&statbuf.st_mtime);

        /* 获取本地化的日期字符串。 */
        strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

        printf(" %s %s\n", datestring, dp->d_name);
    }
}

char * getperm(char * perm , struct stat fileStat) {


    if ( S_ISLNK(fileStat.st_mode) ) {
        perm[0] = 'l';
    }
    else if ( S_ISDIR(fileStat.st_mode) ) {
        perm[0] = 'd';
    }
    else if ( S_ISCHR(fileStat.st_mode) ) {
        perm[0] = 'c';
    }
    else if ( S_ISSOCK(fileStat.st_mode) ) {
        perm[0] = 's';
    }
    else if ( S_ISFIFO(fileStat.st_mode) ) {
        perm[0] = 'p';
    }
    else if ( S_ISBLK(fileStat.st_mode) ) {
        perm[0] = 'b';
    }
    else {
        perm[0] = '-';
    }
    perm[1] = (char )((fileStat.st_mode & S_IRUSR) ? 'r' : '-');
    perm[2] = (char )((fileStat.st_mode & S_IWUSR) ? 'w' : '-');
    perm[3] = (char )((fileStat.st_mode & S_IXUSR) ? 'x' : '-');
    perm[4] = (char )((fileStat.st_mode & S_IRGRP) ? 'r' : '-');
    perm[5] = (char )((fileStat.st_mode & S_IWGRP) ? 'w' : '-');
    perm[6] = (char )((fileStat.st_mode & S_IXGRP) ? 'x' : '-');
    perm[7] = (char )((fileStat.st_mode & S_IROTH) ? 'r' : '-');
    perm[8] = (char )((fileStat.st_mode & S_IWOTH) ? 'w' : '-');
    perm[9] = (char )((fileStat.st_mode & S_IXOTH) ? 'x' : '-');

    if ( fileStat.st_mode & S_ISUID ) {
        perm[3] = 's';
    }
    else if ( fileStat.st_mode & S_IXUSR ) {
        perm[3] = 'x';
    }
    else {
        perm[3] = '-';
    }

    if ( fileStat.st_mode & S_ISGID ) {
        perm[6] = 's';
    }
    else if ( fileStat.st_mode & S_IXGRP ) {
        perm[6] = 'x';
    }
    else {
        perm[6] = '-';
    }

    if ( fileStat.st_mode & S_ISVTX ) {
        perm[9] = 't';
    }
    else if ( fileStat.st_mode & S_IXOTH ) {
        perm[9] = 'x';
    }
    else {
        perm[9] = '-';
    }

    perm[10] = 0;

    return perm;
}
