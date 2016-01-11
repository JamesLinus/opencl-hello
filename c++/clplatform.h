#ifndef _CLPLATFORM_H
#define _CLPLATFORM_H
#include <clobject.h>

class ClPlatform : public ClObject {
private:
protected:
	cl_platform_id platform_id;
	cl_device_type type;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_device_id device_id; 
public:
	ClPlatform() {
		open(CL_DEVICE_TYPE_DEFAULT);
	}
	ClPlatform(cl_device_type type) {
		open(type);
	};
	void close() {
		platform_id = 0;
	};
	void open(cl_device_type type) {
		/* Get Platform and Device Info */
		lastError = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

		lastError = clGetDeviceIDs(platform_id, type, 1, &device_id, &ret_num_devices);
		//m.ret = clGetDeviceIDs(m.platform_id, CL_DEVICE_TYPE_ACCELERATOR, 1, &m.device_id, &m.ret_num_devices);

		if(lastError != 0) {
			TRACE("failed on clGetDeviceIDs on GPU retry default\n");
//			open(CL_DEVICE_TYPE_DEFAULT);
			lastError = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
			if(lastError != 0)
				exit(1);
		}
		show();
	};
	void show() {
		TRACE("clGetDeviceIDs ret_num_devices = %d, for device_id = %p, ret = %u\n", ret_num_devices, (int*)device_id, lastError);

		char cBuffer[1024];
		clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
		TRACE("CL_DEVICE_NAME: %s\n", cBuffer);
		clGetDeviceInfo(device_id, CL_DRIVER_VERSION, sizeof(cBuffer), &cBuffer, NULL);
		TRACE("CL_DRIVER_VERSION: %s\n\n", cBuffer);
	};
	~ClPlatform() {
		close();
	};
	cl_device_id get() {
		return device_id;
	};
};
#endif // _CLPLATFORM_H
