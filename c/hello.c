#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

typedef struct tagHello
{
	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;
	cl_mem memobj;
	cl_program program;
	cl_kernel kernel;
	cl_platform_id platform_id;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
} Hello_t;

char *loadSource(size_t *source_size)
{
	/* Load the source code containing the kernel*/
	FILE *fp;
	char fileName[] = "./hello.cl";
	char *source_str;
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}

	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	*source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	fprintf(stdout, "Load Source code (%zd)\n", *source_size);
	return source_str;
}

void createContext(Hello_t *m)
{
	m->device_id = NULL;
	m->context = NULL;
	m->command_queue = NULL;
	m->memobj = NULL;
	m->program;
	m->kernel;
	m->platform_id;
	m->ret_num_devices;
	m->ret_num_platforms;
	m->ret;

	/* Get Platform and Device Info */
	m->ret = clGetPlatformIDs(1, &m->platform_id, &m->ret_num_platforms);
	m->ret = clGetDeviceIDs(m->platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &m->device_id, &m->ret_num_devices);
	fprintf(stdout, "clGetDeviceIDs ret_num_devices = %d, for device_id = %p\n", m->ret_num_devices, (int*)m->device_id);
	 
	/* Create OpenCL context */
	m->context = clCreateContext(NULL, 1, &m->device_id, NULL, NULL, &m->ret);
}

void createQueue(Hello_t *m)
{
	/* Create Command Queue */
	m->command_queue = clCreateCommandQueue(m->context, m->device_id, 0, &m->ret);
	//m->command_queue = clCreateCommandQueueWithProperties(m->context, m->device_id, 0, &m->ret);
}

void createBuffer(Hello_t *m)
{
	/* Create Memory Buffer */
	m->memobj = clCreateBuffer(m->context, CL_MEM_READ_WRITE,MEM_SIZE * sizeof(char), NULL, &m->ret);
}

void createKernel(Hello_t *m, char *source_str, size_t source_size)
{
	/* Create Kernel Program from the source */
	m->program = clCreateProgramWithSource(m->context, 1, (const char **)&source_str,
			(const size_t *)&source_size, &m->ret);
	 
	fprintf(stdout, "Build Kernel Program\n");

	/* Build Kernel Program */
	m->ret = clBuildProgram(m->program, 1, &m->device_id, NULL, NULL, NULL);
	 
	/* Create OpenCL Kernel */
	m->kernel = clCreateKernel(m->program, "hello", &m->ret);
	 
	/* Set OpenCL Kernel Parameters */
	m->ret = clSetKernelArg(m->kernel, 0, sizeof(cl_mem), (void *)&m->memobj);
}

void enqueueKernel(Hello_t *m)
{
	const size_t *global_work_offset = NULL;
	const size_t global_work_size[1] = {1};
	const size_t local_work_size[1] = {1};

	/* Execute OpenCL Kernel */
	//ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
	m->ret = clEnqueueNDRangeKernel(m->command_queue, m->kernel, 1, global_work_offset, 
			global_work_size, local_work_size, 0, NULL, NULL);
}

void copyResults(Hello_t *m, char *string)
{
	/* Copy results from the memory buffer */
	m->ret = clEnqueueReadBuffer(m->command_queue, m->memobj, CL_TRUE, 0,
			MEM_SIZE * sizeof(char),string, 0, NULL, NULL);
}

void destroyContext(Hello_t *m)
{
	/* Finalization */
	m->ret = clFlush(m->command_queue);
	m->ret = clFinish(m->command_queue);
	m->ret = clReleaseKernel(m->kernel);
	m->ret = clReleaseProgram(m->program);
	m->ret = clReleaseMemObject(m->memobj);
	m->ret = clReleaseCommandQueue(m->command_queue);
	m->ret = clReleaseContext(m->context);
}

int main()
{
	Hello_t m;
	 
	char string[MEM_SIZE];
	 
	char *source_str = NULL;
	size_t source_size = 0;

	source_str = loadSource(&source_size);

	createContext(&m);	
	 
	createQueue(&m);

	createBuffer(&m);

	createKernel(&m, source_str, source_size);
	
  	enqueueKernel(&m);

	copyResults(&m, string);	
	 
	/* Display Result */
	puts(string);
	
  	destroyContext(&m);	
	 
	free(source_str);
	 
	return 0;
}

