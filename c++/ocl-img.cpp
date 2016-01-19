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
#include <climage.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
 
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

void worker(unsigned char *ostr, int nsize) 
{
	// this is the algorithm to port to cl file
	int cnt = 0;
	for(cnt = 0; cnt < nsize; cnt++)
	{
		if(ostr[cnt] > 205) {
			ostr[cnt] = 10;
		}
		if(ostr[cnt] < 10) {
			ostr[cnt] = 205;
		}
	}
}

void search() 
{
	for(int top = 0; top <= 1; top++) {
		int step = (top * 665);
		for(int left = 0; left <= 1; left++) {
			int offset = (left * 915);
			for(int x = 0; x <= 915; x++) {
				for(int y = 0; y <= 665; y++) {
					TRACE("top = %d, left = %d, x = %d, y = %d\n", top, left, x, y);
				}
			}
		}
	}
}

//#define _WITH_INVERT
int main() 
{
	//search(); exit(0);
	ClImage image("./test.bmp");
	size_t nSize = image.Size();
	int nsize[10];
	unsigned char *ostr = (unsigned char *)image.Data();

	ClHost host(CL_DEVICE_TYPE_GPU);
	ClDevice gpu(&host);

	ClBuffer obuffer(host.get(), nSize * sizeof(unsigned char), true); 
	ClBuffer ibuffer(host.get(), 10 * sizeof(int), false); 

#ifdef _WITH_INVERT
	gpu.create("./hello-img.cl", "imageinvert");
#else
	gpu.create("./hello-img.cl", "hello");
#endif //_WITH_INVERT
	gpu.arg(obuffer);

#ifdef _WITH_INVERT
	unsigned int rgbsize = nSize;
	gpu.arg(rgbsize);
#else
	gpu.arg(ibuffer);
#endif //_WITH_INVERT

#ifdef _WITH_INVERT
	nsize[0] = nSize;
#else
	nsize[0] = 915;
#endif //_WITH_INVERT

	nsize[1] = image.width;
	nsize[2] = image.height;

	gpu.write(0, (char *)ostr, nSize * sizeof(unsigned char));
#ifndef _WITH_INVERT
	gpu.write(1, (char *)nsize, 10 * sizeof(int));
#endif //_WITH_INVERT

	TRACE("width = %d\n", nsize[0]);
	TRACE("size_t = %lu\n", sizeof(size_t));

#ifdef _WITH_INVERT
	gpu.run();
#else
#define WORKDIM 2
	size_t global[WORKDIM] = {2, 2};
	size_t local[WORKDIM] = {2, 2};
	gpu.run(2, global, local);
#endif //_WITH_INVERT 

	gpu.read(0, (char *)ostr, nSize * sizeof(char));

	//worker(ostr, nsize[0]);
	image.store("results.bmp");
}

