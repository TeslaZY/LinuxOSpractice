#include<stdio.h>
int main(int argc,char *argv[])
{
    int i;
    for(i=1;i<argc;i++)
	{
		//printf("argv[%d]:%s%s",i,argv[i],(i<argc-1)?"\n":"");
		printf("%s%s",argv[i],(i<argc-1)?" ":"");
	}	
	printf("\n");
	return 0;
}
