#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <fstream>

#include <clplatform.h>
#include <clcontext.h>
#include <clcommandqueue.h>
#include <clprogram.h>
#include <clkernel.h>
#include <clbuffer.h>
#include <clhost.h>
#include <cldevice.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

class ClImage : public ClObject
{
private:
protected:
	std::fstream file;
	int m_length;
	int m_nsize;
	unsigned char *m_buffer;
	unsigned char *m_header;
public:
	ClImage() {
	};
	ClImage(const char *fileName) {
		load(fileName);
	};
	~ClImage() {
		close();
	};
	void open(const char *fileName) {
		file.open(fileName, std::fstream::in);
		TRACE("filename = %s\n", fileName);
		file.seekg (0, file.end);
		m_length = file.tellg();
		file.seekg (0, file.beg);
		TRACE("image m_length = %d\n", m_length);
	};
	void load(const char *fileName) {
		open(fileName);
		m_header = new unsigned char[54];
		file.read((char *)m_header, 54);
		int width = *(int*)&m_header[18];
		int height = *(int*)&m_header[22];
		TRACE("image width = %d, height = %d\n", width, height);
		m_nsize = 3 * width * height;
		TRACE("image nsize = %d\n", m_nsize);
		m_buffer = new unsigned char[m_nsize];
		file.read((char *)m_buffer, m_nsize);
		//close();
	};
	void store(const char *fileName) {
		std::fstream ofile;
		ofile.open(fileName, std::fstream::out);
		ofile.write((char *)m_header, 54); // use same header
		ofile.write((char *)m_buffer, m_nsize); 
		ofile.close();
	}
	void close() {
		file.close();
		delete m_buffer;
	};
	int Size() {
		return m_nsize;
	};
	char *Data() {
		return (char *)m_buffer;
	}
};

int main() 
{
	//char istr[MEM_SIZE] = {"adveng"}; // TODO: change these to image in memory
	char ostr[MEM_SIZE] = {"blank"};
	ClImage image("./test.bmp");
	//size_t nSize = MEM_SIZE;
	size_t nSize = image.Size();
	char *istr = image.Data();

	ClHost host(CL_DEVICE_TYPE_GPU);
	ClDevice gpu(&host);

	ClBuffer obuffer(host.get(), nSize * sizeof(char), true); 
	ClBuffer ibuffer(host.get(), nSize * sizeof(char), false); 

	gpu.create("./hello-img.cl", "hello");
	gpu.arg(obuffer);
	gpu.arg(ibuffer);
	
	gpu.write(1, istr, nSize * sizeof(char));
	gpu.run();
	gpu.read(0, ostr, nSize * sizeof(char));

	istr[0] = 0;
	gpu.write(1, istr, nSize * sizeof(char));
	gpu.run();
	gpu.read(0, ostr, nSize * sizeof(char));

	/*
	// this is the algorithm to port to cl file
	int cnt = 0;
	for(cnt  = 0; cnt < nSize; cnt += 3) {
		unsigned char tmp = istr[cnt];
		istr[cnt] = istr[cnt+2];
		istr[cnt+2] = tmp - 30;
		cnt++;
	}
	*/
	image.store("results.bmp");
}

