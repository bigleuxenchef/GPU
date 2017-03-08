##Results Running on macbookpro intel hd 6100 vs. i5

```
Verbose 1
GPU device name : Intel(R) Iris(TM) Graphics 6100 
Parallel compute units: 48
workgroup size 256
Number of Batch 40 - Batch Size 12800000 - steps 512000000
 0.00002	seconds to allocate memory
 2.43446	seconds to initialize memory
 0.00592	seconds to find devices
 0.01054	seconds to create context
 0.00700	seconds to create command queue
 0.00001	seconds to find create program
 0.00084	seconds to build program
 0.00007	seconds to construct kernel
 1.84393	seconds to populate input queues
 0.08666	seconds to execute program
 0.56202	seconds to read output queues
 0.00000	seconds to test results sequentially
===================================================
 4.96486	total seconds to run
Elapsed time: 7.4198.
bye ...
```

```
Verbose 1
CPU device name : Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz 
Parallel compute units: 4
workgroup size 128
Number of Batch 40 - Batch Size 12800000 - steps 512000000
 0.00003	seconds to allocate memory
 2.41214	seconds to initialize memory
 0.00174	seconds to find devices
 0.00014	seconds to create context
 0.00001	seconds to create command queue
 0.00002	seconds to find create program
 0.00246	seconds to build program
 0.00034	seconds to construct kernel
 6.77984	seconds to populate input queues
 3.01938	seconds to execute program
 2.27151	seconds to read output queues
 0.00001	seconds to test results sequentially
===================================================
14.49694	total seconds to run
Elapsed time: 8.50505.
bye ...
```
