#include<hip/hip_runtime.h>
#define BLOCK_SIZE 16
#define N 2048

__global__ void matrix_mul_gpu(int  *a, int *b, int *c, int n)
{
	int row=blockIdx.y * blockDim.y + threadIdx.y;
	int col=blockIdx.x * blockDim.x + threadIdx.x;
	int sum=0;
	if(col<n && row<n)
	{
		for(int i=0;i<n;i++)
		{
			sum+=a[row*n +i] * b[i*n+col];
		}
		c[row*n+col] =sum;
	}
}
void cpu_matrix_multiplication(int *h_a, int *h_b, int *h_result, int n) {
    for (int i = 0; i < n; ++i) 
    {
        for (int j = 0; j < n; ++j) 
        {
            int tmp = 0;
            for (int k = 0; k < n; ++k) 
            {
                tmp += h_a[i * n + k] * h_b[k * n + j];
            }
            h_result[i * n + j] = tmp;
        }
    }
}

int main()
{
	int *h_a,*h_b,*h_c,*h_cc;
	h_a=(int*)malloc(sizeof(int)*N*N);
	h_b=(int*)malloc(sizeof(int)*N*N);
	h_c=(int*)malloc(sizeof(int)*N*N);
	h_cc=(int*)malloc(sizeof(int)*N*N);

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			h_a[i*N+j] = rand()%RAND_MAX;
		}
	}
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			h_b[i*N+j]=rand()%RAND_MAX;
		}
	}
	int *d_a,*d_b,*d_c;
	hipMalloc((void**)&d_a,sizeof(int)*N*N);
	hipMalloc((void**)&d_b,sizeof(int)*N*N);
	hipMalloc((void**)&d_c,sizeof(int)*N*N);
	
	hipMemcpy(d_a,h_a,sizeof(int)*N*N,hipMemcpyHostToDevice);
	hipMemcpy(d_b,h_b,sizeof(int)*N*N,hipMemcpyHostToDevice);
	
	dim3 threadsPerBlock (BLOCK_SIZE,BLOCK_SIZE);
	int n_blocks = ceil(N/BLOCK_SIZE);
	dim3 blocksPerGrid (n_blocks,n_blocks);

	matrix_mul_gpu<<<blocksPerGrid,threadsPerBlock>>>(d_a,d_b,d_c,N);
	hipDeviceSynchronize();

	hipMemcpy(h_c,d_c,sizeof(int)*N*N,hipMemcpyDeviceToHost);
	
	cpu_matrix_multiplication(h_a,h_b,h_cc,N);
	bool error = false;
    	for (int i = 0; i < N*N; ++i) {
        	if (h_cc[i] != h_c[i]) {
            	printf("Error at index: %d\n", i);
            	error = true;
           	 break;
        	}
    	}

    if (!error) {
        printf("Matrix multiplication is correct!\n");
    }
	hipFree(d_a);
	hipFree(d_b);
	hipFree(d_c);
	free(h_a);
	free(h_b);
	free(h_c);
	free(h_cc);
	return 0;
}


