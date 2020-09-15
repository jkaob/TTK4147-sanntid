# Task B

## insert\_back over capacity without flag
The array_print function fails, but fetching array\_back and printing yields no errors.  
Hypothesis: The value at the addresses following the initialized array are initialized, without being under "control" of the array. At first glance, it might look alright, but the problem is that we're manipualting addresses we shouldn't be allowed to access. For example, if another array was initialized immediately preceeding the first obne, we might alkter the values of this array non-intentialloy by exceeding the capacity of the first array.

##  insert\_back over capacity with flag

Leak sanitizer detects memory leaks if we exceed capacity.
