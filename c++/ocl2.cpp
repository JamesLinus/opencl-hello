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

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

class ClHost 
{
private:
protected:
public:
	ClPlatform *platform;
	ClContext *context;
	ClCommandQueue *command_queue;
	ClHost(cl_device_type type) {
		platform = new ClPlatform(type);
		context = new ClContext(*platform);
		command_queue = new ClCommandQueue(*context, *platform);
	};
	~ClHost() {
		delete platform;
		delete context;
		delete command_queue;
	}
	ClContext& get() {
		return *context;
	}
};

class ClDevice 
{
private:
protected:
public:
	ClProgram *program;
	ClKernel *kernel;
	int index;
	ClHost *m_context;
	std::map<int, ClBuffer*> mapBuffer;
	std::map<int, size_t> mapBufSize;

	ClDevice(ClHost *context) {
		m_context = context;
		index = 0;
	};
	~ClDevice() {
		delete program;
		delete kernel;
	};
	void create(const char *fileName, const char *kernelName) {
		program = new ClProgram(*m_context->context, fileName, kernelName);
		program->build(*m_context->platform);
		kernel = new ClKernel(*program);
	};
	void arg(ClBuffer &buffer) {
		//if(bClear == true)
		//	index = 0;
		//mapBufSize[index] = nsize;
		mapBuffer[index] = &buffer;
		index = kernel->arg(buffer, index);
	};
	void write(int index, const char *istr, int nsize) {
		ClBuffer *buffer = mapBuffer[index];
		m_context->command_queue->write(*buffer, istr, nsize);
	};
	void read(int index, char *ostr, int nsize) {
		ClBuffer *buffer = mapBuffer[index];
		m_context->command_queue->read(*buffer, ostr, nsize);
	}
	void run() {
		m_context->command_queue->enqueueKernel(*kernel);
	};
};

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

