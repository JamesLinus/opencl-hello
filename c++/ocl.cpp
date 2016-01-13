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
	char ostr[MEM_SIZE] = {"blank"};
	size_t nSize = MEM_SIZE;

	//ClPlatform platform(CL_DEVICE_TYPE_GPU);
	ClPlatform platform(CL_DEVICE_TYPE_DEFAULT);
	platform.show();

	ClContext context(platform);

	ClBuffer obuffer(context, nSize * sizeof(char), true); 
	ClBuffer ibuffer(context, nSize * sizeof(char), false); 

	ClCommandQueue command_queue(context, platform);

	ClProgram program(context, "./hello.cl", "hello");
	program.build(platform);

	ClKernel kernel(program);
	kernel.arg(obuffer, 0);
	kernel.arg(ibuffer, 1);
	
	command_queue.write(ibuffer, istr, nSize * sizeof(char));
	command_queue.enqueueKernel(kernel);
	command_queue.read(obuffer, ostr, nSize * sizeof(char));

	istr[0] = 0;
	command_queue.write(ibuffer, istr, nSize * sizeof(char));
	command_queue.enqueueKernel(kernel);
	command_queue.read(obuffer, ostr, nSize * sizeof(char));
	
	return 0;
}

