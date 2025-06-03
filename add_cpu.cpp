/*c program to perform vector addition on CPU and calculating time took for function to execute*/
#include <iostream>
#define N 1000000000 // 1 billion numbers
#include <sys/time.h>
void cpu_vector_add(float *c,float *a,float *b,int n)  //vector add function
{
	for(int i=0;i<n;i++)
		c[i]= a[i]+ b[i];
}
int main()
{
	float *a=NULL;
	float *b=NULL;
	float *c=NULL;
	a = (float *)malloc(sizeof(float) * N); //memory allocation for pointer a
	b = (float *)malloc(sizeof(float) * N); //memory allocation for pointer b
	c = (float *)malloc(sizeof(float) * N); //memory allocation for pointer c
	for(int i=0;i<N;i++) //assigning the values to arrays
	{
		a[i]= 1.0f;
		b[i]= 2.0f;
	}
	struct timeval start, end;
    	unsigned long long secs_run, millis_run, micros_run;
	gettimeofday(&start, NULL);//start time
	cpu_vector_add(c,a,b,N); //calling the function
	gettimeofday(&end, NULL);//end time
	secs_run=(end.tv_sec - start.tv_sec); //calculating secs
	micros_run= ((secs_run*1000000) + end.tv_usec) - (start.tv_usec);//calculating micro secs
	millis_run=micros_run/1000; //calculating milli secs
	printf("The first index of resulting array is c[0] = %f\n",c[0]);
	printf("Elapsed duration in seconds:%llu\n",secs_run);
	printf("Elapsed duration in micro seconds:%llu\n",micros_run);
	printf("Elapsed duration in milli seconds:%llu\n",millis_run);
	free(a);//freeing the memory of pointer a
	free(b);//freeing the memory of pointer b
	free(c);//freeing the memory of pointer c
	return 0;
}
