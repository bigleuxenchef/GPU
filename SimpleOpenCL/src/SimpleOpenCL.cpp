
//
// File:       hello.c
//
// Abstract:   A simple "Hello World" compute example showing basic usage of OpenCL which
//             calculates the mathematical square (X[i] = pow(X[i],2)) for a buffer of
//             floating point values.
//
//
// Version:    <1.0>
//
// Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc. ("Apple")
//             in consideration of your agreement to the following terms, and your use,
//             installation, modification or redistribution of this Apple software
//             constitutes acceptance of these terms.  If you do not agree with these
//             terms, please do not use, install, modify or redistribute this Apple
//             software.
//
//             In consideration of your agreement to abide by the following terms, and
//             subject to these terms, Apple grants you a personal, non - exclusive
//             license, under Apple's copyrights in this original Apple software ( the
//             "Apple Software" ), to use, reproduce, modify and redistribute the Apple
//             Software, with or without modifications, in source and / or binary forms;
//             provided that if you redistribute the Apple Software in its entirety and
//             without modifications, you must retain this notice and the following text
//             and disclaimers in all such redistributions of the Apple Software. Neither
//             the name, trademarks, service marks or logos of Apple Inc. may be used to
//             endorse or promote products derived from the Apple Software without specific
//             prior written permission from Apple.  Except as expressly stated in this
//             notice, no other rights or licenses, express or implied, are granted by
//             Apple herein, including but not limited to any patent rights that may be
//             infringed by your derivative works or by other works in which the Apple
//             Software may be incorporated.
//
//             The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
//             WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
//             WARRANTIES OF NON - INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
//             PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION
//             ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
//
//             IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
//             CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//             SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//             INTERRUPTION ) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
//             AND / OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER
//             UNDER THEORY OF CONTRACT, TORT ( INCLUDING NEGLIGENCE ), STRICT LIABILITY OR
//             OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright ( C ) 2008 Apple Inc. All Rights Reserved.
//

////////////////////////////////////////////////////////////////////////////////
//#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cmath>
#include <time.h>
#include <OpenCL/opencl.h>
#include <chrono>
#include <iostream>
using namespace std::chrono;


#define ZERO 1e-10
#define SMALL 1e-4
#define DELTA 1e-10
#define isZero(A) ( (A < ZERO) && (A > -ZERO) )
#define isSmall(A) ( (A < SMALL) && (A > -SMALL) )
#define isSame(A, B) ( ((A-B) < ZERO) && ((A-B) > -ZERO) )
#define isSimilar(A, B) ( ((A-B) < SMALL) && ((A-B) > -SMALL) )
#define isBetween(A, B, C) ( ((A-B) > -ZERO) && ((A-C) < ZERO) )



////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//
#define DATA_SIZE (1000)
#define MAX_BATCH_SIZE (512000)
#define NUMBER_THREAD (1)
#define SCRATCH_SIZE 2048
#define USAGE "SimpleOpenCl OPTIONS\n \
\t-g        - Use CPU instead of GPU \n\
\t-h        - Display this message \n\
\t-v []     - Display extra information 0-1-2\n \
\t-k        - Validate opertion\n \
\t-t [NUM]  - Set number of thread, default: 1\n \
\t-s [NUM]  - # steps, default 320000\n\
\t-b [NUM]  - Batch size, default "




////////////////////////////////////////////////////////////////////////////////

// Simple compute kernel which computes the square of an input array
//
const char *KernelSource = "\n" \
"#define RAND_MAX (0x7fffffff)\n" \
"float rand(unsigned int *seed){\n" \
"*seed=((*seed)*16807)%2147483647;\n"\
"return (float)(*seed)*4.6566129e-10;;}\n"\
"float srand(){return 1;}\n"\
" float square2(                                                       \n" \
"   float input)                                               \n" \
"{          return input*input;                                        \n" \
"}    \n" \
"float calcpi(int n){\n"\
"  float inside = 0;\n"\
"  srand(1);\n"\
"  for(int i = 0; i < n; ++i){\n"\
"    float x = ((float) rand(&n) / (0x7fffffff));\n"\
"    float y = ((float) rand(&n) / (0x7fffffff));\n"\
"    if( x*x + y*y < 1.0 ){\n"\
"      inside = inside + 1;\n"\
"    }\n"\
"  }\n"\
"  return(4 * inside / n);\n"\
"} \n"\
"__kernel void square(                                                  \n" \
"   __global float* input,                                              \n" \
"   __global float* output,                                             \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0),loop;  \n"\
"   if(i < count)                                                       \n" \
"      { output[i] = input[i] * input[i];                               \n" \
"		output[i] = sqrt(output[i]/2);									\n" \
"		output[i] = log(output[i]);  }                                  \n" \
"}                                                                      \n" \
"\n";



