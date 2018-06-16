#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include <math.h>
#define ENDNUMBER 10000000

double sub_result;
double master_result;

void *subthread(void *arg){
    long int i;
	sub_result=0.0;
	for(i=ENDNUMBER/2;i<ENDNUMBER;i++){
	    sub_result=sub_result+pow((-1),i)*(1.0/(double)(2*i+1));
	    //printf("sub_result%d:%lf\n",i,sub_result);
	}
	return NULL;	
}

void master(){
    long int i;
	master_result=0.0;
	for(i=0;i<ENDNUMBER/2;i++){
		master_result=master_result+pow((-1),i)*(1.0/(double)(2*i+1));
		//printf("master_result%d:%lf\n",i,master_result);
	}
}

int main(){

    pthread_t worker_tid;
	double total=0.0;
	pthread_create(&worker_tid,NULL,subthread,NULL);
	master();
	pthread_join(worker_tid,NULL);
	total=(master_result+sub_result)*4;
	printf("圆周率为：%.7lf\n",total);

	return 0;
}

