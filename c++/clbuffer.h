#ifndef _CLBUFFER_H
#define _CLBUFFER_H
#include <clobject.h>

class ClBuffer : public ClObject {
private:
protected:
	ClContext context;
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
		/* Finalization */
		lastError = clReleaseMemObject(memobj);
	}
	void open(ClContext &context) {
		/* Create Memory Buffer */
		memobj = clCreateBuffer(context.get(), CL_MEM_READ_WRITE, MEM_SIZE * sizeof(char), NULL, &lastError);
	}
	cl_mem get() {
		return memobj;
	};
};
#endif // _CLBUFFER_H
