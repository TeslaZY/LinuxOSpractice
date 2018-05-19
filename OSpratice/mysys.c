#include <stdio.h>
#include </home/zyy/OSpratice/mysys.h>
int main()
{
    printf("--------------------------------------------------\n");
    mysystem("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    printf("--------------------------------------------------\n");
    mysystem("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}