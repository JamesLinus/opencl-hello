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

class ocl_base {
public:
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
	ocl_base() {
		device_id = NULL;
		context = NULL;
		command_queue = NULL;
		memobj = NULL;
		program;
		kernel;
		platform_id;
		ret_num_devices;
		ret_num_platforms;
		ret;
	};
	~ocl_base() {
	}
};

class ocl {
private:
	ocl_base m;
	char *source_str;
	size_t source_size;

protected:
	void create() {
		createContext();	
		 
		createQueue();

		createBuffer();
	};

	void createContext() {
		/* Get Platform and Device Info */
		m.ret = clGetPlatformIDs(1, &m.platform_id, &m.ret_num_platforms);

		m.ret = clGetDeviceIDs(m.platform_id, CL_DEVICE_TYPE_GPU, 1, &m.device_id, &m.ret_num_devices);
		//m.ret = clGetDeviceIDs(m.platform_id, CL_DEVICE_TYPE_ACCELERATOR, 1, &m.device_id, &m.ret_num_devices);

		if(m.ret != 0) {
			fprintf(stdout, "failed on clGetDeviceIDs on GPU retry default\n");
			m.ret = clGetDeviceIDs(m.platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &m.device_id, &m.ret_num_devices);
			if(m.ret != 0)
				exit(1);
		}

		fprintf(stdout, "clGetDeviceIDs ret_num_devices = %d, for device_id = %p, ret = %u\n", m.ret_num_devices, (int*)m.device_id, m.ret);

		char cBuffer[1024];
		clGetDeviceInfo(m.device_id, CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
		printf("CL_DEVICE_NAME: %s\n", cBuffer);
		clGetDeviceInfo(m.device_id, CL_DRIVER_VERSION, sizeof(cBuffer), &cBuffer, NULL);
		printf("CL_DRIVER_VERSION: %s\n\n", cBuffer);
		 
		/* Create OpenCL context */
		m.context = clCreateContext(NULL, 1, &m.device_id, NULL, NULL, &m.ret);
	};

	void createQueue() {
		/* Create Command Queue */
#ifdef _FREESCALE
#warning "deprecated call clCreateCommandQueue used"
		m.command_queue = clCreateCommandQueue(m.context, m.device_id, 0, &m.ret);
#endif //_FREESCALE

#ifdef _INTEL
		m.command_queue = clCreateCommandQueueWithProperties(m.context, m.device_id, 0, &m.ret);
#endif //_INTEL

	};

	void createBuffer() {
		/* Create Memory Buffer */
		m.memobj = clCreateBuffer(m.context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &m.ret);
	};

public:
	ocl() 
	 : source_str(NULL)
		, source_size(0)
	{
		create();
	};

	~ocl() {
		destroyContext();	
		if(source_str != NULL) 
			free(source_str);
	};
	
	void run(const char *fileName, const char *kernel_name) {
		if(source_str != NULL) 
			free(source_str);

		source_str = NULL;
		
		source_str = loadSource(&source_size, fileName);

		createKernel(source_str, source_size, kernel_name);
		
		enqueueKernel();
	};

	void copyResults(char *string) {
		/* Copy results from the memory buffer */
		m.ret = clEnqueueReadBuffer(m.command_queue, m.memobj, CL_TRUE, 0,
				MEM_SIZE * sizeof(char), string, 0, NULL, NULL);
	};

	void copyInput(char *string) {
		/* Copy input to the memory buffer */
		m.ret = clEnqueueWriteBuffer(m.command_queue, m.memobj, CL_TRUE, 0,
				MEM_SIZE * sizeof(char), string, 0, NULL, NULL);
	};

protected:
	char *loadSource(size_t *source_size, const char *fileName) {
		/* Load the source code containing the kernel*/
		FILE *fp;
		char *source_str;
		fp = fopen(fileName, "r");
		if (!fp) {
			fprintf(stderr, "Failed to load kernel.\n");
			exit(1);
		}

		source_str = (char*)malloc(MAX_SOURCE_SIZE);
		*source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
		fclose(fp);
		fprintf(stdout, "Load Source code (size=%zd)\n", *source_size);
		return source_str;
	};

	void createKernel(char *source_str, size_t source_size, const char *kernel_name) {
		/* Create Kernel Program from the source */
		m.program = clCreateProgramWithSource(m.context, 1, (const char **)&source_str,
				(const size_t *)&source_size, &m.ret);
		 
		fprintf(stdout, "Build Kernel Program\n");

		/* Build Kernel Program */
		m.ret = clBuildProgram(m.program, 1, &m.device_id, NULL, NULL, NULL);
		 
		/* Create OpenCL Kernel */
		m.kernel = clCreateKernel(m.program, kernel_name, &m.ret);
		 
		/* Set OpenCL Kernel Parameters */
		m.ret = clSetKernelArg(m.kernel, 0, sizeof(cl_mem), (void *)&m.memobj);
	};

	void enqueueKernel() {
		const size_t *global_work_offset = NULL;
		const size_t global_work_size[1] = {1};
		const size_t local_work_size[1] = {1};

		/* Execute OpenCL Kernel */
		//ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
		m.ret = clEnqueueNDRangeKernel(m.command_queue, m.kernel, 1, global_work_offset, 
				global_work_size, local_work_size, 0, NULL, NULL);
	};

	void destroyContext() {
		/* Finalization */
		m.ret = clFlush(m.command_queue);
		m.ret = clFinish(m.command_queue);
		m.ret = clReleaseKernel(m.kernel);
		m.ret = clReleaseProgram(m.program);
		m.ret = clReleaseMemObject(m.memobj);
		m.ret = clReleaseCommandQueue(m.command_queue);
		m.ret = clReleaseContext(m.context);
	};
};

int main() {
	ocl k;	 
	char string[MEM_SIZE];
	char input[MEM_SIZE] = {"hELLO"};

	k.copyInput(input);

	// hello.cl is the file containing the source to run via opencl
  	// hello is the __kernel name	
	k.run("./hello.cl", "hello");
	
	k.copyResults(string);	
	 
	/* Display Result */
	puts(string);

	printf("atl = %d\n", string[32]);
	
	return 0;
}

