#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define M 10
#define N 128
#define L 256

void split(char source[L], char object[M][N]){
    char *token;
    char *buf;
    int i, j = 0;
    buf = source;
    while((token = strsep(&buf,"|")) != NULL){
        if(token[0] != '\0'){
            for(i = 0; token[i] != '\0'; i++){
                object[j][i] = token[i];
            }
            object[j][i] = '\0';
            j++;
         }
    }
}

void execute(char instruction[M][N]){
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    int i = 0;
    while(instruction[i][0] != '\0'){
        puts(instruction[i]);
        i++;
    }
    for(i = 0; instruction[i][0] != '\0'; i++){
        if(fork() == 0){
            if(i != 0){
                dup2(fd1[0], 0);
                close(fd1[0]);
                close(fd1[1]);
            }
            if(instruction[i + 1][0] != '\0'){
                dup2(fd2[1], 1);
                close(fd2[0]);
                close(fd2[1]);
            }
            system(instruction[i]);
            exit(0);
        }
        else{
            
            close(fd1[0]);
            close(fd1[1]);
            fd1[0] = fd2[0];
            fd1[1] = fd2[1];
            pipe(fd2);
        }
    }
    return ;
}

int main(){
    char input[L];
    char instruction[M][N] = {{'\0'}};
    printf("%s","myshell>>");
    fgets(input, L, stdin);
    input[strlen(input) - 1] = '\0';
    split(input,instruction);
    execute(instruction);
    return 0;
}