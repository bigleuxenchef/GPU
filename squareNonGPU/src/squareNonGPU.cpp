//============================================================================
// Name        : squareNonGPU.cpp
// Author      : rumi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>
#include <pthread.h>
#include <cmath>
#include <sys/time.h>
#include <mach/clock.h>
#include <mach/mach.h>
#include <chrono>

using namespace std;

#define ZERO 1e-10
#define SMALL 1e-4
#define DELTA 1e-10
#define isZero(A) ( (A < ZERO) && (A > -ZERO) )
#define isSmall(A) ( (A < SMALL) && (A > -SMALL) )
#define isSame(A, B) ( ((A-B) < ZERO) && ((A-B) > -ZERO) )
#define isSimilar(A, B) ( ((A-B) < SMALL) && ((A-B) > -SMALL) )
#define isBetween(A, B, C) ( ((A-B) > -ZERO) && ((A-C) < ZERO) )

#define NUMBER_THREAD 4
unsigned int thread_count;

struct thread_data {
	float *input, *output;
	int count;
	pthread_t thread_id;
	int thread_index;

};


double calcpi(int n){
  double inside = 0;
  srand(1);
  for(int i = 0; i < n; ++i){
    double x = ((double) rand() / (RAND_MAX));
    double y = ((double) rand() / (RAND_MAX));
    if( x*x + y*y < 1.0 ){
      inside = inside + 1;
    }
  }
  return(4 * inside / n);
}



void square(float* input, float* output, const unsigned int count) {
	int i;
	for (i = 0; i < count; i++)
	{
		output[i] = input[i] * input[i];
		output[i] = sqrt(output[i]);
		output[i] = log(output[i]);
		//output[i] = exp(output[i]) * calcpi(1000);

	}
	cout << "square " << input[0] << " : " << output[0] << " --- " << input[count-1] << " : " << output[count-1] << endl;
}

void *squareThreadWrapper(void *threadarg){
	struct thread_data *my_data;
	my_data=(struct thread_data *) threadarg;

	printf("squareThreadWrapper() count :%d\n ", my_data->count);

	square(my_data->input,my_data->output, my_data->count);

	printf("squareThreadWrapper() exiting Thread : %d\n", my_data->thread_index);

	__sync_fetch_and_sub(&thread_count,1);
	pthread_exit(NULL);

}


int main(int argc, char* const argv[]) {

	pthread_t *threads;
	struct thread_data *td;
	float *input, *output;
	int i,count,thread;
	int rc;
	clock_t start,c_memcreate, c_meminit, c_command, c_read, c_program, c_end;

	cout << "!!!Hello World!!! This is about time" << endl;

	if (argc > 1) {
		try {
			count = std::stoi(argv[1]);
		} catch (exception& e) {
			cout << "Sample size is not a number : " << argv[1] << endl;
			exit(0);
		}
	}
	else {
		count = 1000000;
	}

	if (argc > 2) {
			try {
				thread = std::stoi(argv[2]);

			} catch (exception& e) {
				cout << "Sample thread number is not a number : " << argv[2] << endl;
				exit(0);
			}
		}
		else {
			thread = NUMBER_THREAD;
		}
	 thread_count=thread;

	start = clock();

	threads=(pthread_t *)malloc(sizeof(pthread_t)*thread);


	td=(struct thread_data *)malloc(sizeof(struct thread_data)*thread);

	input = (float*) malloc(sizeof(float) * count);
	output = (float*) malloc(sizeof(float) * count);
	c_memcreate = clock();


	for (i = 1; i < count; i++)
		input[i] = i;



	auto start_time = chrono::high_resolution_clock::now();


	c_meminit = clock();

	for(i=0;i < thread;i++)
	{
	cout << "main() creating Thread, " << i << endl;

	td[i].input = &input[i * (int(count)/int(thread))];
	td[i].output = &output[i * (int(count)/int(thread))];
	td[i].count = ((i+1 == thread) && (count % thread !=0)) ?(int(count)/int(thread))+ count % thread :(int(count)/int(thread)) ;
    td[i].thread_index=i;

	cout << td[i].count<<endl;

	rc=pthread_create(&td[i].thread_id,NULL,squareThreadWrapper,(void*)&td[i]);

	//square(input, output, count);

	}


	 do {
	    __sync_synchronize();
	  } while (thread_count);

	 printf("All threads done\n");


	c_end = clock();
	auto end_time = chrono::high_resolution_clock::now();

	int verbose = 1; //TODO implement proper parameters to call the program
	if (verbose) {
			//We care more about timing data
			printf("%8.5f\tseconds to allocate memory\n",
					(double) (c_memcreate - start) / CLOCKS_PER_SEC);
			printf("%8.5f\tseconds to initialize memory\n",
							(double) (c_meminit - c_memcreate) / CLOCKS_PER_SEC);


			printf("%8.5f\tCumulated thread time in seconds to execute program\n",(double) (c_end - c_meminit) / CLOCKS_PER_SEC);

			printf("%lld.%5lld\tEffective Threads Runtime to execute program\n", chrono::duration_cast<chrono::seconds>(end_time - start_time).count(),
						chrono::duration_cast<chrono::microseconds>(end_time - start_time).count());


			printf("===================================================\n");
			printf("%8.5f\ttotal Threads execution times in seconds to run\n",
					(double) (c_end - start) / CLOCKS_PER_SEC);

			printf("===================================================\n");



		}
		printf("bye ...\n");

}
