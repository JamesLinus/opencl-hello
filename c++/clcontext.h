#ifndef _CLCONTEXT_H
#define _CLCONTEXT_H

#include <clobject.h>

class ClContext : public ClObject {
private:
protected:
	cl_context context;
	ClPlatform platform;
	cl_device_id device_id;
public:
	ClContext() {
	};
	ClContext(ClPlatform &platform) {
		open(platform);
	};
	~ClContext() {
		close();
	};
	void close() {
		lastError = clReleaseContext(context);
	}
	void open(ClPlatform &platform) {
		device_id = platform.get();
		/* Create OpenCL context */
		context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &lastError);
	}
	cl_context get() {
		return context;
	}
};


#endif // _CLCONTEXT_H
