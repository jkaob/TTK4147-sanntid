# Task D

The bug is that the singleton isn't finished initializing before it is marked as ready - therefore the values are 0000 when the first threads are trying to getch it.