#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<math.h>

#define NR_TOTAL 1000000
#define NR_CPU 2
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param{
    double *array;
	int start;
	int end;
};

struct result{
    double sum;
};

void * compute(void * arg){
    struct param *param;
	struct result *result;
	double sum=0.0;
	int i=0;
	param = (struct param*)arg;
	for(i=param->start;i<param->end;i++){
	    sum=sum+param->array[i];
	}
    
	result=malloc(sizeof(struct result));
	result->sum=sum;
	return result;
}


int main(){
    pthread_t workers[NR_CPU];
	struct param params[NR_CPU];
	int i=0;
	double array[NR_TOTAL];
    //初始化数组
    for(i=0;i<NR_TOTAL;i++){
	    array[i]=pow((-1),i)*(1.0/(double)(2*i+1));
       // printf("%f\n",array[i]);
	}
 
    	for(i=0;i<NR_CPU;i++){
	    struct param *param;
        param=&params[i];
		param->array = array;
		param->start = i*NR_CHILD;
		param->end = (i+1)*NR_CHILD;
		pthread_create(&workers[i],NULL,compute,param);
 	}
	double sum=0.0;
 	for(i=0;i<NR_CPU;i++){
	    struct result *result;
		pthread_join(workers[i],(void **)&result);
		sum=sum+result->sum;
		free(result);
	}
    sum=sum*4.0;
	printf("圆周率：%.20lf\n",sum);
	return 0;
}
