#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include <clplatform.h>
#include <clcontext.h>
#include <clcommandqueue.h>
#include <clprogram.h>
#include <clkernel.h>
#include <clbuffer.h>
#include <clhost.h>
#include <cldevice.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

int main() 
{
	char istr[MEM_SIZE] = {"adveng"};
	char ostr[MEM_SIZE] = {"blank"};
	size_t nSize = MEM_SIZE;

	ClHost host(CL_DEVICE_TYPE_GPU);
	ClDevice gpu(&host);

	ClBuffer obuffer(host.get(), nSize * sizeof(char), true); 
	ClBuffer ibuffer(host.get(), nSize * sizeof(char), false); 

	gpu.create("./hello.cl", "hello");
	gpu.arg(obuffer);
	gpu.arg(ibuffer);
	
	gpu.write(1, istr, nSize * sizeof(char));
	gpu.run();
	gpu.read(0, ostr, nSize * sizeof(char));

	istr[0] = 0;
	gpu.write(1, istr, nSize * sizeof(char));
	gpu.run();
	gpu.read(0, ostr, nSize * sizeof(char));
}

