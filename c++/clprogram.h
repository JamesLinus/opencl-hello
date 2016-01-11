#ifndef _CLPROGRAM_H
#define _CLPROGRAM_H
#include <clobject.h>
#include <clcontext.h>

class ClProgram : public ClObject {
private:
protected:
	cl_program program;
public:
	ClProgram() {
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
	char *name() {
		return "hello";
	}
	void close() {
		lastError = clReleaseProgram(program);
	}
	void open() {
		/* Create Kernel Program from the source */
		program = clCreateProgramWithSource(context.get(), 1, (const char **)&source_str,
				(const size_t *)&source_size, &lastError);
	};
	void BuildProgram(ClPlatform platform) {
		/* Build Kernel Program */
		lastError = clBuildProgram(program.get(), 1, &m.device_id, NULL, NULL, NULL);
	};
};
#endif // _CLPROGRAM_H
