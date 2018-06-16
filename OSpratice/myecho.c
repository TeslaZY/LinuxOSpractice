#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define true 1;

int main(int argc,char *argv[])
{
    int i,fd;
    int flag=0;
    //printf("argc：%d\n",argc );
    int loc=0;
    for(i=1;i<argc;i++){
        if(strcmp(argv[i],">")==0){
            //printf(">：%d\n",i);
            flag=true;
            loc=i;
            break;
        }
    }
    //printf("loc:%d\n",loc);

    if(flag)
    {
        char * path=argv[loc+1];
        //fd=open(path,O_RDWR|O_APPEND|O_CREAT);
        fd=open(path,O_RDWR| O_TRUNC | O_CREAT);
        if(fd<0){
            printf("file open error\n");
            return -1;
        }
        char * record;
        int step=1;
        char *start,*end;
        while(step<loc)
        {
            start=end=argv[step];
            while(*end!='\0'){
                end++;
            }
            int length=end-start+1;
            //printf("length:%d\n",length);
            record = malloc(sizeof(char)*length);
            strcpy(record,argv[step]);
            //printf("record str:%s\n",record);
            //printf("argv str:%s\n",argv[step]);
            int count = write(fd,record,length);
            if(count<0){
                printf("write open error\n");
                return -1;
            }
            
            write(fd," ",1);
            free(record);
            step++;
        }

        write(fd,"\n",1);
            /*if(count<0){
                printf("write open error\n");
                return -1;
            }
            */
    }

    else{
        for(i=1;i<argc;i++)
        {
            //printf("argv[%d]:%s%s",i,argv[i],(i<argc-1)?"\n":"");
            printf("%s%s",argv[i],(i<argc-1)?" ":"\n");
        }       
    }
    return 0;
}
