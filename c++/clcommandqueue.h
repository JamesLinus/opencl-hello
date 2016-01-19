#ifndef _CLCOMMANDQUEUE_H
#define _CLCOMMANDQUEUE_H
#include <clobject.h>
#include <clbuffer.h>
#include <clkernel.h>

class ClCommandQueue : public ClObject {
private:
protected:
	cl_command_queue command_queue;
public:
	ClCommandQueue() {
	};
	ClCommandQueue(ClContext &context, ClPlatform &platform) {
		open(context, platform);
	};
	~ClCommandQueue() {
		close();
	}
	void close() {
		if(!is_open())
			return;
		lastError = clFlush(command_queue);
		lastError = clFinish(command_queue);
		lastError = clReleaseCommandQueue(command_queue);
		TRACE("lastError = %d\n", lastError);
		m_bOpen = false;
	}
	void open(ClContext &context, ClPlatform &platform) {
		if(is_open())
			return;
		/* Create Command Queue */
#ifdef _FREESCALE
#warning "deprecated call clCreateCommandQueue used"
		command_queue = clCreateCommandQueue(context.get(), platform.get(), 0, &lastError);
#endif //_FREESCALE

#ifdef _INTEL
	//	command_queue = clCreateCommandQueueWithProperties(context.get(), platform.get(), 0, &lastError);
		command_queue = clCreateCommandQueue(context.get(), platform.get(), 0, &lastError);
#endif //_INTEL
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	};
	void enqueueKernel(ClKernel &kernel) {
		const size_t *global_work_offset = NULL; // must be NULL
		const size_t global_work_size[1] = {1}; //get_global_id
		const size_t local_work_size[1] = {1}; //get_local_size()
		if(!is_open())
			return;
		if(!kernel.is_open())
			return;
		/* Execute OpenCL Kernel */
#if(0)
		lastError = clEnqueueTask(command_queue, kernel.get(), 0, NULL,NULL);
#else
		lastError = clEnqueueNDRangeKernel(command_queue, kernel.get(), 1, global_work_offset, 
				global_work_size, local_work_size, 0, NULL, NULL);
#endif
		TRACE("lastError = %d\n", lastError);
	};
	void enqueueKernel(ClKernel &kernel, int dimCount, size_t *global, size_t *local) {
		const size_t *global_work_offset = NULL; // must be NULL
		if(!is_open())
			return;
		if(!kernel.is_open())
			return;

//		size_t local[1] = {1}; //get_local_size()
//		lastError = clGetKernelWorkGroupInfo(kernel.get(), device_id, 
//				CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), local, NULL);
//		TRACE("lastError = %d\n", lastError);

		/* Execute OpenCL Kernel */
#if(0)
		lastError = clEnqueueTask(command_queue, kernel.get(), 0, NULL,NULL);
#else
		lastError = clEnqueueNDRangeKernel(command_queue, kernel.get(), dimCount, 
				global_work_offset, global, local, 
				0, NULL, NULL);
#endif
		TRACE("lastError = %d\n", lastError);
	};
	void write(ClBuffer &buffer, const char *string, size_t nsize) {
		if(!is_open())
			return;
		if(!buffer.is_open())
			return;
		/* Copy input to the memory buffer */
		lastError = clEnqueueWriteBuffer(command_queue, buffer.get(), CL_TRUE, 0,
				nsize, string, 0, NULL, NULL);
		TRACE("lastError = %d\n", lastError);
//		if(lastError == CL_SUCCESS) {
//			INFO("write string = %s\n", string);
//		}
	};
	void read(ClBuffer &buffer, char *string, size_t nsize) {
		if(!is_open())
			return;
		if(!buffer.is_open())
			return;
		/* Copy results from the memory buffer */
		lastError = clEnqueueReadBuffer(command_queue, buffer.get(), CL_TRUE, 0,
				nsize, string, 0, NULL, NULL);
		TRACE("lastError = %d\n", lastError);
		/* Display Result */
//		if(lastError == CL_SUCCESS) {
//			INFO("read string = %s\n", string);
//		}
	};
};


#endif // _CLCOMMANDQUEUE_H
