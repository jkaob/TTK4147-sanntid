TASK A
====

## sleep()
real	0m1,002s  
user	0m0,002s  
sys	    0m0,000s  

## busy_wait() with clock_gettime(CLOCK_MONOTONIC, ...)
real	0m1,002s  
user	0m1,001s  
sys	    0m0,000s  

## busy_wait() with times()
real	0m1,007s  
user	0m0,277s  
sys	    0m0,730s  

## conclusion
"sys" is kernel time, and times() uses most of this, because it measures the CPU time spent executing inside the kernel.