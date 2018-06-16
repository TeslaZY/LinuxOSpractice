#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

#define Max_Cmd 128
#define Max_Dir_Name 128

void pre_make_str(char * buf,char *temp_buf){
	//printf("pre_make_str\n");
	if(*buf=='\n'){
        *temp_buf='\n';
	}
	else
	{
	    char *c=buf,*p=temp_buf;
	    while(*c!='\n')
	    {
		    if(*c=='<'||*c=='>')
		    {
			    *p=' ';
			    p++;
			    *p=*c;
			    p++;
			    *p=' ';
			    c++;
			    p++;
		    }
		    else
		    {
                *p=*c;
                c++;
                p++;
		    }
	    }
	    *p=*c;
	    p++;
	    *p='\0';
        //printf("temp_buf:%s",temp_buf);
    }
    //printf("pre_make_str over\n");
}



int shell_cmd(char **argv){
	//printf("shell_cmd.\n",argv[0]);
	int error;
	if(strcmp(argv[0],"echo")==0){
		error = execv("./myecho", argv);
	}
	/*
	else if(strcmp(argv[0],"cat")==0){
		error = execvp("mycat",argv);
	}
	*/
	else {
        error = execvp(argv[0],argv);
	}
	if(error<0){
		//printf("error\n");
	    return -1;
	}
	else{
		return 1;
	}
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
			printf("No such directory：%s\n",argv[0]);
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

char buf_cmd[Max_Cmd*2];

int split_cmdstr(char *cmd_str,char **argv)//字符分割，设置参数argv
{
	//printf("split_cmdstr\n");
	//char buf_cmd[Max_Cmd*2];
	char *buf;
	pre_make_str(cmd_str,buf_cmd);
	buf=buf_cmd;
	//buf=cmd_str;
	int argc=0;
	char *buf_end;
	while(*buf==' '){buf++;}//忽略开头空格

	while(*buf!='\n')
	{
		buf_end=buf;
		while(*buf_end!='\n'&&*buf_end!=' ')
		{
			buf_end++;//当前字符不为空格和回车键时，下标后移
		}

		if(*buf_end=='\n')//到了回车键，命令结束
		{

			*buf_end='\0';
			argv[argc]=buf;
			argc++;
			break;
		}

		else if(*buf_end==' ')
		{//空格为字符串分界
		    *buf_end='\0';
		    argv[argc]=buf;
		    buf=buf_end+1;
		    argc++;
		}

		while(*buf==' ')
		{
			buf++;
		}
	}

	argv[argc]=NULL;
	/*the last element is NULL*/
	/*
	argc=0;	
    while(argv[argc]!=NULL)
	{
		printf("%s\n",argv[argc]);
		argc++;
	}
	//*/
	//printf("split_cmdstr over\n");
	return 0;

}


void eval(char * cmdstring)
{
	//char buf[Max_Cmd];
	char *argv[Max_Cmd];
	//int i=0;
	//for(i=0;i<Max_Cmd;i++)
	//	buf[i]='\0';
    split_cmdstr(cmdstring,argv);
	/*
	int argc=0;	
    while(argv[argc]!=NULL)
	{
		printf("%s\n",argv[argc]);
		argc++;
	}
	printf("first command:%s\n",argv[0]);
    //*/
    ///*
	if(argv[0]==NULL)
	{
		//printf("NULL command:%s\n",argv[0]);
		return;
	}
	else if(buildin_cmdstr(argv))
	{
		//printf("buildin command:%s\n",argv[0]);
		return;
	}
	else
	{
		pid_t pid=fork();
		int status;
		if(pid==0)
		{
			//printf("pid.\n",argv[0]);
			//printf("pif command:%s\n",argv[0]);
			if(shell_cmd(argv)<0)
			{
				printf("%s:command not found.\n",argv[0]);
				exit(0);
			}		
		}
		wait(&status);
	}
		//*/
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