#include<vector>
#include<hip/hip_runtime.h>

__global__ void average(float* a,float* b,int n)
{
	int i = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;
	if(i>=n)
		return;
	a[i] = (a[i] + b[i])/2;
}

int main()
{
	uint64_t length=1024;
	int num_gpu=4;
	std::vector<std::vector<float>>h_output(num_gpu);
	std::vector<float*>output_gpu(num_gpu);
	float* final_result;
	for(int i=1;i<num_gpu;i=i<<1)
	{
		for(int j=0;j<num_gpu;j=j+(i*2))
		{
			int leftgpu=j;
			int rightgpu=j+1;
			hipSetDevice(leftgpu);
			float *left_buf=output_gpu[leftgpu];
			float* right_buf;
			hipMalloc(&right_buf,(size_t)length*sizeof(float));
			hipMemcpyDtoD(right_buf,output_gpu[rightgpu],(size_t)length*sizeof(float));
			int block_size=256;
			int num_blocks=(length-1)/block_size+1;
			average<<<num_blocks,block_size>>>(left_buf,right_buf,length);
			hipFree(right_buf);
		}
	}
	return 0;
}

