#include<stdio.h>
#include<hip/hip_runtime.h>

__global__ void divergence_kernel(int *output)
{
	int idx=threadIdx.x;
	if(idx%2==0)
	{
		output[idx]=idx+100;
	}
	else
	{
		output[idx]=idx*10;
	}
}


int main()
{
	const int N=64;
	int h_output[N];
	int *d_output;
	hipMalloc((void**)&d_output,N*sizeof(int));
	hipMemcpy(d_output,h_output,N*sizeof(int),hipMemcpyHostToDevice);
	hipLaunchKernelGGL(divergence_kernel,dim3(1),dim3(N),0,0,d_output);
	hipMemcpy(h_output,d_output,N*sizeof(int),hipMemcpyDeviceToHost);
	for(int i=0;i<N;i++)
	{
		printf("%d ",h_output[i]);
	}
	printf("\n");
	hipFree(d_output);
	return 0;
}
