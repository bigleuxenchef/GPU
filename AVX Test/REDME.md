# Crunching Numbers with Advanced Vector Extensions 

## Introduction

Much before GPU existed, processor designer always try to come with specific respond to need for performance depending on context. GPU has been overshadowing what industry knew for very long, the "darwin" therory of evolution has worked well to my mind in the chip space too. Just few things to remember :

- RISC architecture is to me one evolution that may have inspired GPU.
- DSP architecture was a super narrow specialized chip for digital signal processing and to attempt to parallelise more instruction in the same machine cycle. One of the evolution of DSP is SIMD (Single instruction, multiple data) then Advanced Vector Extensions.

Wikipedia is doing a good job educating us on something not new but that new generation may think that the revolution has just happened in the post internet time!

[:eyes:RISC architecture](https://en.wikipedia.org/wiki/Reduced_instruction_set_computer)
[:eyes:DSP Architecture](https://en.wikipedia.org/wiki/Digital_signal_processor)
[:eyes:AVX](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions)

[:eyes:CompilerAutovectorizationGuide source link](https://software.intel.com/sites/default/files/m/4/8/8/2/a/31848-CompilerAutovectorizationGuide.pdf)
[:eyes:CompilerAutovectorizationGuide copy link](31848-CompilerAutovectorizationGuide.pdf)


## using AVX

### Use case #1 


 
### Results single Precision


```
!!!Hello World OF Advanced Vector Extensions : Single Precision Test!!!
Effective Threads Runtime
 1.08624	seconds normal
 0.40506	seconds sse128
 0.22753	seconds sse256
Effective clock Runtime
Normal 1.1088036 s
SSE128 0.405116 s
SSE256 0.227802 s

```

```
# with maximum optimization and -ttree-vectorize

!!!Hello World OF Advanced Vector Extensions : Single Precision Test!!!
Effective Threads Runtime
 0.53997	seconds normal
 0.18437	seconds sse128
 0.17486	seconds sse256
Effective clock Runtime
Normal 0.541213 s
SSE128 0.184862 s
SSE256 0.174922 s
```

### Results Double Precision





```
# no compiler optimization enforce the vectorization disable to compare with and without optimization
!!!Hello World OF Advanced Vector Extensions : Double Precision!!!
Effective Threads Runtime
 1.38743	seconds normal
 0.82126	seconds sse128
 0.51806	seconds sse256
Effective clock Runtime
Normal 1.1387632 s
SSE128 0.821300 s
SSE256 0.518172 s

```

## Modern compiler are integrating vectorization in standard optimization configuration

using g++ and setting -tree-vectorize only



```
# with maximum optimization
!!!Hello World OF Advanced Vector Extensions : Double Precision!!!
Effective Threads Runtime
 1.41134	seconds normal
 0.63199	seconds sse128
 0.59083	seconds sse256
Effective clock Runtime
Normal 1.1416178 s
SSE128 0.634894 s
SSE256 0.592923 s

```

interesting to notice that we are a bit lost here because the optimazer has even optimized sse128!

```
# with maximum optimization and -ttree-vectorize
!!!Hello World OF Advanced Vector Extensions : Double Precision!!!
Effective Threads Runtime
 1.32498	seconds normal
 0.47684	seconds sse128
 0.47063	seconds sse256
Effective clock Runtime
Normal 1.1325111 s
SSE128 0.476849 s
SSE256 0.470666 s

```
