#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <clplatform.h>
#include <clcontext.h>
#include <clcommandqueue.h>
#include <clprogram.h>
#include <clkernel.h>
#include <clbuffer.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main() {
	char string[MEM_SIZE];
	char input[MEM_SIZE] = {"hELLO"};

	ClPlatform platform(CL_DEVICE_TYPE_GPU);

	ClContext context(platform);
	ClCommandQueue command_queue(context, platform);
	/*
	ClBuffer buffer(context);

	buffer.EnqueueWriteBuffer();

	ClKernel kernel(context);

	kernel.loadSource();
	ClProgram program(kernel);

	program.BuildProgram(platform);

	program.CreateKernel("kernel_name");
	program.SetKernelArg(buffer);
	
	enqueueKernel();
	
	buffer.EnqueueReadBuffer();
	 */
	/* Display Result */
	puts(string);

	printf("atl = %d\n", string[32]);
	
	return 0;
}

