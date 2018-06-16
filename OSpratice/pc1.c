#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CAPACITY 4 //共享缓存区

int buffer1[CAPACITY];
int buffer1_in;//C语言全局变量或者静态变量默认值为0
int buffer1_out;

int buffer2[CAPACITY];
int buffer2_in;//C语言全局变量或者静态变量默认值为0
int buffer2_out;

int buffer_is_empty(int bufferid){
	if(bufferid==1)
	    return buffer1_in==buffer1_out;
	else if(bufferid==2)
		return buffer2_in==buffer2_out;
}

int buffer_is_full(int bufferid){
	if(bufferid==1)
	    return (buffer1_in + 1)%CAPACITY==buffer1_out;
	else if(bufferid==2)
	    return (buffer2_in + 1)%CAPACITY==buffer2_out;
}

int get_item(int bufferid){
	int item;
	if(bufferid==1){
		item=buffer1[buffer1_out];
	    buffer1_out=(buffer1_out+1)%CAPACITY;
	    return item;
	}
	else if(bufferid==2){
		item=buffer2[buffer2_out];
		buffer2_out=(buffer2_out+1)%CAPACITY;
		return item;
	}
	
}

void put_item(int bufferid,int item){
	if(bufferid==1){
		buffer1[buffer1_in]=item;
        buffer1_in=(buffer1_in + 1)%CAPACITY;
	}
	else if(bufferid==2){
		buffer2[buffer2_in]=item;
        buffer2_in=(buffer2_in + 1)%CAPACITY;
	}
}

pthread_mutex_t mutex_buffer1;
pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_full_buffer1;

pthread_mutex_t mutex_buffer2;
pthread_cond_t wait_empty_buffer2;
pthread_cond_t wait_full_buffer2;


#define ITEM_COUNT (CAPACITY*2)

void *produce(void *arg){
    int i;
	int item;

	for(i=0;i<ITEM_COUNT;i++){
	    pthread_mutex_lock(&mutex_buffer1);
		while(buffer_is_full(1)){
		   	pthread_cond_wait(&wait_empty_buffer1,&mutex_buffer1);
    	}

	   	item='a'+i;
		put_item(1,item);
		printf("produce item:%c\n",item);
	   
	   	pthread_cond_signal(&wait_full_buffer1);
		pthread_mutex_unlock(&mutex_buffer1);
	}
	return NULL;
}

void *compute(void *arg){
	int i;
	int item1,item2;
    for(i=0;i<ITEM_COUNT;i++){
	    pthread_mutex_lock(&mutex_buffer1);
		while(buffer_is_empty(1)){
		   	pthread_cond_wait(&wait_full_buffer1,&mutex_buffer1);
		}
	   	item1=get_item(1);
	    pthread_cond_signal(&wait_empty_buffer1);
	    pthread_mutex_unlock(&mutex_buffer1);

	    pthread_mutex_lock(&mutex_buffer2);
		while(buffer_is_full(2)){
		   	pthread_cond_wait(&wait_empty_buffer2,&mutex_buffer2);
    	}
    	item2=item1+('A'-'a');
		put_item(2,item2);
		printf("    change %c to %c\n",item1,item2);
		pthread_cond_signal(&wait_full_buffer2);
		pthread_mutex_unlock(&mutex_buffer2);

	}

	return NULL;
}


void *consume(void *arg){
    int i;
	int item;
	for(i=0;i<ITEM_COUNT;i++){
	    pthread_mutex_lock(&mutex_buffer2);
		while(buffer_is_empty(2)){
		   	pthread_cond_wait(&wait_full_buffer2,&mutex_buffer2);

		}
	   	item=get_item(2);
		printf("        consume item: %c\n",item);

	pthread_cond_signal(&wait_empty_buffer2);
	pthread_mutex_unlock(&mutex_buffer2);
	}
    return NULL;
}


int main(){
	pthread_t consumer_tid;
	pthread_t computer_tid;
	pthread_t producer_tid;

   	pthread_mutex_init(&mutex_buffer1,NULL);
    pthread_cond_init(&wait_empty_buffer1,NULL);
    pthread_cond_init(&wait_full_buffer1,NULL);

    pthread_mutex_init(&mutex_buffer2,NULL);
    pthread_cond_init(&wait_empty_buffer2,NULL);
    pthread_cond_init(&wait_full_buffer2,NULL);

    pthread_create(&producer_tid,NULL,produce,NULL);
    pthread_create(&computer_tid,NULL,compute,NULL);
	pthread_create(&consumer_tid,NULL,consume,NULL);

	pthread_join(producer_tid,NULL);
	pthread_join(computer_tid,NULL);
	pthread_join(consumer_tid,NULL);
	return 0;
}
