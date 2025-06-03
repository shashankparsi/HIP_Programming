#include<hip/hip_runtime.h>
#include<vector>
#include<thread> //compile with flag -lpthread
__global__ void vec_add(float *A,float *B,float *C,int n)
{
	int id = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;
	if(id >= n)
	{
		return;
	}

	C[id] = A[id] + B[id];
}

void gpu_thread(int gpu_id,int length_per_gpu,float *A,float *B,float *C)
{
	hipSetDevice(gpu_id);
	int n=length_per_gpu;
	float gpu_elapsed_time_ms;
        hipEvent_t start,stop;
        hipEventCreate(&start);
        hipEventCreate(&stop);
	float *dA,*dB,*dC;
	hipMalloc(&dA,n*sizeof(float));
	hipMalloc(&dB,n*sizeof(float));
	hipMalloc(&dC,n*sizeof(float));
	hipMemcpy(dA,A,n*sizeof(float),hipMemcpyHostToDevice);
	hipMemcpy(dB,B,n*sizeof(float),hipMemcpyHostToDevice);
	int block_size=256;
	int grid_size = (n+block_size-1)/block_size;
	hipEventRecord(start,0);
	vec_add<<<grid_size,block_size>>>(dA,dB,dC,n);
	hipEventRecord(stop,0);
	hipEventSynchronize(stop);
        hipEventElapsedTime(&gpu_elapsed_time_ms,start,stop);
	hipDeviceSynchronize();
	printf("\nTime elapsed on vector addition on GPU:%d is %f ms.\n\n",gpu_id,gpu_elapsed_time_ms);
	hipMemcpy(C,dC,n*sizeof(float),hipMemcpyDeviceToHost);
	hipFree(dA);
	hipFree(dB);
	hipFree(dC);
}

int main()
{
	hipError_t err;
	int device_count;
	hipDeviceProp_t Props;
	hipGetDeviceCount(&device_count);
	uint64_t length=1024000000;
	int num_gpus=device_count;
	int length_per_gpu = length/num_gpus;
	std::vector<float>a,b,c;
	for(int i=0;i<length;i++)
	{
		a.push_back((float)rand()/(float)RAND_MAX);
		b.push_back((float)rand()/(float)RAND_MAX);
	}
	c.resize(length);
	std::vector<std::thread>threads;
	for(int i=0;i<num_gpus;i++)
	{
		threads.push_back(std::thread(gpu_thread,i,length_per_gpu,a.data() + i * length_per_gpu,b.data()+i*length_per_gpu,c.data()+i*length_per_gpu));
	}
	for(int i=0;i<num_gpus;i++)
	{
		threads[i].join();
	}
	return 0;
}

