#ifndef _CLPROGRAM_H
#define _CLPROGRAM_H
#include <clobject.h>
#include <clcontext.h>

static const char *tmpName = "hello";

class ClProgram : public ClObject {
private:
protected:
	cl_program program;
	size_t source_size;
public:
	ClProgram() : ClObject() {
	};
	ClProgram(ClContext &context, const char *source_str) : ClObject() {
		open(context, source_str);
	};
	~ClProgram() {
		close();
	};
	char *loadSource(size_t *source_size, const char *fileName) {
		/* Load the source code containing the kernel*/
		FILE *fp;
		char *source_str;
		fp = fopen(fileName, "r");
		if (!fp) {
			fprintf(stderr, "Failed to load kernel.\n");
			exit(1);
		}

		source_str = (char*)malloc(MAX_SOURCE_SIZE);
		*source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
		fclose(fp);
		TRACE("Load Source code (size=%zd)\n", *source_size);
		return source_str;
	};
	const char *name() {
		return tmpName;
	}
	void close() {
		if(!m_bOpen)
			return;
		lastError = clReleaseProgram(program);
		TRACE("lastError = %d\n", lastError);
		m_bOpen = false;
	}
	void open(ClContext &context, const char *source_str) {
		if(is_open())
			return;
		if(!context.is_open())
			return;

		/* Create Kernel Program from the source */
		program = clCreateProgramWithSource(context.get(), 1, (const char **)&source_str,
				(const size_t *)&source_size, &lastError);
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	};
	void BuildProgram(ClPlatform platform) {
		if(is_open())
			return;
		if(!platform.is_open())
			return;

		cl_device_id device_id = platform.get();
		/* Build Kernel Program */
		lastError = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
		TRACE("lastError = %d\n", lastError);
	};
	cl_program get() {
		return program;
	}
};
#endif // _CLPROGRAM_H
