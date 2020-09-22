# Task C

## semaphore time pshared=1
|-------|---------|
|real	|0m19,674s|
|user	|0m18,998s|
|sys	|0m20,197s|

## semaphore time  pshared=0
|-------|---------|
|real	|0m11,211s|
|user	|0m14,121s|
|sys	|0m8,207s |


## non-semaphore time
|-------|--------|  
|real	|0m0,183s|  
|user	|0m0,362s|  
|sys	|0m0,000s|  

1) Semaphore version uses more system time, because the threads spends time waiting for resources to become available.\
2) Semaphore version uses more user time, because the read-write operations no longer overlap, and the sum of all read-write operations now take longer time.\
3) real time is lower than sum of user and sys because user and sys time takes the time of both threads and add them, while real time defines the time the program actually uses to execute because the threads are running concurrently.