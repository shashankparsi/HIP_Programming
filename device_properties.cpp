#include<stdio.h>
#include<hip/hip_runtime.h>

int main()
{
	int device_count;
	hipGetDeviceCount(&device_count);
	if(device_count == 0)
		printf("There is no HIP supporting device\n");
	int dev=0;
	for(dev=0;dev<device_count;dev++)
	{
		hipDeviceProp_t device_props;
		hipGetDeviceProperties(&device_props,dev);
		if(dev==0)
			if(device_props.major<1)
				printf("There is no HIP supporting device\n");
			else if(device_count == 1)
				printf("Available no.of devices: %d\n",device_count);
			else 
				printf("Available no.of devices:%d\n",device_count);
	printf("\nDevice %d: \"%s\"\n",dev,device_props.name);
	printf("Major Revision Number: %d\n",device_props.major);
	printf("Minor Revison Number: %d\n",device_props.minor);
	printf("Total amount of global memory: %ld bytes or %ld GB\n",device_props.totalGlobalMem,device_props.totalGlobalMem/(1024*1024*1024));
	printf("Total amount of constant memory: %ld bytes\n",device_props.totalConstMem);
	printf("Total amount of shared memory per block: %ld bytes\n",device_props.sharedMemPerBlock);
	printf("Total number of registers available per block: %d\n",device_props.regsPerBlock);
	printf("warp size: %d\n",device_props.warpSize);
	printf("MultiProcessor Count: %d\n",device_props.multiProcessorCount);
	printf("maximum threads per block: %d\n",device_props.maxThreadsPerBlock);
	printf("maximum sizes of each dimension of a block: %d x %d x %d\n",device_props.maxThreadsDim[0],device_props.maxThreadsDim[1],device_props.maxThreadsDim[2]);
	printf("maximum sizes of each dimension of a grid: %d x %d x %d\n",device_props.maxGridSize[0],device_props.maxGridSize[1],device_props.maxThreadsDim[2]);
	printf("maximum memory pitch: %ld bytes\n",device_props.memPitch);
	printf("Texture Alignment: %ld bytes\n",device_props.textureAlignment);
	printf("\n======================================================================\n");
	}
	return 0;
}

