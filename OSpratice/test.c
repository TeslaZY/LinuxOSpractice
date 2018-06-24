#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    int i=1;

    pid = fork(); 
    printf("pid = %d\n", pid); 
    if (pid == 0) 
        printf("In child process:%d\n",&i); 
    else
        printf("In parent process:%d\n",&i); 
    return 0;
}