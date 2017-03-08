##Results Running on macbookpro intel hd 6100 vs. i5

```
Verbose 1
GPU device name : Intel(R) Iris(TM) Graphics 6100 
Parallel compute units: 48
workgroup size 256
Number of Batch 4 - Batch Size 128000000 - steps 512000000
 0.00003	seconds to allocate memory
 2.14108	seconds to initialize memory
 0.00501	seconds to find devices
 0.00713	seconds to create context
 0.00454	seconds to create command queue
 0.00001	seconds to find create program
 0.00092	seconds to build program
 0.00006	seconds to construct kernel
 0.43874	seconds to populate input queues
 0.14194	seconds to execute program
 0.55849	seconds to read output queues
 0.00001	seconds to test results sequentially
===================================================
 3.38966	total seconds to run
Elapsed time: 5.92196.
bye ...
```

```
Verbose 1
CPU device name : Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz 
Parallel compute units: 4
workgroup size 128
Number of Batch 4 - Batch Size 128000000 - steps 512000000
 0.00003	seconds to allocate memory
 2.16142	seconds to initialize memory
 0.00106	seconds to find devices
 0.00003	seconds to create context
 0.00001	seconds to create command queue
 0.00001	seconds to find create program
 0.00130	seconds to build program
 0.00020	seconds to construct kernel
 4.62529	seconds to populate input queues
 5.29466	seconds to execute program
 2.15032	seconds to read output queues
 0.00000	seconds to test results sequentially
===================================================
14.31270	total seconds to run
Elapsed time: 8.2973.
bye ...
```
