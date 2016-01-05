# sudo ln -s /opt/intel/opencl-1.2-sdk-5.0.0.43/include/CL /usr/include/CL

all:
	gcc -I /usr/include -L /usr/lib -D_FREESCALE -o class_hello class_hello.cpp -Wl,-rpath,/usr/lib -lGAL -lOpenCL

clean:
	rm hello

