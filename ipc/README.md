Inter process communication.
============================


# Shared Memory 

ipcs provides information about the communication facilities.  

`% _ipcs -m` 


------ Shared Memory Segments --------  

| key | shmid | owner | perms | bytes| nattch | status|  
| --- | --- | --- | --- | --- | --- | --- |  
| 0x00000000 | 1627649 | user | 640 | 25600 | 0 |  |    


If this memory segment was erroneously left behind by a program, you can use the ipcrm command to remove it.  
`%ipcrm shm 1627649`


## Pros
* Shared memory permits fast bidirectional communication among any number of process.   
* Each user can both read and write
* A program must establish and follow some protocol to prevent race condtion


## Cons
* Linux doesn't guarantee exclusive access even if you create a new shared segment with IPC_PRIVATE
* all the process must use the same key


# Process semaphore

Semaphore for threads are counters that permit thread synchrnizing multiple threads.


`semget` allocates semaphore
`semctl` deallocates semaphore

The last process to use a semaphore must explicitly remove it.

## Wait and Post Operation


`semop(semaphore_set_identifier, an array off struct sembuf which specify the operation, lenght of the array)` suppports both wait and post operation  


The fields of struct sembuf are listed here:
* sem_num is the semaphore number in the semaphore set on which the operation is performed.  
* sem_op is an integer that specifies the semaphore operation.  
  * If sem_op is a positive number, that number is added to the semaphore value immediately.  
  * If sem_op is a negative number, the absolute value of that number is subtracted from the semaphore value.  
  * If this would make the semaphore value negative, the call blocks until the semaphore value becomes as large as the absolute value of sem_op (because some other process increments it).  
  * If sem_op is zero, the operation blocks until the semaphore value becomes zero.
* sem_flg is a flag value. Specify IPC_NOWAIT to prevent the operation from blocking; if the operation would have blocked, the call to semop fails instead. If you specify SEM_UNDO , Linux automatically undoes the operation on the
semaphore when the process exits  

### Debug semaphres
```%ipcrm sem 5790517 ``` removes the semaphre number described







