# OS_LAB_PROJECT

We have implemented Context Switching and RR scheduling. We need to do Context Switching whenever Process switch takes place. Here Scheduling is done, so whenever time slice of one process is completed, processor is allocated to next process,for this we need to save the state of process so that next time it should run from where it was left. We have implemented context switch for Scheduling and I/O interrupts
Process Module and files:


----EXECUTION----
Main.c It contains all operations such as process creation, scheduling ,context switch,updating PCB and GUI,which is implemented using GTK. To run : /gcc ‘pkg-config gtk+-3.0 --cflags‘ main.c stack implementation.c queue implementation.c -o os ‘pkg-config gtk+-3.0 --libs‘

./os

stack implementation It contains stack implementation for push,pop operations

queue implementation It contains queue implementation for enqueue, dequeue operations process*.txt it contains instruction of process.

This code can run in linux. Only main.c needs to be runned.You need to install GTK3.0 to run this.


---IMPLENTATION---

• Four processes are added to four different text files and in which instruction for process is given. Ready queue is formed using circular queue. Now scheduling is done,for every quantum when process runs, its PC is incremented,completed time for particular running process increases by quantum, 2 instructions are executed in one quantum and value of variables are PUSH-

ed in stack of that process.Whenever that process again gets processor to execute, value of this registers is used. After this, next process which is in ready queue gets turn and execute instructions in similar way. This will continue until any of the process gets blocked. Whenever any process gets blocked, it is added to block queue and processor is given to next process. When needed resource for blocked process is free/available, it is again added o ready queue. As a result we are showing before and updated PCB of each process. Resources are blocked and released through GUI.


----CONCLUSION----

Context switching is important part of OS. As without context switching there is no use of different scheduling algorithms. If concept of context switch is not implemented then forcibly we have to use FCFS(first come first server) scheduling. RR(round robin) and other scheduling algorithms are not possible to implement without switching. And SRT(shortest remaining time) and SPN(shortest process next) and HRRN (highest response ratio next) are not applicable in real life as we do not know service time.In FCFS no need of context switching as once process enters it gets executed. For large value of quantum RR will behave like FCFS. So, to show context switching in better way RR with small quantum value is preferred.
