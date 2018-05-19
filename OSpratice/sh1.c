#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

#define Max_Cmd 128
#define Max_Dir_Name 128


int split_cmdstr(char *buf,char **argv)//字符分割，设置参数argv
{
	while(*buf==' '){buf++;}//忽略开头空格
	int index=0;
	int argc=0;
	while(*buf!='\n')
	{
		while(buf[index]!='\n'&&buf[index]!=' ')
		{
			index++;//当前字符不为空格和回车键时，下标后移
		}

		if(buf[index]=='\n')//到了回车键，命令结束
		{
			buf[index]='\0';
			argv[argc]=buf;
			argc++;
			break;
		}
		//经过之前判断 此时全空格跳出前一个while
		buf[index]='\0';
		argv[argc]=buf;
		buf=buf+index+1;
		index=0;
		argc++;

		while(*buf==' ')
		{
			buf++;
		}
	}
	argv[argc]=NULL;
	/*the last element is NULL*/
	argc=0;
/*	while(argv[argc]!=NULL)
	{
		printf("%s\n",argv[argc]);
		argc++;
	}
	*/
	return 0;

}

int buildin_cmdstr(char **argv)
{
	if(strcmp(argv[0],"exit")==0)
	{
		//printf("exit\n");
		exit(0);
	}
	if(strcmp(argv[0],"cd")==0)
	{
		if(chdir(argv[1])<0)
		{
			printf("%s:no such directory\n",argv[1]);
		}
		return 1;
	}
	else if(strcmp(argv[0],"pwd")==0)
	{
		char path[Max_Dir_Name];
		getcwd(path,Max_Dir_Name);
		printf("%s\n",path);
		return 1;
	}
	return 0;
}

void eval(char * cmdstring)
{
	char buf[Max_Cmd];
	char *argv[Max_Cmd];
	int i=0;
	for(i=0;i<Max_Cmd;i++)
		buf[i]='\0';
	strcpy(buf,cmdstring);
	split_cmdstr(buf,argv);
	if(argv[0]==NULL)
	{
		//printf("null\n");
		return;
	}
	if(buildin_cmdstr(argv))
	{
		//printf("buildin_cmdstr\n");
		return;
	}
	pid_t pid=fork();
	int status;
	if(pid==0)
	{
		int argc=0;
		while(argv[argc]!=NULL)
		{
			if(*argv[argc]=='>')
			{
				if(system(cmdstring)<0)
				{
					printf("%s:command incorrect.\n",argv[0]);
					exit(0);
				}
				else
					exit(0);
			}
			argc++;
		}
		if(execvp(argv[0],argv)<0)
		{
			printf("%s:command not found.\n",argv[0]);
			exit(0);
		}		
	}
	wait(&status);
}


int main(int argc, char *argv[])
{
	char cmdstring[Max_Cmd];
	int n;
	int i;
	while(1)
	{
		for(i=0;i<Max_Cmd;i++)
		cmdstring[i]='\0';
		printf("myshell>");
		fflush(stdout);

		if((n=read(0,cmdstring,Max_Cmd))<0)
		{
			printf("read error!\n");
		}
		else
		{
			eval(cmdstring);
		}
	}
	return 0;
}