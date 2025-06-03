#include<hip/hip_runtime.h>
#define N 10000

struct TwoDimPoints {
	float x;
	float y;
	float distToOrigin;
};

__global__ void calculatedistToOrigin(TwoDimPoints *points,int n)
{
	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	if(idx < n)
	{
		points[idx].distToOrigin = sqrt(points[idx].x * points[idx].x + points[idx].y * points[idx].y);
	}
}

int main()
{
	TwoDimPoints *h_points;
	h_points = (TwoDimPoints *)malloc(sizeof(TwoDimPoints)*N);

	for(int i=0;i<N;i++)
	{
		h_points[i].x= rand()%RAND_MAX;
		h_points[i].y = rand()%RAND_MAX;
	}
	TwoDimPoints *d_points;
	hipMalloc((void **)&d_points,N*sizeof(TwoDimPoints));

	hipMemcpy(d_points,h_points,sizeof(TwoDimPoints)*N,hipMemcpyHostToDevice);
	int block_size=256;
	int Grid_size = (N+block_size-1)/block_size;
	calculatedistToOrigin<<<Grid_size,block_size>>>(d_points,N);
	hipDeviceSynchronize();
	hipMemcpy(h_points,d_points,sizeof(TwoDimPoints)*N,hipMemcpyDeviceToHost);

	for(int i=0;i<N;i++)
	{
		printf("Point %d: (%f, %f), distToOrigin = %f\n", i, h_points[i].x, h_points[i].y, h_points[i].distToOrigin);
	}
	hipFree(d_points);
	free(h_points);
	return 0;
}



