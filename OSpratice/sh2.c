#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

#define LEN 256
#define WIDTH 256
#define HEIGHT 10
#define PRO_LEN 512

void pre_make_str(char source_cmd[],char *pre_source_cmd)
{
	printf("pre make str\n");
	int i=0,j=0;
	while(source_cmd[i]==' ')
		i++;
	pre_source_cmd[j]=source_cmd[i];
	i++;j++;

	while(source_cmd[i]!='\n')
	{
		if(source_cmd[i]=='>'||source_cmd[i]=='|')
		{
			pre_source_cmd[j]=' ';
			j++;
			pre_source_cmd[j]=source_cmd[i];
			i++;
			j++;
			pre_source_cmd[j]=' ';
			j++;
		}
		else
		{
			pre_source_cmd[j]=source_cmd[i];
			i++;j++;
		}
	}
	pre_source_cmd[j]=source_cmd[i];
	j++;
	pre_source_cmd[j]='\0';
	printf("%s",pre_source_cmd);
	printf("pre make str OVER\n");
}

void split_cmd(char source_cmd[],char dest_cmd[HEIGHT][WIDTH]){
	char *p;
	char *pre_source_cmd=malloc(sizeof(char)*PRO_LEN);
	pre_make_str(source_cmd,pre_source_cmd);
	p=strsep(&pre_source_cmd," ");
	int i=0,j=1;
	for(i=0;p[i]!='\0'&&p[i]!='\n';i++){
		dest_cmd[0][i]=p[i];
	}
	dest_cmd[0][i]='\0';
	while(p!=NULL){
		p=strsep(&pre_source_cmd," ");
		if(p&&*p!=0){
			printf("%d:%d-%s\n",j,p[0],p);
			for(i=0;p[i]!='\0'&&p[i]!='\n';i++){
				dest_cmd[j][i]=p[i];
			}
			dest_cmd[j][i]='\0';
			j++;
		}
	}
	dest_cmd[j][0]='\0';
	for(i=0;i<=j;i++){
		printf("%d:%s\n",i+1,dest_cmd[i]);
	}
}

int execute_cmd(char dest_cmd[HEIGHT][WIDTH]){
	pid_t pid;
	if (strcmp(dest_cmd[0], "echo") == 0) {
		pid = fork();
		if (pid == 0) 
		{
			int i, fd;
			int flag = 0;
			int loc = 0;

			for (i = 1; dest_cmd[i][0]!='\0'; i++) {
				if (dest_cmd[i][0]=='>') {
					flag = 1;
					loc = i;
					break;
				}
			}
			//printf("loc:%d\n",loc);

			if (flag)
			{
				char * path = dest_cmd[loc + 1];
				//fd=open(path,O_RDWR|O_APPEND|O_CREAT);
				fd = open(path, O_RDWR | O_TRUNC | O_CREAT);
				if (fd < 0) {
					printf("file open error\n");
					exit(0);
				}
				int step = 1;
				while (step < loc)
				{
					int count = write(fd,dest_cmd[step],strlen(dest_cmd[step]));
					if (count < 0) {
						printf("write open error\n");
						exit(0);
					}
					write(fd, " ", 1);
					step++;
				}

				write(fd, "\n", 1);
				exit(1);
			}

			else 
			{
				for (i = 1; dest_cmd[i][0]!='\0'; i++)
				{
					//printf("argv[%d]:%s%s",i,argv[i],(i<argc-1)?"\n":"");
					printf("%s ", dest_cmd[i]);
				}
				printf("\n");
				exit(1);
			}

		}
		else
		{
			int status;
			wait(&status);
			if (WIFEXITED(status)) 
			{
        		//printf("WIFEXITED = true\n"); 
        		if(WEXITSTATUS(status)==0)
        			return -1;
        		else if(WEXITSTATUS(status)==1)
        		    return  1;
    		}
		}
	}//if echo
    else if(strcmp(command)){

    }
	else
	{
		return 0;
	}
}

int main()
{
	char source_cmd[LEN];
	char dest_cmd[HEIGHT][WIDTH]={{'\0'}};
	while(1)
	{
		printf("myshell>>");
		fflush(stdout);

		if(read(0,source_cmd,LEN)<0)
		{
			printf("read error!\n");
		}
		else
		{
			split_cmd(source_cmd,dest_cmd);
			int status=execute_cmd(dest_cmd);
			if(status==0)
			{
				printf("%s:Command not found\n",dest_cmd[0]);
			}
		}
		int i;
		for(i=0;i<LEN;i++)//重置字符串
		{
			command[i]='\0';
		}
	}
	return 0;
}