#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>


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
		    if(*c=='<'||*c=='>'||*c=='|')
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
	printf("shell_cmd:%s\n",argv[0]);
	int error;
	if(strcmp(argv[0],"echo")==0)
	{
		//error = execvp("echo", argv);
		int i,fd;
    	int flag=0;
    	//printf("argc：%d\n",argc );
    	i=0;
    	int loc=0;
    	while(argv[i]!=NULL){
        	if(strcmp(argv[i],">")==0){
            	//printf(">：%d\n",i);
            	flag=1;
            	loc=i;
           	 break;
       		}
       		else{
       			i++;
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
        	error=1;
            	/*if(count<0){
                	printf("write open error\n");
                	return -1;
            	}
            	*/
    	}
    	else
    	{
        	error = execvp("echo", argv);  
    	}
	}
	/*else if(strcmp(argv[0],"cat")==0){
		printf("Now in cat\n");
		pid_t pid=fork();
		int status;
		if(pid==0)
		{
			//printf("pid.\n",argv[0]);
			//printf("pif command:%s\n",argv[0]);
			char *argvv[]={"cat","<","log",NULL};
			if(execvp("cat",argvv)<0)
			{
				printf("%s:command error.\n",argv[0]);
				exit(0);
			}		
		}
		wait(&status);
		error=status;
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

int mysystem(const char * command)
{
    pid_t pid;
    int status;
    if(command==NULL)
    {
    	return (1);
    }
    if((pid=fork())<0)
    {
    	status=-1;//fork失败，返回-1
    }
    else if (pid==0)//在子进程当中
    {
    	execl("/bin/sh","sh","-c",command,NULL);
    	_exit(127); 
    	// exec执行失败,也就是没有装入函数，返回127，注意exec
    	//只在失败时才返回现在的进程,成功的话现在的进程就不存在
    }
    else //父进程
    {
    	while(wait(&status)<0)//在使用fork()之前调用wait(),wait()的返回    		                  
    	{                     //值则为-1,正常情况下wait()的返回值为子进程的PID
    		if(errno!=EINTR)
    		{
    			status=-1;
    			break;
    		}
    	}
    }
    return status;
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
			//mysystem(cmdstring);
		}
	}
	return 0;
}