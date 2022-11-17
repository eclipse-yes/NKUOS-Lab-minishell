//
// NKU OS Course
// Created by eclipse on 11/17/22.
//

#include "date.h"
#include <time.h>

//int date_builtin(int argc, char* argv[MAXARGS]) {
//    printf("hello");
//}

extern time_t tval;
extern int slidetime;
extern int jflag;

int date_builtin(int argc, char* argv[MAXARGS]) {
    const char *errstr;
    struct tm *tp;
    int ch, rflag;
    char *format, buf[1024], *outzone = NULL;
    const char *pformat = NULL;
    long long val;

    rflag = 0;
    jflag = 1;
    slidetime = 1;

    while ((ch = getopt(argc, argv, "af:jr:uz:")) != -1)
        switch(ch)
        {
            case 'a':
                slidetime = 1;
                break;
            case 'f':  /* 用 strptime 解析  */
                pformat = optarg;
                break;
            case 'j':  /* 不要设置 */
                jflag = 1;
                break;
            case 'r':  /* 用户指定的秒数 */
                rflag = 1;
                //tval = strtonum(optarg, LLONG_MIN, LLONG_MAX, &errstr);
                //if (errstr)
                // errx(1, "seconds is %s: %s", errstr, optarg);
                val = strtoll(optarg, (char** )&buf, 0);
                tval = (time_t)val;
                break;
            case 'u':  /* 在 UTC 中做所有事情  */
                if (setenv("TZ", "UTC", 1) == -1)
                    err(1, "cannot unsetenv TZ");
                break;
            case 'z':
                outzone = optarg;
                break;
            default:
                usage();
        }
    argc -= optind;
    argv += optind;

    if (!rflag && time(&tval) == -1)
        err(1, "time");
    /* 要格式化 date 命令的输出，您可以使用以 + 号开头的控制字符*/
    format = "%a %b %e %H:%M:%S %Z %Y";

    /* 允许任何顺序的操作数 */
    if (*argv && **argv == '+')
    {
        format = *argv + 1;
        argv++;
        argc--;
    }

    if (*argv)
    {
        setthetime(*argv, pformat);
        argv++;
        argc--;
    }

    if (*argv && **argv == '+')
    {
        format = *argv + 1;
        argc--;
    }

    if (argc > 0)
        errx(1, "too many arguments");

    if (outzone)
        setenv("TZ", outzone, 1);

    tp = localtime(&tval);
    if (tp == NULL)
        errx(1, "conversion error");
    strftime(buf, sizeof(buf), format, tp);
    printf("%s\n", buf);
    return 0;
}

#define ATOI2(ar) ((ar) += 2, ((ar)[-2] - '0') * 10 + ((ar)[-1] - '0'))

void setthetime(char *p, const char *pformat)
{
    struct tm *lt, tm;
    struct timeval tv;
    char *dot, *t;
    time_t now;
    int yearset = 0;



    lt = localtime(&tval);

    lt->tm_isdst = -1;   /* correct for DST */

    if (pformat)
    {
        tm = *lt;
        if (strptime(p, pformat, &tm) == NULL)
        {
            fprintf(stderr, "trouble %s %s\n", p, pformat);
            badformat();
        }
        lt = &tm;
    }
    else
    {
        for (t = p, dot = NULL; *t; ++t)
        {
            if (isdigit((unsigned char)*t))
                continue;
            if (*t == '.' && dot == NULL)
            {
                dot = t;
                continue;
            }
            badformat();
        }

        if (dot != NULL)
        {   /* .SS */
            *dot++ = '\0';
            if (strlen(dot) != 2)
                badformat();
            lt->tm_sec = ATOI2(dot);
            if (lt->tm_sec > 61)
                badformat();
        }
        else
            lt->tm_sec = 0;

        switch (strlen(p))
        {
            case 12:    /* cc */
                lt->tm_year = (ATOI2(p) * 100) - 1900;
                yearset = 1;
            case 10:    /* yy */
                if (!yearset) {
                    /* mask out current year, leaving only century */
                    lt->tm_year = ((lt->tm_year / 100) * 100);
                }
                lt->tm_year += ATOI2(p);
            case 8:     /* mm */
                lt->tm_mon = ATOI2(p);
                if ((lt->tm_mon > 12) || !lt->tm_mon)
                    badformat();
                --lt->tm_mon;   /* time struct is 0 - 11 */
            case 6:     /* dd */
                lt->tm_mday = ATOI2(p);
                if ((lt->tm_mday > 31) || !lt->tm_mday)
                    badformat();
            case 4:     /* HH */
                lt->tm_hour = ATOI2(p);
                if (lt->tm_hour > 23)
                    badformat();
            case 2:     /* MM */
                lt->tm_min = ATOI2(p);
                if (lt->tm_min > 59)
                    badformat();
                break;
            default:
                badformat();
        }
    }

    /* 将故障时间转换为 UTC 时钟时间  */
    if (pformat != NULL && strstr(pformat, "%s") != NULL)
        tval = timegm(lt);
    else
        tval = mktime(lt);
    if (tval == -1)
        errx(1, "specified date is outside allowed range");

    if (jflag)
        return;

    /* 设置时间  */
    if (slidetime)
    {
        if ((now = time(NULL)) == -1)
            err(1, "time");
        tv.tv_sec = tval - now;
        tv.tv_usec = 0;
        if (adjtime(&tv, NULL) == -1)
            err(1, "adjtime");
    }
    else
    {

        tv.tv_sec = tval;
        tv.tv_usec = 0;
        if (settimeofday(&tv, NULL))
            err(1, "settimeofday");

    }

    if ((p = getlogin()) == NULL)
        p = "???";
    syslog(LOG_AUTH | LOG_NOTICE, "date set by %s", p);
}

void badformat(void) {

}
