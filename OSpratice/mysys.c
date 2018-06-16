#include <stdio.h>
#include </home/zyy/OSpratice/FileRW/mysys.h>
int main()
{
    printf("--------------------------------------------------\n");
    mysystem("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysystem("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}