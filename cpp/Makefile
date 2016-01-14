# sudo ln -s /opt/intel/opencl-1.2-sdk-5.0.0.43/include/CL /usr/include/CL

ARCH=$(shell uname -m)

all:
ifeq ($(ARCH),x86_64)
	g++ -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o class_hello class_hello.cpp -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL -I. -D_DEBUG
else
	g++ -I /usr/include -L /usr/lib -D_FREESCALE -o class_hello class_hello.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL
endif

freescale:
	g++ -I /usr/include -L /usr/lib -D_FREESCALE -o class_hello class_hello.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL

intel:
	g++ -I /usr/include -L /usr/lib -D_FREESCALE -o class_hello class_hello.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL

pc:
	g++ -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o class_hello class_hello.cpp -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL

clean:
	rm class_hello

