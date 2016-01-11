#ifndef _CLKERNEL_H
#define _CLKERNEL_H
#include <clobject.h>

class ClKernel : public ClObject {
private:
protected:
	cl_kernel kernel;
	ClBuffer buffer;
	ClProgram program;
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
		lastError = clReleaseKernel(kernel);
	}
	void open(char *kernel_name) {	 
		/* Create OpenCL Kernel */
		kernel = clCreateKernel(program, kernel_name, &lastError);
	}
	void SetKernelArg(ClBuffer buffer) {	 
		m.memobj = buffer;
		/* Set OpenCL Kernel Parameters */
		lastError = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&m.memobj);
	};
};

#endif // _CLKERNEL_H
