#include<hip/hip_runtime.h>
#include<stdio.h>
#include<stdlib.h>

__device__ int get_global_id(void) //should be called only from GPU kernel and executes on device
{
	return blockIdx.x * blockDim.x + threadIdx.x;
}


__global__ void gpu_hello()  //kernel which executes on GPU
{
	int id = get_global_id();
	printf("Hello world from thread: %d\n",id);  //result from GPU
}

__host__ int add_num(int a, int b) //executes only on CPU
{
	return a+b;
}

int main()
{
	int res=add_num(1,2); //perform computation on CPU
	printf("result from CPU =%d\n",res); //result from CPU
	gpu_hello<<<4,4>>>(); //<<<GridSize,BlockSize>>>
	hipDeviceSynchronize();//host waits for all commands in all streams on the specified device to complete
	return 0;
}
