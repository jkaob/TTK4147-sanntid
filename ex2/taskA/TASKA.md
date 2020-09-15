# Task A

## What happens?
The computer freezes. Memory usage of program is 6.2 GiB.
## What is the difference between "memory" and "swap"?
Swap is located on the hard drive. It's virtual memory that is written to when available RAM doesn't suffice.
## Change x\_dim from 100
### to 1000
Program uses now only 3.8 GiB, and the computer doesn't freeze.
When dimension is samller than a certain threshold memory will actually be allocated, which happened with x\_dim = 100.  
However, when x\_dim=1000, it's sufficiently large for the program not to actually allocate before anything is initialized, and thus not too much memory is allocated.
### to 10000
Program uses now only 391,4 MiB, and the computer doesn't freeze.  
Reason is the same as earlier only amplified, (because even lower y\_dim)

## Init with zeros
The memory usage rises to 6.8 GiBs a result of countering the effect earlier mentioned. When actively initiliazing the elements to zero, memory will be actually allocated, thus taking up much memory.
Get segfault if not malloc first.
## When freeing

### Freeing before allocation complete
Not much difference.   
(Computer speeds up after closing terminal, even though program was finished.)
### Freeing after allocation complete

Not much difference