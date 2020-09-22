# Task E
Same bug:  
The bug is that the singleton isn't finished initializing before it is marked as ready  
values are now 15000 when the first threads are trying to getch it (because 15 =1111?).