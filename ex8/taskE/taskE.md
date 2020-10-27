mutex value: 0
Broadcasting semaphore
Task high started, going to sleep. 
Task low trying to lock mut A, val: 0 
Task low locked mut A, start busy wait, val: 1 
Task low bw 1
Task low bw 2
Task low trying to lock mut B, val: 1 
Task low locked mut B, start busy wait
Task low bw 1
Task low bw 2
Task low releasing mut B
Task high trying to lock mut B
Task high trying to lock mut A
Task low releasing mut A
Task high locked mut A, start busy wait 
Task high releasing mut A
Task high releasing mut B
task high finishedtask low finishedProgram finished
Semaphores deleted

