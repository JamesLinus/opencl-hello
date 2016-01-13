#ifndef _CLOBJECT_H
#define _CLOBJECT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)
#define TRACE(format, ...) fprintf(stdout, "(%s/%s:%d) " format, __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__ )

class ClObject {
private:
protected:
	int m_bOpen;
	cl_int lastError;
public:
	ClObject() {
		m_bOpen = false;
		lastError = CL_SUCCESS;
	};
	~ClObject() {
	}
	int is_open() {
		return m_bOpen;
	};
};

#endif //_CLOBJECT_H
