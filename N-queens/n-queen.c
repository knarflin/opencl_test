#include <stdio.h>
#include <stdlib.h>
 
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MAX_SOURCE_SIZE (0x100000)
#define MAX_STRING_LEN (0x10000)	//Dangerous !! Could be longer!

inline int checkerror( cl_int cl_errno, int line_num ){
	if( cl_errno != 0 ){
		printf("cl_errno = %d, line %d\n", cl_errno, line_num );
		exit(0);
	}
	return 0;
}
 
int main(void)
{
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_mem memobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	 
	FILE *fp;
	char fileName[] = "./n-queen.cl";
	char *source_str;
	size_t source_size;
	 
	int num_queen;
	printf("num_queen = \n");
	scanf("%d", &num_queen );

	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	 
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	checkerror( ret, __LINE__ );
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
	checkerror( ret, __LINE__ );
	
	printf("Host: number of devices = %d\n", ret_num_devices);//debug point
	 
	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	checkerror( ret, __LINE__ );
	 
	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	checkerror( ret, __LINE__ );
	 
	/* Create Memory Buffer */
	int input[1] = { num_queen };
	int mutex[1] = { -1 };
	cl_mem inputBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, (1) * sizeof(int), (void *)input, &ret);
	cl_mem inputMutex = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, (1) * sizeof(int), (void *)mutex, &ret);
	checkerror( ret, __LINE__ );
	 
	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	checkerror( ret, __LINE__ );
	 
	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	//checkerror( ret, __LINE__ );

	cl_build_status ret_build_status;
	char* ret_build_log = (char *)malloc( sizeof(char) * MAX_STRING_LEN );
	ret = clGetProgramBuildInfo( program, device_id, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &ret_build_status, NULL );
	checkerror( ret, __LINE__ );
	ret = clGetProgramBuildInfo( program, device_id, CL_PROGRAM_BUILD_LOG, MAX_STRING_LEN, ret_build_log, NULL );
	checkerror( ret, __LINE__ );
	fputs( ret_build_log, stdout );
	 
	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "nqueen", &ret);
	checkerror( ret, __LINE__ );
	 
	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputBuffer );
	ret = clSetKernelArg(kernel, 1, sizeof(int) * num_queen, NULL);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&inputMutex );
	checkerror( ret, __LINE__ );
	 
	/* Execute OpenCL Kernel */
	size_t global_work_size[1] = { num_queen };
	size_t local_work_size[1] = {1};
	ret = clEnqueueNDRangeKernel( command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	checkerror( ret, __LINE__ );
	 
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(memobj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	 
	free(source_str);
	 
	return 0;
}
