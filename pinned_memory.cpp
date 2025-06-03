#include<stdio.h>
#include<hip/hip_runtime.h>
#include<stdlib.h>
#include<iostream>
#include<time.h>
#define N 1000000000

__global__ void vector_add(float *h_c,float *h_a,float *h_b,int n)
{
	int tid = blockDim.x * blockIdx.x + threadIdx.x;
	//printf("tid=%d\n",tid);
	if(tid < N)
	{
		h_c[tid] = h_a[tid] + h_b[tid];
	}
}

int main()
{
	float *h_a=NULL;
	float *h_b=NULL;
	float *h_c=NULL;

	float *d_a=NULL;
	float *d_b=NULL;
	float *d_c=NULL;

	//h_a = (float *)malloc(sizeof(float) * N);
	//h_b = (float *)malloc(sizeof(float) * N);
	//h_c = (float *)malloc(sizeof(float) * N);
	hipHostMalloc(&h_a,sizeof(float)*N);
	hipHostMalloc(&h_b,sizeof(float)*N);
	hipHostMalloc(&h_c,sizeof(float)*N);

	for(int i =0; i<N; i++)
	{
		h_a[i]=1.0f;
		h_b[i]=2.0f;
	}
	//hipMalloc((void **)&d_a,sizeof(float) * N);
	//hipMalloc((void **)&d_b,sizeof(float) * N);
	//hipMalloc((void **)&d_c,sizeof(float) * N);
	
	//hipMemcpy(d_a,h_a,sizeof(float)*N,hipMemcpyHostToDevice);
	//hipMemcpy(d_b,h_b,sizeof(float) *N,hipMemcpyHostToDevice);

	int block_size = 256;//no.of threads which will be executed in parallel
	int grid_size = (N + block_size-1)/block_size; //No.of blocks to be launched
	//vector_add<<<grid_size,block_size>>>(d_c,d_a,d_b,N);
	vector_add<<<grid_size,block_size>>>(h_c,h_a,h_b,N);
	hipDeviceSynchronize();
	//hipMemcpy(h_c,d_c,sizeof(float)*N,hipMemcpyDeviceToHost);
	printf("The first index of resulting array is:%f\n",h_c[0]);
	hipFree(d_a);
	hipFree(d_b);
	hipFree(d_c);
	hipHostFree(h_a);
	hipHostFree(h_b);
	hipHostFree(h_c);
	return 0;
}



