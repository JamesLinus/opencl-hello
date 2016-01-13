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
	char istr[MEM_SIZE] = {"adveng"};
	char ostr[MEM_SIZE];
	char input[MEM_SIZE] = {"hELLO"};

	ClPlatform platform(CL_DEVICE_TYPE_GPU);

	ClContext context(platform);

	ClBuffer buffer(context); 

	ClCommandQueue command_queue(context, platform);
//	command_queue.EnqueueWriteBuffer(buffer, istr);

	ClProgram program(context, "./hello.cl", "hello");
	program.BuildProgram(platform);

	ClKernel kernel(program);
	kernel.SetKernelArg(buffer, 0);
	
	command_queue.EnqueueKernel(kernel);
	command_queue.EnqueueReadBuffer(buffer, ostr);

	/* Display Result */
	//puts(ostr);

	printf("atl = %d\n", ostr[32]);
	
	return 0;
}

