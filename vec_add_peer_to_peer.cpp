#include<vector>
#include<hip/hip_runtime.h>

__global__ void vec_add(float *a,float *b,float *c,int n)
{
	int i = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;
	if(i>=n)
		return;
	c[i] = a[i]+b[i];
}

int main()
{
	hipError_t err;
	int device_count;
	hipDeviceProp_t props;
	int memory_gpu=1;
	int calculation_gpu=2;
	int canAccessPeer=0;
	uint64_t length=102400;
	int block_size=256;
	int num_gpus=4;
	int length_per_gpu=length/num_gpus;
	int num_blocks_per_gpu = (length_per_gpu-1)/block_size+1;
	err=hipDeviceCanAccessPeer(&canAccessPeer,calculation_gpu,memory_gpu);
	if(err!=hipSuccess)
	{
		fprintf(stderr,"cant access peer device: %d\n",memory_gpu);
		exit(EXIT_FAILURE);
	}
	hipSetDevice(calculation_gpu);
	hipDeviceEnablePeerAccess(memory_gpu,0);
	std::vector<float>a,b,c;
	float *d_a,*d_b,*d_c;
	hipMalloc((void**)&d_a,length*sizeof(float));
	hipMalloc((void**)&d_b,length*sizeof(float));
	hipMalloc((void**)&d_c,length*sizeof(float));
	hipMemcpyHtoD(d_a,a.data(),length*sizeof(float));
	hipMemcpyHtoD(d_b,b.data(),length*sizeof(float));
	hipSetDevice(calculation_gpu);
	vec_add<<<num_blocks_per_gpu,block_size>>>(d_a,d_b,d_c,length);
	hipDeviceSynchronize();
	hipSetDevice(memory_gpu);
	hipMemcpyDtoH(c.data(),d_c,length*sizeof(float));
	hipFree(d_a);
	hipFree(d_b);
	hipFree(d_c);
	return 0;
}
