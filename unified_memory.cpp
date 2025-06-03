#include<iostream>
#include<hip/hip_runtime.h>
#define N 1000000000 // 1 billion elements

__global__ void vector_add(float *c,float *a,float *b,int n) //kernel 
{
	int tid = blockDim.x * blockIdx.x + threadIdx.x;
	if(tid < N)
	{
		c[tid] = a[tid] + b[tid];
	}
}

int main()
{
	float *a=NULL;
	float *b=NULL;
	float *c=NULL;
	hipMallocManaged(&a,sizeof(float)*N);//allows data be shared and accessible to both CPU and GPU using a single pointer.
	hipMallocManaged(&b,sizeof(float)*N);//allows data be shared and accessible to both CPU and GPU using a single pointer.
	hipMallocManaged(&c,sizeof(float)*N);//allows data be shared and accessible to both CPU and GPU using a single pointer.
	for(int i=0;i<N;i++)
	{
		a[i]= 1.0f;
		b[i]= 2.0f;
	}
	int blocksize=256;
	int gridsize=(N+blocksize-1)/blocksize;
	vector_add<<<gridsize,blocksize>>>(c,a,b,N);//kernel launch
	hipDeviceSynchronize();//sync between CPU and GPU
	printf("The first index of the resultant : c[0] = %f\n",c[0]);
	hipFree(a);//freeing memory
	hipFree(b);//freeing memory
	hipFree(c);//freeing memory
	return 0;
}
