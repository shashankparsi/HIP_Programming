/* c program to perform matrix multiplication on CPU*/  
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#define N 2048
void cpu_matrix_mul(int *a,int *b,int *result,int n) //matrix mul function
{
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			int tmp=0;
			for(int k=0;k<n;++k)
			{
				tmp+=a[i*n +k] * b[k*n + j];
			}
			result[i*n+j]=tmp;
		}
	}
}
int main()
{
	struct timeval start,end;
	unsigned long long secs_run,milli_secs,micro_secs;
	int *a,*b,*c;
	a=(int *)malloc(sizeof(int)*N*N);
	b=(int *)malloc(sizeof(int)*N*N);
	c=(int *)malloc(sizeof(int)*N*N);
	for(int i=0;i<N;++i)
	{
		for(int j=0;j<N;++j)
			a[i*N +j]= rand()%RAND_MAX;
	}
	for(int i=0;i<N;++i)
	{
		for(int j=0;j<N;++j)
			b[i*N + j] = rand()%RAND_MAX;
	}
	gettimeofday(&start,NULL);
	cpu_matrix_mul(a,b,c,N); //function call
	gettimeofday(&end,NULL);
	secs_run = (end.tv_sec - start.tv_sec);
	printf("Elapsed duration in seconds:%llu\n",secs_run);
	micro_secs = ((secs_run*1000000) + end.tv_usec) - (start.tv_usec);
	printf("Elapsed duration in micro seconds:%llu\n",micro_secs);
	milli_secs = micro_secs/1000;
	printf("Elapsed duration in milli seconds:%llu\n",milli_secs);
	free(a);
	free(b);
	free(c);
	return 0;
}

