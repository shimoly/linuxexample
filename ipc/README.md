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

```semop ``` supports both wait and post operation
```
semop
   (
    semaphore_set_identifier,
    an array off struct sembuf which specify the operation,
    lenght of the array)
``` 


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

## Mappped Memory
 
allows processes to communicate via a shared file.
Mapped memory forms an association between a file and a process memory.  
Linux splits the file into page-sized chunks and then copies them into virtual memory pages so that they can be made available in a process’s address space.  

```
mmap
  (
    address at which you would like linux to map the file into your process’s address space; the value NULL allows Linux to choose an available start address,  
    the length of the map in bytes byte,
    the protection on the map address, it consists of bitwise or of PROT_READ, PROT_WRITE and PROT_EXEC, 
    A flag value that specifies additional options
    the file descriptor opened to the file to be mapped,
    the offset from the begining of the file
  )
```

the flag value is a bitwise "or" of the following constraints
* MAP FIXED 
  * Linux uses the address  specified to map the file, rather than treating it as a hint.
  * The address must be page aligned
* MAP_PRIVATE 
  * Write to memory range should not be written back to the attached file, rather to private copy.
  * No other process see the changes
  * This mode may not be used  with MAP_SHARED
* MAP_SHARED
  * Writes immeddiately refelected in the underlying file rather than buffering writes
  * Use this mode when using mapped memory for IPC
  * This Mode may not be used with MAP_PRIVATE


if the call succeeds it mmap returns a pointer to the begining of the memory  
On failure it returns MAP_FAILED

When finished with memory mapping, release it by 
```
munmap
  (
    start_address,
    length of the mapped memory regions
  )

```



If MAP_SHARED flag is set any writes is transferred to the underlying file, and visible to other process, otherwise linux might buffer.

To force buffered writes into disks using ``` msync ``` it's two args are the same as ``` munmap ``` the third parameter can be

``` MS_ASYNC ``` The update is scheduled but not necessariy run before the call returns   
``` MS_SYNC ``` The update is immediate, call to ``` msync ``` blocks until it is done. Can't be used with ``` MS_ASYNC ```  
``` MS_INVALIDATE ``` all other files are invalidated so they can see the updated value  

For example, to flush a shared file mapped at address mem_addr of length mem_length bytes, call this:  
``` msync (mem_addr, mem_length, MS_SYNC| MS_INVALIDATE); ```

As with shared memory segments, users of memory-mapped regions must establish and follow a protocol to avoid race conditions.  
For example, a semaphore can be used to prevent more than one process from accessing the mapped memory at one time.   
Alternatively, you can use fcntl to place a read or write lock on the file.



### Other use of mmap

* replacement of ```read ``` and ```write```, map the file and scan the memory (may run faster)
* BUild data structure in memory mapped file
* to map special "/dev/zero" file into memory, if programs needs chunk of zero


## Pipes

- allows unidirectioanl communication
- serial device, data is always read from the pipe in the same order it was written.
 

``` % ls |  less ``` creates a pipe connecting the standard output of the ls process with the input of the less command.  
writer waits until space to write is available and reader blocks until data to read is availble, this sync the two process.

### Creating pipes

To create a pipe supply an integer of size 2.  
The call to pipe stores the reading file descriptor in arra postion 0 and the writing in position 1.

```
int pipe_fds[2];
int read_fd, write_fd;

pipe(pipe_fds);
read_fd = pipe_fds[0];
write_fd = pipe_fds[1];
``` 
Data written to the file descriptor ```read_fd``` can be read back from ```write_fd```  

A call to pipe creates a file descriptor that is valid only to the within that process and child process.  
when a process calls ```fork``` file process are copied to the new process.

Using ``` dup2 ``` it is possiable to equate one file descriptor to the another.

```
dup2(fd, STDIN_FILENO)

```
``` STDIN_FILENO ``` represents the file descriptor for the standard input that has a value 0
The call closes standard inputs and then reopens it as a deplicatie of fd so that the two mau be used intechangably.


The common use of pipes is to send or receive data from a program being run in a subprocess.  


### popen and pclose


The ```popen``` and ```pclose``` functions ease this paradigm by eliminating the need to invoke `pipe`, `fork` , `dup2`, `exec`, and `fdopen`   



```

#include <stdio.h>
#include <unistd.h>
int main ()
{
FILE* stream = popen (“sort”, “w”); \\creates a child process executing the sort command, replacing calls to pipe fork dup2 and execlp
the second argument "w" indicates this process wants to write to the child process.  
return from popen is one end of the pipe.
The other end is connected to the child process standard input.
fprintf (stream, “This is a test.\n”);
fprintf (stream, “Hello, world.\n”);
fprintf (stream, “My dog has fleas.\n”);
fprintf (stream, “This program is great.\n”);
fprintf (stream, “One fish, two fish.\n”);
return pclose (stream);\\ closes stream returned by the popen,after closing popen waits for the child process to terminate return tthe stattus value.
}
```
