#include<stdio.h>
#include<hip/hip_runtime.h>

__host__ __device__ int add(int a, int b)
{
	return a+b;
}

__device__ int multiply(int a,int b)
{
	return a*b;
}

__global__ void calculate(int *result)
{
	int x=2,y=3;
	int sum=add(x,y); 
	int prod=multiply(x,y);
	result[0]=sum;
	result[1]=prod;
}

int main()
{
	int h_result[2];
	int *d_result;
	hipMalloc((void**)&d_result,2*sizeof(int));
	calculate<<<1,1>>>(d_result);
	hipMemcpy(h_result,d_result,2*sizeof(int),hipMemcpyDeviceToHost);
	printf("Sum (GPU): %d\n",h_result[0]);
	printf("Prod (GPU) :%d\n",h_result[1]);
	printf("Sum (CPU): %d\n",add(2,3));
	hipFree(d_result);
	return 0;
}

