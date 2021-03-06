#ifndef _CLDEVICE_H
#define _CLDEVICE_H

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

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
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
	bool create(const char *fileName, const char *kernelName) {
		program = new ClProgram(*m_context->context, fileName, kernelName);
		bool ret = program->build(*m_context->platform);
		kernel = new ClKernel(*program);
		if(ret == true)
			return true;
		TRACE("create failed for program kernel %s\n", kernelName);
		return false;
	};
	void arg(ClBuffer &buffer) {
		//if(bClear == true)
		//	index = 0;
		//mapBufSize[index] = nsize;
		mapBuffer[index] = &buffer;
		index = kernel->arg(buffer, index);
	};
	void arg(unsigned int &buffer) {
		//if(bClear == true)
		//	index = 0;
		//mapBufSize[index] = nsize;
		//mapBuffer[index] = &buffer;
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
	void run(int dimCount, size_t *global, size_t *local) {
//		cl_device_id device_id = m_context->context->getId();
		m_context->command_queue->enqueueKernel(*kernel, dimCount, global, local);
	};
};

#endif // _CLDEVICE_H
