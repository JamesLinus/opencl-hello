# opencl-hello
opencl hello world
```
 to build on freescale/imx: gcc main.c -lGAL -lOpenCL -o cl_sample -std=c99
```
* for intel download intel_code_builder_for_opencl_2015_ubuntu_5.0.0.43_x64.tgz and intel_ocl_samples_linux.tar.gz
* mkdir -p /etc/OpenCL/vendors && cp intel64.icd /etc/OpenCL/vendors

4 examples

**_./main.c_ for raw opencl usage**

**_./c_ for a cleaner opencl usage**

**_./cpp_ for opencl class wrapper**

**_./c++_ for opencl class framework**
