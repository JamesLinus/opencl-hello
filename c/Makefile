
ARCH=$(shell uname -m)

all:
ifeq ($(ARCH),x86_64)
	g++ -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o hello hello.c -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL -I. -D_DEBUG
else
	gcc -I /usr/include -L /usr/lib -D_FREESCALE -o hello hello.c -Wl,-rpath,/usr/lib -lGAL -lOpenCL
endif

intel:
	gcc -I /usr/include -L /usr/lib -D_INTEL -o hello hello.c -Wl,-rpath,/usr/lib -lOpenCL

nvidia:
	gcc -I /usr/include -L /usr/lib -D_NVIDIA -o hello hello.c -Wl,-rpath,/usr/lib -lOpenCL

pc:
	gcc -I /opt/intel/opencl-1.2-sdk-5.0.0.43/include -L /opt/intel/opencl-1.2-5.0.0.43/lib64 -o hello hello.c -Wl,-rpath,/opt/intel/opencl-1.2-5.0.0.43/lib64 -lOpenCL -D_INTEL

clean:
	rm hello

