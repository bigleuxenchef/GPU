#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
//using namespace std;

#define MAX_NAME_LEN 1000




int main(int argc, char* const argv[]) {

	//Number of devices
	cl_uint num_devices = 0;
	cl_uint i,j;
	char* value;
	size_t valueSize;
	cl_uint platformCount;
	//      cl_uint deviceCount;
	cl_uint maxComputeUnits;

//get number of platforms aka potential # of cards like CPU /GPU
	clGetPlatformIDs(0, NULL, &platformCount);
// allocating structure for number of plaform
	cl_platform_id *platforms = (cl_platform_id *) malloc(
			platformCount * sizeof(cl_platform_id));
// Get platform list of IDs
	clGetPlatformIDs(platformCount, platforms, NULL);

// Get the name of the platform based on IDs
	for (cl_uint i = 0; i < platformCount; ++i) {
		char buf[MAX_NAME_LEN];
		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(buf), buf,
				NULL);
		printf("CL_PLATFORM_VENDOR  |  [%d] %s\n", i, buf);

//Getting device ids
//    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
		cl_device_id* devices = (cl_device_id*) malloc(num_devices*sizeof(cl_device_id));
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices, devices,
				NULL);

		for (j = 0; j < num_devices; j++) {
			cl_device_type cl_buf;
			clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(buf), buf, NULL);
			fprintf(stdout, "Device[%d] %s supports ", j,buf);

			clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 128, buf, NULL);

			//Printing device info
			fprintf(stdout, "%s\n", buf);

			// print hardware device version
			clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, 0, NULL, &valueSize);
			value = (char*) malloc(valueSize);
			clGetDeviceInfo(devices[j], CL_DEVICE_VERSION, valueSize, value,
					NULL);
			printf(" %d.%d Hardware version: %s\n", j + 1, 1, value);
			free(value);

			// print software driver version
			clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, 0, NULL, &valueSize);
			value = (char*) malloc(valueSize);
			clGetDeviceInfo(devices[j], CL_DRIVER_VERSION, valueSize, value,
					NULL);
			printf(" %d.%d Software version: %s\n", j + 1, 2, value);
			free(value);

			// print c version supported by compiler for device
			clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, 0, NULL,
					&valueSize);
			value = (char*) malloc(valueSize);
			clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, valueSize,
					value, NULL);
			printf(" %d.%d OpenCL C version: %s\n", j + 1, 3, value);
			free(value);

			// print parallel compute units
			clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS,
					sizeof(maxComputeUnits), &maxComputeUnits, NULL);
			printf(" %d.%d Parallel compute units: %d\n",
					j + 1, 4, maxComputeUnits);

			clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, 128, &cl_buf, NULL);

			//Printing device info
			switch (cl_buf) {

			case CL_DEVICE_TYPE_DEFAULT:
				fprintf(stdout, "CL_DEVICE_TYPE_DEFAULT\n");
				break;
			case CL_DEVICE_TYPE_CPU:
				fprintf(stdout, "CL_DEVICE_TYPE_CPU\n");
				break;
			case CL_DEVICE_TYPE_GPU:
				fprintf(stdout, "CL_DEVICE_TYPE_GPU\n");
				break;
			case CL_DEVICE_TYPE_ACCELERATOR:
				fprintf(stdout, "CL_DEVICE_TYPE_ACCELERATOR\n");
				break;
			case CL_DEVICE_TYPE_ALL:
				fprintf(stdout, "CL_DEVICE_TYPE_ALL\n");
				break;

			}

		}

		free(devices);
	}
}
