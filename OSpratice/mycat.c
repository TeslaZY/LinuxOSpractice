#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
// cat [选项] [文件]
int fd;
char ch;
int main(int argc,char* argv[])
{
	if(argc==1||argc>2)
	{
		printf("Input wrong instruction!\ncat [file]");
		exit(1);
	}
	else
	{
		fd=open(argv[1],O_RDONLY);
		if(fd<0)
		{
			printf("Wrong file path!\n");
			exit(1);
		}		
	    while(read(fd,&ch,1))
			printf("%c",ch);
		printf("\n");
		close(fd);
	}	
	return 0;
}
