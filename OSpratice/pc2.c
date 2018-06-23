#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define CAPACITY 4

int buffer1[CAPACITY];
int buffer1_in;
int buffer1_out;

int buffer2[CAPACITY];
int buffer2_in;
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

typedef struct{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sema_t;

void sema_init(sema_t *sema, int value){
	sema->value=value;
	pthread_mutex_init(&sema->mutex,NULL);
	pthread_cond_init(&sema->cond,NULL);
}

void sema_wait(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	sema->value--;
	while(sema->value<0)
		pthread_cond_wait(&sema->cond,&sema->mutex);
	pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema){
	pthread_mutex_lock(&sema->mutex);
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}

sema_t buffer1_mutex_sema;
sema_t empty_buffer1_sema;
sema_t full_buffer1_sema;

sema_t buffer2_mutex_sema;
sema_t empty_buffer2_sema;
sema_t full_buffer2_sema;

#define ITEM_COUNT (CAPACITY*2)

void *produce(void *arg){
	int i;
	int item;

	for(i=0;i<ITEM_COUNT;i++){
		sema_wait(&empty_buffer1_sema);
		sema_wait(&buffer1_mutex_sema);
        
        item = 'a'+i;
		put_item(1,item);
        sema_signal(&buffer1_mutex_sema);
        sema_signal(&full_buffer1_sema);
        printf("produce item：%c\n",item);
	}
    return NULL;
}

void *compute(void *arg){
	int i;
	int item1;
	int item2;
	for(i=0;i<ITEM_COUNT;i++){
		sema_wait(&full_buffer1_sema);
		sema_wait(&buffer1_mutex_sema);

		item1=get_item(1);

		sema_signal(&buffer1_mutex_sema);
		sema_signal(&empty_buffer1_sema);

		sema_wait(&empty_buffer2_sema);
		sema_wait(&buffer2_mutex_sema);
		item2=item1+('A'-'a');
		put_item(2,item2);
		sema_signal(&buffer2_mutex_sema);
        sema_signal(&full_buffer2_sema);
        printf("    change %c to %c\n",item1,item2);
	}
	return NULL;
}

void *consume(void *arg){
    int i;
    int item;

    for(i=0;i<ITEM_COUNT;i++){
    	sema_wait(&full_buffer2_sema);
    	sema_wait(&buffer2_mutex_sema);

    	item=get_item(2);
        
        sema_signal(&buffer2_mutex_sema);
        sema_signal(&empty_buffer2_sema);
        printf("        consume item：%c\n",item);
    }
    return NULL;
}


int main(){
	pthread_t consumer_tid;
	pthread_t computer_tid;
	pthread_t producer_tid;

	sema_init(&buffer1_mutex_sema,1);
	sema_init(&empty_buffer1_sema,CAPACITY-1);
	sema_init(&full_buffer1_sema,0);

	sema_init(&buffer2_mutex_sema,1);
	sema_init(&empty_buffer2_sema,CAPACITY-1);
	sema_init(&full_buffer2_sema,0);

	pthread_create(&computer_tid,NULL,consume,NULL);

	pthread_create(&consumer_tid,NULL,compute,NULL);

    pthread_create(&producer_tid,NULL,produce,NULL);
	
	pthread_join(computer_tid,NULL);	
	pthread_join(producer_tid,NULL);
	pthread_join(consumer_tid,NULL);
	return 0;
}