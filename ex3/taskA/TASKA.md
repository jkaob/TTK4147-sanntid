# Task A

## What happens?
Both local vars are 50 000 000.\
However the global vars differ from each other from each thread and both values are only slightly higher than 50 000 000.\
This is because both threads read the global var simultaneously and write to the variable without noticing that the other thread has written something just before.\


