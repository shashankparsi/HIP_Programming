#include<stdio.h>
#include<hip/hip_runtime.h>
#include<math.h>
#define N 999999999 //No.of rectangles

__global__ void calculate_pi(double *d_arr,double dx)
{
	int tid;
	double x,y;
	tid=blockIdx.x * blockDim.x + threadIdx.x;
	if(tid < N) //calculating area of one rectangle
	{
		x=tid*dx;
		y=sqrt(1-x*x);
		d_arr[tid]=y*dx;
	}
}


int main()
{
	int i;
	double area,pi;
	double dx;
	double *h_arr,*d_arr;
	h_arr =(double*) malloc(N*sizeof(double));
	hipMalloc((void**)&d_arr,N*sizeof(double));
	dx=1.0/N; //width of each rectangle
	area=0.0;
	hipMemcpy(d_arr,h_arr,N*sizeof(double),hipMemcpyHostToDevice);
	int block_size=256;
	int grid_size=(N+block_size-1)/block_size;
	calculate_pi<<<grid_size,block_size>>>(d_arr,dx);
	hipMemcpy(h_arr,d_arr,N*sizeof(double),hipMemcpyDeviceToHost);
	for(i=0;i<N;i++)
		area=area+h_arr[i];
	pi=4.0*area;
	printf("\nValue of PI is: %0.16f\n",pi);
	free(h_arr);
	hipFree(d_arr);
	return 0;
}

