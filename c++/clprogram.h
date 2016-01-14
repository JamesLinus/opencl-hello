#ifndef _CLPROGRAM_H
#define _CLPROGRAM_H
#include <clobject.h>
#include <clcontext.h>

class ClProgram : public ClObject {
private:
protected:
	size_t source_size;
	char *source_str;
	const char *program_name;
public:
	cl_program program;
	ClProgram() : ClObject() {
	};
	ClProgram(ClContext &context, const char *fileName, const char *progname) : ClObject() {
		char *src = load(fileName);
		open(context, src);
		program_name = progname;
	};
	~ClProgram() {
		close();
	};
	char *load(const char *fileName) {
		/* Load the source code containing the kernel*/
		FILE *fp;
		fp = fopen(fileName, "r");
		if (!fp) {
			TRACE("Failed to load kernel source.\n");
			exit(1);
		}

		source_str = (char*)malloc(MAX_SOURCE_SIZE);
		source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
		fclose(fp);
		INFO("Load Source code (size=%zd)\n", source_size);
		//TRACE("src = %s\n", source_str);
		return source_str;
	};
	const char *name() {
		return program_name;
	}
	void close() {
		if(!m_bOpen)
			return;
		lastError = clReleaseProgram(program);
		if(source_str) 
			free(source_str);
		source_str = NULL;
		TRACE("lastError = %d\n", lastError);
		m_bOpen = false;
	}
	void open(ClContext &context, const char *source_str) {
		if(is_open())
			return;
		if(!context.is_open())
			return;
		//TRACE("source_str = %s\n", source_str);	
		/* Create Kernel Program from the source */
		program = clCreateProgramWithSource(context.get(), 1, (const char **)&source_str,
				(const size_t *)&source_size, &lastError);
		TRACE("lastError = %d\n", lastError);
		if(lastError == CL_SUCCESS)
			m_bOpen = true;
	};
	void build(ClPlatform &platform) {
		if(!is_open())
			return;
		if(!platform.is_open())
			return;

		cl_device_id device_id = platform.get();

		/* Build Kernel Program */
		lastError = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
		TRACE("lastError = %d\n", lastError);
	};
	cl_program& get() {
		return program;
	}
};
#endif // _CLPROGRAM_H
