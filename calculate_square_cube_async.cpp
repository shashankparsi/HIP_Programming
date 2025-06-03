#include<hip/hip_runtime.h>
#define N 1000000000

__global__ void squarekernel(float *input,float *output)
{
	int tid = blockDim.x * blockIdx.x + threadIdx.x;
	output[tid] = input[tid] * input[tid];
}

__global__ void cubekernel(float *input,float *output)
{
	int tid = blockDim.x * blockIdx.x + threadIdx.x;
	output[tid] = input[tid] * input[tid] * input[tid];
}

int main()
{
	float *h_input,*h_output_sq,*h_output_cube;
	float *d_input,*d_output_sq,*d_output_cube;

	h_input = (float *)malloc(sizeof(float) * N);
	h_output_sq = (float *)malloc(sizeof(float)*N);
	h_output_cube=(float *)malloc(sizeof(float)*N);

	for(int i=0;i<N;i++)
	{
		//h_input[i] = rand()%RAND_MAX;
		h_input[i]= 2.0f;
	}


	hipMalloc((void**)&d_input,sizeof(float)*N);
	hipMalloc((void**)&d_output_sq,sizeof(float)*N);
	hipMalloc((void**)&d_output_cube,sizeof(float)*N);

	hipStream_t stream1,stream2; //streams are used to execute the kernels concurrently
	hipStreamCreate(&stream1);
	hipStreamCreate(&stream2);
	hipMemcpyAsync(d_input,h_input,sizeof(float)*N,hipMemcpyHostToDevice,stream1);
	int blocksize=256;
	int gridsize=(N+blocksize-1)/blocksize;
	squarekernel<<<gridsize,blocksize,N*sizeof(float),stream1>>>(d_input,d_output_sq);
	hipMemcpyAsync(h_output_sq,d_output_sq,N*sizeof(float),hipMemcpyDeviceToHost,stream1);
	cubekernel<<<gridsize,blocksize,,stream2>>>(d_input,d_output_cube);
	hipMemcpyAsync(h_output_cube,d_output_cube,N*sizeof(float),hipMemcpyDeviceToHost,stream2);
	hipStreamSynchronize(stream1);
	printf("Resultant of suqarekernel first index:%0.2f\n",h_output_sq[0]);
	hipStreamSynchronize(stream2);
	printf("Resultant of cubekernel first index:%0.2f\n",h_output_cube[0]);
	hipFree(d_input);
	hipFree(d_output_sq);
	hipFree(d_output_cube);
	free(h_input);
	free(h_output_sq);
	free(h_output_cube);
	return 0;
}



