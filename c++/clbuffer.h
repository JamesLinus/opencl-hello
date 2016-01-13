#ifndef _CLBUFFER_H
#define _CLBUFFER_H
#include <clobject.h>

class ClBuffer : public ClObject {
private:
protected:
public:
	cl_mem memobj;
	ClBuffer() {
	};
	ClBuffer(ClContext &context, size_t nSize, bool bReadOnly) {
		open(context, nSize, bReadOnly);
	}
	~ClBuffer() {
		close();
	}
	void close() {
		if(!is_open())
			return;
		/* Finalization */
		lastError = clReleaseMemObject(memobj);
		TRACE("lastError = %d\n", lastError);
	}
	void open(ClContext &context, size_t nsize, bool bReadOnly) {
		if(is_open())
			return;
		if(!context.is_open())
			return;
		/* Create Memory Buffer */
		if(bReadOnly) {
			memobj = clCreateBuffer(context.get(), CL_MEM_WRITE_ONLY, nsize, NULL, &lastError);
		} else {
			memobj = clCreateBuffer(context.get(), CL_MEM_READ_WRITE, nsize, NULL, &lastError);
		}
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	}
	cl_mem& get() {
		return memobj;
	};
};
#endif // _CLBUFFER_H
