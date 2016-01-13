#ifndef _CLBUFFER_H
#define _CLBUFFER_H
#include <clobject.h>

class ClBuffer : public ClObject {
private:
protected:
	cl_mem memobj;
public:
	ClBuffer() {
	};
	ClBuffer(ClContext &context) {
		open(context);
	}
	~ClBuffer() {
		close();
	}
	void close() {
		if(!is_open())
			return;
		/* Finalization */
		lastError = clReleaseMemObject(memobj);
	}
	void open(ClContext &context) {
		if(is_open())
			return;
		if(!context.is_open())
			return;
		/* Create Memory Buffer */
		memobj = clCreateBuffer(context.get(), CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &lastError);
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	}
	cl_mem get() {
		return memobj;
	};
};
#endif // _CLBUFFER_H
