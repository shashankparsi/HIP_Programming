#include<stdio.h>
#include<hip/hip_runtime.h>
#include<stdlib.h>
#include<iostream>
#define N 1000000000 // 1 billion elements
__global__ void vector_add(float *h_c,float *h_a,float *h_b,int n)  //vector_add kernel
{
	int tid = blockDim.x * blockIdx.x + threadIdx.x;
	if(tid < N)
		h_c[tid] = h_a[tid] + h_b[tid];
}
int main()
{
	float *h_a,*h_b,*h_c,*d_a,*d_b,*d_c; //host pointers and device pointers
	/*Allocating memory to the HOST*/
	h_a = (float *)malloc(sizeof(float) * N);
	h_b = (float *)malloc(sizeof(float) * N);
	h_c = (float *)malloc(sizeof(float) * N);
	for(int i =0; i<N; i++) //Assigning values to arrays
	{
		h_a[i]=1.0f;
		h_b[i]=2.0f;
	}
	/*Allocating memory to the DEVICE*/
	hipMalloc((void **)&d_a,sizeof(float) * N);
	hipMalloc((void **)&d_b,sizeof(float) * N);
	hipMalloc((void **)&d_c,sizeof(float) * N);
	float gpu_elapsed_time_ms;
        hipEvent_t start,stop;
        hipEventCreate(&start);
        hipEventCreate(&stop);
	/*copying the memory from HOST to DEVICE*/
	hipMemcpy(d_a,h_a,sizeof(float)*N,hipMemcpyHostToDevice);
	hipMemcpy(d_b,h_b,sizeof(float) *N,hipMemcpyHostToDevice);
	int block_size = 256;//no.of threads which will be executed in parallel
	int grid_size = (N + block_size-1)/block_size; //No.of blocks to be launched
	hipEventRecord(start,0); //capturing the start time
	vector_add<<<grid_size,block_size>>>(d_c,d_a,d_b,N); //launching the kernel
	hipEventRecord(stop,0);//capturing the end time
	hipEventSynchronize(stop);
	hipDeviceSynchronize();//sync between cpu and gpu
	hipMemcpy(h_c,d_c,sizeof(float)*N,hipMemcpyDeviceToHost);//copying back the memory from Device to HOST
	printf("The first index of resulting array is:%f\n",h_c[0]);
	hipEventElapsedTime(&gpu_elapsed_time_ms,start,stop);
	printf("\nTime elapsed on vector addition on GPU:%f ms.\n\n",gpu_elapsed_time_ms/1000);
	hipFree(d_a);//freeing the device memory
	hipFree(d_b);//freeing the device memory
	hipFree(d_c);//freeing the device memory
	free(h_a);//freeing the host memory
	free(h_b);//freeing the host memory
	free(h_c);//freeing the host memory
	return 0;
}
