#ifndef _CLKERNEL_H
#define _CLKERNEL_H
#include <clobject.h>
#include <clprogram.h>

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
		if(!m_bOpen)
			return;
		lastError = clReleaseKernel(kernel);
		m_bOpen = false;
	}
	void open(ClProgram &program) {	 
		if(is_open())
			return;
		if(!program.is_open())
			return;

		TRACE("program name = %s\n", program.name());

		/* Create OpenCL Kernel */
		kernel = clCreateKernel(program.get(), program.name(), &lastError);
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
		switch(lastError) {
			case CL_INVALID_PROGRAM: TRACE("if program is not a valid program object."); break;
			case CL_INVALID_PROGRAM_EXECUTABLE: TRACE("if there is no successfully built executable for program."); break;
			case CL_INVALID_KERNEL_NAME: TRACE("if kernel_name is not found in program."); break;
			case CL_INVALID_KERNEL_DEFINITION: TRACE("if the function definition for __kernel function given by kernel_name such as the number of arguments, the argument types are not the same for all devices for which the program executable has been built."); break;
			case CL_INVALID_VALUE: TRACE("if kernel_name is NULL."); break;
			case CL_OUT_OF_HOST_MEMORY: TRACE("if there is a failure to allocate resources required by the OpenCL implementation on the host."); break;
			default:
				TRACE("huh");
		};
	}
	void SetKernelArg(ClBuffer buffer, int index) {	 
		if(!is_open())
			return;
		if(!buffer.is_open())
			return;
		cl_mem memobj = buffer.get();
		/* Set OpenCL Kernel Parameters */
		lastError = clSetKernelArg(kernel, index, sizeof(cl_mem), (void *)&memobj);
		TRACE("lastError = %d\n", lastError);
		TRACE("index = %d\n", index);
	};
	cl_kernel get() {
		return kernel;
	}
};

#endif // _CLKERNEL_H
