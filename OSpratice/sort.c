#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
# include<time.h>

#define NR_TOTAL 30
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param{
	int *array;
	int start;
	int end;
};



void * select_sort(void *arg){
	struct param *param;
	param = (struct param *)arg;
	
	int i,j;
	int temp=0;
	for(i=param->start;i<param->end;i++){
        for(j=i;j<param->end;j++){
        	if(param->array[i]>param->array[j]){
        		temp=param->array[i];
        		param->array[i]=param->array[j];
        		param->array[j]=temp;
        	}
        }   
	}

	return param;
}

void merge_sort(int *array,int start,int end){
	int *temp = malloc(sizeof(int)*(end-start+1));//start end 均为下标
	int mid=(start+end)/2;
    int m=start,n=mid+1,k=0;
	while(m<=mid&&n<=end){
		if(array[m]<=array[n]){
			temp[k]=array[m];
			m++;
			k++;
		}
		else{
			temp[k]=array[n];
			k++;
			n++;
		}
	}
	while(m<=mid){
        temp[k]=array[m];
        k++;
        m++;
	}
	while(n<=end){
        temp[k]=array[n];
        k++;
        n++;
	}
	for(m=start,k=0;m<=end;k++,m++){
		array[m]=temp[k];
	}
	free(temp);
}

int main(){
	pthread_t worker_id;
	struct param param[NR_CPU];
	int i;
	int *array;
	array=malloc(sizeof(int)*NR_TOTAL);
	srand((unsigned)time(NULL));
	for(i=0;i<NR_TOTAL;i++){
		array[i]=rand()%(NR_TOTAL-0+1);
		printf("%d  ",array[i]);
	}
	printf("\n");

	for (i = 0; i < NR_CPU; i++)
	{
		param[i].array=array;
		param[i].start=i*NR_CHILD;
		param[i].end=(i+1)*NR_CHILD;
	}

    pthread_create(&worker_id, NULL, select_sort, &param[1]);

	select_sort((void *)&param[0]);
	pthread_join(worker_id,(void **)&param[1]);
	
    merge_sort(array,0,NR_TOTAL-1);

	for(i=0;i<NR_TOTAL;i++)
		printf("%d  ",array[i]);
	printf("\n");
	return 0;
}