#include<stdio.h>
#include<hip/hip_runtime.h>
#define N 10
__constant__ float coeff[5]; // declaration of constant memory
__global__ void compute_kernel(float *input,float *output)
{
	int idx=threadIdx.x + blockIdx.x * blockDim.x;
	if(idx<N){
		float sum = 0.0f;
		for(int j=0;j<5;j++) {
			sum+=coeff[j] * input[idx];
	}
	output[idx]=sum;
	}
}
int main()
{
	float h_input[N],h_output[N];
	float h_coeff[5]={1,2,3,4,5};
	for(int i=0;i<N;i++)
		h_input[i]=i+1;
	float *d_input,*d_output;
	hipMalloc((void**)&d_input,N*sizeof(float));
	hipMalloc((void**)&d_output,N*sizeof(float));
	hipMemcpy(d_input,h_input,N*sizeof(float),hipMemcpyHostToDevice);
	hipMemcpyToSymbol(coeff,h_coeff,5*sizeof(float)); //copy to constant memory
	hipLaunchKernelGGL(compute_kernel,dim3(1),dim3(N),0,0,d_input,d_output);
	hipMemcpy(h_output,d_output,N*sizeof(float),hipMemcpyDeviceToHost);
	printf("Output:\n");
	for(int i=0;i<N;i++)
		printf("%0.2f ",h_output[i]);
	printf("\n");
	hipFree(d_input);
	hipFree(d_output);
	return 0;
}

