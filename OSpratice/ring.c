#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define N 8
int count;
int final_result;
pthread_t init_id;
pthread_mutex_t mutex;

struct param{
	int data;
};

struct result{
	int final_result;
};

void *add_date(void *arg){
	int ptemp;
	pthread_mutex_lock(&mutex);
	count++;
	ptemp=(count+1)%N==0?8:(count+1)%N;
	pthread_mutex_unlock(&mutex);
	//sleep(1);
	struct param *param=(struct param*)arg;
	printf("线程%d的数据：%d\n",ptemp,param->data);
	pthread_t pthread_id;

	if(count<N){
        param->data++;
        pthread_create(&pthread_id,NULL,add_date,param);
        pthread_join(pthread_id,NULL);
        return NULL;
	}
	else{
		param->data++;
		//pthread_mutex_lock(&mutex);
	    final_result=param->data;
	    //pthread_mutex_unlock(&mutex);
	    return NULL;
	}
}

int main(){
	struct param pa;
	pa.data=1;
	struct result *result;
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&init_id,NULL,add_date,&pa);
	pthread_join(init_id,(void **)&result);
	return 0;
}