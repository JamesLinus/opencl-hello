#ifndef _CLKERNEL_H
#define _CLKERNEL_H
#include <clobject.h>
#include <clprogram.h>

class ClKernel : public ClObject {
private:
protected:
	cl_kernel kernel;
public:
	ClKernel() {
	};
	ClKernel(ClProgram &program) {
		open(program);
	};
	~ClKernel() {
		close();
	};
	void close() {
		if(!m_bOpen)
			return;
		lastError = clReleaseKernel(kernel);
		TRACE("lastError = %d\n", lastError);
		m_bOpen = false;
	}
	void open(ClProgram &program) {	 
		if(is_open())
			return;
		if(!program.is_open())
			return;

		//TRACE("program name = %s\n", program.name());

		cl_program prog = program.get();
		//TRACE("program = %p\n", prog);

		/* Create OpenCL Kernel */
		kernel = clCreateKernel(program.program, program.name(), &lastError);
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	}
	void arg(ClBuffer &buffer, int index) {	 
		if(!is_open())
			return;
		if(!buffer.is_open())
			return;
		//void *memobj = buffer.get();
		//TRACE("mem = %p\n", memobj);
		/* Set OpenCL Kernel Parameters */
		lastError = clSetKernelArg(kernel, index, sizeof(cl_mem), (void *)&buffer.memobj);
		TRACE("lastError = %d\n", lastError);
		TRACE("index = %d\n", index);
	};
	cl_kernel& get() {
		return kernel;
	}
};

#endif // _CLKERNEL_H
