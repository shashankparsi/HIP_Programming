#include<stdio.h>  
#include<hip/hip_runtime.h>
#define N 1000000000   
__global__ void vector_add(float *h_c,float *h_a,float *h_b,int n)
{
	int tid = blockDim.x * blockIdx.x + threadIdx.x;
	if(tid < N)
		h_c[tid] = h_a[tid] + h_b[tid];
}
int main()
{
	float *h_a,*h_b,*h_c;//HOST pointers
	float *d_a,*d_b,*d_c;//DEVICE pointers
	//HOST memory allocation
	h_a = (float *)malloc(sizeof(float) * N);
	h_b = (float *)malloc(sizeof(float) * N);
	h_c = (float *)malloc(sizeof(float) * N);
	for(int i =0; i<N; i++) //Assigning values
	{
		h_a[i]=1.0f;
		h_b[i]=2.0f;
	}
	//DEVICE memory allocation
	hipMalloc((void **)&d_a,sizeof(float) * N);
	hipMalloc((void **)&d_b,sizeof(float) * N);
	hipMalloc((void **)&d_c,sizeof(float) * N);
	float gpu_elapsed_time_ms;
	hipEvent_t start,stop;
	hipEventCreate(&start);
	hipEventCreate(&stop);
	hipMemcpy(d_a,h_a,sizeof(float)*N,hipMemcpyHostToDevice);//memcpy b/w HOST and DEVICE
	hipMemcpy(d_b,h_b,sizeof(float) *N,hipMemcpyHostToDevice);//memcpy b/w HOST and DEVICE
	int block_size = 256;//no.of threads which will be executed in parallel
	int grid_size = (N + block_size-1)/block_size; //No.of blocks to be launched
	hipEventRecord(start,0);//starting of the event record
	vector_add<<<grid_size,block_size>>>(d_c,d_a,d_b,N);
	hipEventRecord(stop,0);//stopping of the event record
	hipEventSynchronize(stop);//waiting for event to complete
	hipDeviceSynchronize();//sync b/w HOST and DEVICE
	hipMemcpy(h_c,d_c,sizeof(float)*N,hipMemcpyDeviceToHost);//memcpy from DEVICE to HOST
	hipEventElapsedTime(&gpu_elapsed_time_ms,start,stop);//elapsed time b/w two events
	printf("\nTime elapsed on vector addition on GPU:%f ms.\n\n",gpu_elapsed_time_ms);
	printf("The first index of resulting array is:%f\n",h_c[0]);
	//Freeing DEVICE memory
	hipFree(d_a);
	hipFree(d_b);
	hipFree(d_c);
	//Freeing HOST memory
	free(h_a);
	free(h_b);
	free(h_c);
	return 0;
}