////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
	int err;                            // error code returned from api calls
	float *data;              // original data set given to device
	float *results;           // results returned from device
	//    float data[DATA_SIZE];              // original data set given to device
	//    float results[DATA_SIZE];           // results returned from device

	unsigned int correct;               // number of correct results returned
	unsigned int notcorrect;               // number of correct results returned

	size_t global;                     // global domain size for our calculation
	size_t local;                       // local domain size for our calculation

	cl_device_id device_id;             // compute device id
	cl_context context;                 // compute context
	cl_command_queue commands;          // compute command queue
	cl_program program;                 // compute program
	cl_kernel kernel;                   // compute kernel

	cl_mem input;                      // device memory used for the input array
	cl_mem output;                    // device memory used for the output array
	int gpu = 1;
	// Fill our data set with random float values
	//
	int i = 0, opt, verbose, validate = 0;
	unsigned long count = DATA_SIZE,temp=0;
	long kernelcount = MAX_BATCH_SIZE;
	clock_t start,c_memcreate, c_meminit, c_device, c_context, c_command, c_read, c_program,
	c_build, c_kernel, *c_startcopydata, *c_copydata, *c_execute,
	*c_readback, c_test, c_end;
	int thread = NUMBER_THREAD;


	while ((opt = getopt(argc, argv, "gchkv:s:t:b:")) != -1) {
		switch (opt) {


		case 'g':
			gpu = false;
			break;
		case 'v':
			verbose = atoi(optarg);
			verbose = verbose== 0 ? 1:verbose;
			printf("Verbose %d\n",verbose);
			break;
		case 'h':
			printf(USAGE);
			exit(1);
		case 'k':
			validate = true;
			break;
		case 's':
			temp = (unsigned long) atol(optarg);
			count = (unsigned long) temp;
			break;
		case 'b':
			kernelcount = atoi(optarg);
			break;
		case 't':
			thread = atoi(optarg);
			break;

		}
	}
	const auto beginclock = high_resolution_clock::now();

	start = clock();


	data = (float *) malloc(sizeof(float) * count);
	results = (float *) malloc(sizeof(float) * count);
	c_memcreate = clock();

	for (i = 0; i < count; i++)
		data[i] = i + 1; //rand() / (float)RAND_MAX;
	c_meminit = clock();

	// Connect to a compute device
	//
	err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1,
			&device_id, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to create a device group!\n");
		return EXIT_FAILURE;
	} else {
		char scratch[SCRATCH_SIZE];
		clGetDeviceInfo(device_id, CL_DEVICE_NAME, SCRATCH_SIZE, scratch, NULL);

		printf("%s device name : %s \n",gpu?"GPU":"CPU", scratch);
	}

	c_device = clock();

	// Create a compute context
	//

	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (!context) {
		printf("Error: Failed to create a compute context!\n");
		return EXIT_FAILURE;
	} else {
		cl_uint maxComputeUnits;

		clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS,
				sizeof(maxComputeUnits), &maxComputeUnits, NULL);
		printf("Parallel compute units: %d\n", maxComputeUnits);

	}
	// Create a command commands
	//
	c_context = clock();

	commands = clCreateCommandQueue(context, device_id, 0, &err);
	if (!commands) {
		printf("Error: Failed to create a command commands!\n");
		return EXIT_FAILURE;
	}

	// Create the compute program from the source buffer
	//
	c_command = clock();

	program = clCreateProgramWithSource(context, 1,
			(const char **) &KernelSource, NULL, &err);
	if (!program) {
		printf("Error: Failed to create compute program!\n");
		return EXIT_FAILURE;
	}

	// Build the program executable
	//

	c_program = clock();
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
				sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		exit(1);
	}
	c_build = clock();

	// Create the compute kernel in the program we wish to run
	//
	kernel = clCreateKernel(program, "square", &err);
	if (!kernel || err != CL_SUCCESS) {
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}
	c_kernel = clock();

	// Create the input and output arrays in device memory for our calculation
	//
	input = clCreateBuffer(context, CL_MEM_READ_ONLY,
			sizeof(float) * kernelcount, NULL, NULL);
	output = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
			sizeof(float) * kernelcount, NULL, NULL);
	if (!input || !output) {
		printf("Error: Failed to allocate device memory!\n");
		exit(1);
	}

	// Set the arguments to our compute kernel
	//
	err = 0;
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
	err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &kernelcount);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to set kernel arguments! %d\n", err);
		exit(1);
	}

	// Get the maximum work group size for executing the kernel on the device
	//
	err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE,
			sizeof(local), &local, NULL);
	if (err != CL_SUCCESS) {
		printf("Error: Failed to retrieve kernel work group info! %d\n", err);
		exit(1);
	} else
		printf("workgroup size %lu\n", local);

	int batch = count / kernelcount;
	if (verbose)
		printf("Number of Batch %d - Batch Size %ld - steps %lu\n",batch,kernelcount, count);

	c_startcopydata = (clock_t*) malloc(sizeof(clock_t) * batch);
	c_copydata = (clock_t*) malloc(sizeof(clock_t) * batch);
	c_execute = (clock_t*) malloc(sizeof(clock_t) * batch);
	c_readback = (clock_t*) malloc(sizeof(clock_t) * batch);

	c_read = clock();

	for (i = 0; i < batch; i++) {

		// td[i].input = &input[i * (int(count)/int(thread))];
		//	td[i].output = &output[i * (int(count)/int(thread))];
		//td[i].count = ((i+1 == thread) && (count % thread !=0)) ?(int(count)/int(thread))+ count % thread :(int(count)/int(thread)) ;

		// Write our data set into the input array in device memory
		//
		c_startcopydata[i] = clock();
		err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0,
				sizeof(float) * kernelcount,
				&data[(long)i * (long(count) / long(batch))], 0, NULL, NULL);
		if (err != CL_SUCCESS) {
			printf("Error: Failed to write to source array! %ld %d %ld %d %ld\n",kernelcount, i, count,batch,(long)i * (long(count) / long(batch)));
			exit(1);
		}
		c_copydata[i] = clock();
		// Execute the kernel over the entire range of our 1d input data set
		// using the maximum number of work group items for this device
		//

		global = kernelcount;
		int k;

		for (k = 0; k < 1; k++) {
			err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global,
					&local, 0, NULL, NULL);
			if (err) {
				printf("Error: Failed to execute kernel!\n");
				return EXIT_FAILURE;
			}
		}
		// Wait for the command commands to get serviced before reading back results
		//
		clFinish(commands);
		c_execute[i] = clock();
		//printf("batch %d done ...\n",i);
		// Read back the results from the device to verify the output
		//
		err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0,
				sizeof(float) * kernelcount,
				&results[i * (int(count) / int(batch))], 0, NULL, NULL);
		if (err != CL_SUCCESS) {
			printf("Error: Failed to read output array! %d\n", err);
			exit(1);
		}
		c_readback[i] = clock();
	}

	// Validate our results
	//
	if (validate) {
		printf("Done ....\n proceed with checking data ...\n");

		correct = 0;
		notcorrect = 0;
		for (i = 0; i < count; i++) {
			if (isSimilar(log(sqrt((data[i] * data[i])/2)), results[i]))
				correct++;
			else {
				notcorrect++;
			if (verbose == 2)
				printf(
						"index %d data[index] %f verif[index] %.6f != result[index] %.6f\n",
						i, data[i], log(sqrt((data[i] * data[i])/2)), results[i]);
			}
		}

		// Print a brief summary detailing the results
		//
		printf("Computed '%d' correct %d not correct values!\n", correct,
				notcorrect);
	}
	c_test = clock();
	// Shutdown and cleanup
	//
	clReleaseMemObject(input);
	clReleaseMemObject(output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	c_end = clock();
	auto endclock = high_resolution_clock::now() - beginclock;

	if (verbose) {
		double total;
		int i;
		//We care more about timing data
		printf("%8.5f\tseconds to allocate memory\n",
				(double) (c_memcreate - start) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to initialize memory\n",
						(double) (c_meminit - c_memcreate) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to find devices\n",
				(double) (c_device - c_meminit) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to create context\n",
				(double) (c_context - c_device) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to create command queue\n",
				(double) (c_command - c_context) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to find create program\n",
				(double) (c_program - c_command) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to build program\n",
				(double) (c_build - c_program) / CLOCKS_PER_SEC);
		printf("%8.5f\tseconds to construct kernel\n",
				(double) (c_kernel - c_build) / CLOCKS_PER_SEC);
		for (i = 0, total = 0; i < batch; i++)
			total += (double) (c_copydata[i] - c_startcopydata[i])
			/ CLOCKS_PER_SEC;
		printf("%8.5f\tseconds to populate input queues\n", total);
		for (i = 0, total = 0; i < batch; i++)
			total += (double) (c_execute[i] - c_copydata[i]) / CLOCKS_PER_SEC;
		printf("%8.5f\tseconds to execute program\n", total);
		for (i = 0, total = 0; i < batch; i++)
			total += (double) (c_readback[i] - c_execute[i]) / CLOCKS_PER_SEC;
		printf("%8.5f\tseconds to read output queues\n", total);
		printf("%8.5f\tseconds to test results sequentially\n",
				(double) (c_test - c_readback[batch - 1]) / CLOCKS_PER_SEC);
		printf("===================================================\n");
		printf("%8.5f\ttotal seconds to run\n",
				(double) (c_end - start) / CLOCKS_PER_SEC);


	}


	std::cout << "Elapsed time: " << duration<double, std::ratio<1>>(endclock).count() << ".\n";


	printf("bye ...\n");
	return 0;
}
