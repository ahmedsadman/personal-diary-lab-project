// show the date-time of x days before

#include <stdio.h>
#include <time.h>
int main ()
{
    time_t rawtime;
    long long int ret;
    struct tm *info;
    time(&rawtime);
    int x;
    printf("x: ");
    scanf("%d", &x);

    rawtime -= x*86400; // the main magic
    info = localtime(&rawtime);
    ret = mktime(info);
    printf("%lld\n", ret/3600);

    printf ("%s", asctime(info));
    return 0;
}
