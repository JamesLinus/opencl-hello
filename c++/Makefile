# sudo ln -s /opt/intel/opencl-1.2-sdk-5.0.0.43/include/CL /usr/include/CL

ARCH=$(shell uname -m)

all:
	@echo "ARCH=$(ARCH)"
ifeq ($(ARCH),x86_64)
	g++ -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o ocl ocl.cpp -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL -I. -D_DEBUG
else
	g++ -I /usr/include -L /usr/lib -D_FREESCALE -o ocl ocl.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL -I.
endif

ifeq ($(ARCH),x86_64)
	g++ -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o ocl2 ocl2.cpp -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL -I. -D_DEBUG
else
	g++ -I /usr/include -L /usr/lib -D_FREESCALE -o ocl ocl.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL -I.
endif

freescale:
	g++ -I /usr/include -L /usr/lib -D_FREESCALE -o ocl ocl.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL -I.

intel:
	g++ -I /usr/include -L /usr/lib -D_INTEL -o ocl ocl.cpp -Wl,-rpath,/usr/lib -lOpenCL -I.

pc:
	g++ -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o ocl ocl.cpp -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL -I.

clean:
	rm ocl

