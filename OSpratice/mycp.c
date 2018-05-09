#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#define SIZE 8192
// cat [选项] [文件]
int main(int argc,char* argv[])
{
     int fdin,fdout;
	 int len;
	 char buf[SIZE];
	if(argc!=3)
	{
		printf("Input wrong instruction!\ncat [file]");
		exit(1);
	}
	else

	{
		fdin=open(argv[1],O_RDONLY);
		if(fdin<0)

		{
			printf("Wrong srcouce file path!\n");
			exit(1);
		}		
	    fdout=open(argv[2],O_CREAT|O_WRONLY|O_TRUNC,0666);
		while((len=read(fdin,buf,sizeof(buf)))>0)
		{
		    write(fdout,buf,len);
		}
		close(fdin);
		close(fdout);
	}	
	return 0;
}
