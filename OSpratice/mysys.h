#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

/*
  1.fork一个子进程； 
  2.在子进程中调用exec函数去执行command； 
  3.在父进程中调用wait去等待子进程结束。 对于fork失败，
system()函数返回-1。 如果exec执行成功，也即command顺
利执行完毕，则返回command通过exit或return返回的值。
（注意，command顺利执行不代表执行成功，比如command：
"rm debuglog.txt"，不管文件存不存在该command都顺利执
行了） 如果exec执行失败，也即command没有顺利执行，比
如被信号中断，或者command命令根本不存在，system()函数
返回127. 如果command为NULL，则system()函数返回非0值，
一般为1. 
*/

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
