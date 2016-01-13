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
#define TRACE(format, ...) fprintf(stdout, "(%s:%d) " format, __FUNCTION__, __LINE__, ## __VA_ARGS__ )

class ClObject {
private:
protected:
	int m_bOpen;
	cl_int lastError;
public:
	ClObject() {
		m_bOpen = false;
	};
	~ClObject() {
	}
	int is_open() {
		return m_bOpen;
	};
};
/*
class ocl_base {
public:
	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;
	cl_mem memobj;
	cl_program program;
	cl_kernel kernel;
	cl_platform_id platform_id;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	ocl_base() {
		device_id = NULL;
		context = NULL;
		command_queue = NULL;
		memobj = NULL;
		program;
		kernel;
		platform_id;
		ret_num_devices;
		ret_num_platforms;
		ret;
	};
	~ocl_base() {
	}
};
*/
#endif //_CLOBJECT_H
