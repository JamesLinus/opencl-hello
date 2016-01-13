#ifndef _CLPLATFORM_H
#define _CLPLATFORM_H
#include <clobject.h>

class ClPlatform : public ClObject {
private:
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
protected:
	cl_platform_id platform_id;
	cl_device_type type;
	cl_device_id device_id; 
public:
	ClPlatform() : ClObject() {
		open(CL_DEVICE_TYPE_DEFAULT);
	}
	ClPlatform(cl_device_type type) : ClObject() {
		open(type);
	};
	void close() {
		if(!m_bOpen)
			return;
		platform_id = 0;
		m_bOpen = false;
	};
	void open(cl_device_type type) {
		/* Get Platform and Device Info */
		lastError = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
		TRACE("lastError = %d\n", lastError);

		lastError = clGetDeviceIDs(platform_id, type, 1, &device_id, &ret_num_devices);
		//m.ret = clGetDeviceIDs(m.platform_id, CL_DEVICE_TYPE_ACCELERATOR, 1, &m.device_id, &m.ret_num_devices);
		TRACE("lastError = %d\n", lastError);

		if(lastError != CL_SUCCESS) {
			TRACE("failed on clGetDeviceIDs on GPU retry default\n");
//			open(CL_DEVICE_TYPE_DEFAULT);
			lastError = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, 
					&ret_num_devices);
			if(lastError != CL_SUCCESS)
				exit(1);
		}
		m_bOpen = true;
	};
	void show() {
		if(!is_open())
			return;
		char cBuffer[1024];
		TRACE("clGetDeviceIDs ret_num_devices = %d, for device_id = %p, ret = %u\n", 
				ret_num_devices, (int*)device_id, lastError);
		clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(cBuffer), &cBuffer, NULL);
		INFO("CL_DEVICE_NAME: %s\n", cBuffer);
		clGetDeviceInfo(device_id, CL_DRIVER_VERSION, sizeof(cBuffer), &cBuffer, NULL);
		INFO("CL_DRIVER_VERSION: %s\n", cBuffer);
	};
	~ClPlatform() {
		close();
	};
	cl_device_id& get() {
		return device_id;
	};
};
#endif // _CLPLATFORM_H
