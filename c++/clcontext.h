#ifndef _CLCONTEXT_H
#define _CLCONTEXT_H

#include <clobject.h>

class ClContext : public ClObject {
private:
protected:
	cl_device_id device_id;
public:
	cl_context context;
	ClContext() {
	};
	ClContext(ClPlatform &platform) {
		open(platform);
	};
	~ClContext() {
		close();
	};
	void close() {
		if(!m_bOpen)
			return;
		lastError = clReleaseContext(context);
		m_bOpen = false;
	}
	void open(ClPlatform &platform) {
		if(is_open())
			return;
		if(!platform.is_open())
			return;

		device_id = platform.get();
		context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &lastError);
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	}
	cl_context& get() {
		return context;
	}
	cl_device_id getId() {
		return device_id;
	}
	void getEvent(cl_event &evt) {
		if(!is_open())
			return;
		evt = clCreateUserEvent(context, &lastError);
		TRACE("lastError = %d\n", lastError);
	}
};


#endif // _CLCONTEXT_H
