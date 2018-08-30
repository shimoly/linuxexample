System Calls
============================================

A listing of system calls for your version of the Linux kernel is in /usr/include/asm-generic/unistd.h 

* A library function Ordinary function that resides in a library external to your program
* A system call: implemented in linux kernel. when a program makes a sytem call, the arguments are packaged up and handled to the kernel which takes over the excution until the call completes. GNU C library (the implementation of the standard C library provided with GNU/Linux systems) wraps Linux system calls with functions so that you can call them easily.


## strace
used to debug system calls Eg``` strace hostname ``` each line corresponds to a single system call.

## access
Determine wheather the calling process has access permission to a file. it checks a combination `read`, `write` and `execute`. it can also check file'e exitence.

To check file existence, a return value of 0 means the file exists. It returns -1 if the file doesn't exist or the directory in the path is inaccessible,  and `errno` is set to `ENOENT` if the file exists and ` EACCES ` if a directory is inaccessible.

```
access(
    path_to_file,
    F_OK
)
```

To check file permission, a return value of 0 means the process the requested permission. It returns -1 if the file doesn't exist or the caller doesn't have the specified permission,  and `errno` is set to `EROFS` if the filesystem is readonly and write is specified or ` EACCES `.

```
access(
    path_to_file,
    R_OK|W_OK|X_OK
)
```

## fcntl Locks and Other file operations

`fcntl` is the access point for several advanced operations on file descriptors.A read lock is placed on a readable file descriptor, and a write lock is placed on a writable file descriptor.

If you prefer fcntl not to block if the call cannot get the lock you requested use `F_SETLK ` instead of `F_SETLKW `.

## fsync
It takes one argument, a writable file descriptor, and flushes to disk any data written to this file. The ` fsync ` call doesn’t return until the data has physically been written

`fdatasync `can update faster

## getrlimit and setrlimit
allows to read and set process to process and set limits on the system resources that it can consume.` ulimit` enables to restrict the resource usage of programs you run

`hard limit` only super user may change the upper limit  
`soft limit` must be less than hard limit
Typically app limit `soft limit ` to throttle on the resources it uses.
```
rlimit
{
    rlim_cur, //is the soft limit
    rlim_max  // the hard limit
}
```
```
getrlimit (
    specifying the resource limit type,
    a pointer to rlimit that would be filled
)

setrlimit (
    specifying the resource limit type,
    a pointer to rlimit that holds the value
)
``` 

### Resource limit type
* `RLIMIT_CPU` max CPU time in seconds (might not be accurate), it it exceeds it is terminated with a `SIGXCPU`
* `RLIMIT_DATA ` max memory size additional allocation fails.
* ` RLIMIT_NPROC ` max child process, if the process fork after limit it would fail
* `RLIMIT_NOFILE` max numebr of file descriptores

# getrusage
`getrusage` system call retrieves process statistics from the kernel.
```
struct rusage (
    ru_time,  //a timeval struct, amount of user in seconds (time spent user program)
    ru_stime, //a timeval struct, amount of system in seconds (time spent during system calls)
    ru_maxrss // The largest amount of physical memoery occupied by the process data
)

struct rusage usage;
getrusage (RUSAGE_SELF,//RUSAGE_CHILDREN for children
     &usage);
```

# mlock
allows a program to lock some or all of its address space into physical memory. this prrevents linux from paging this memory to swap space.  
A time-critical program might lock physical memory because the time delay of paging memory out and back may be too long or too unpredictable. High-security applications may also want to prevent sensitive data from being written out to a swap file, where they might be recovered by an intruder after the program terminates.

To allocate 32 MB of address space
```
#include <sys/mman.h>

const int alloc_size = 32 * 1024 * 1024;
char* memory = malloc (alloc_size);
mlock (memory, alloc_size);

```
Note that simply allocating a page of memory and locking it with mlock doesn’t reserve physical memory for the calling process because the pages may be copy-on-write. Therefore, you should write a dummy value to each page as well:

```
size_t i;
size_t page_size = getpagesize ();
for (i = 0; i < alloc_size; i += page_size)
memory[i] = 0;

```
The write to each page forces Linux to assign a unique, unshared memory page to the process for that page.

To unlock a region call `unlock ` munlockall unlocks all memory locked by the current process using (mlock, mlockall)

A convenient way to monitor the memory usage of your program is to use the `top` command.n the output from top , the SIZE column displays the virtual address space size of each program (the total size of your program’s code, data, and stack, some of which may be paged out to swap space).The RSS column (for resident set size) shows the size of physical memory that each program currently resides in.The sum of all the RSS values for all running programs cannot exceed your computer’s physical memory size, and the sum of all address space sizes is limited to 2GB (for 32-bit versions of Linux).
# mprotect
` mmap ` system call to map a file into memory, the third argument to `mmap`is a permission `PROT_READ`, `PROT_WRITE` and `PROT_EXEC`, or `PROT_NONE` for no memory access. If a program attemts to perform an operation not allowed by the permission Linux would send a ` SIGSEGV ` (segmentation vialation) signal.  
After memory is mapped it can be modified by `mprotect` system call.
```
mprotect (
    address_of_memory_region,
    size_of_region,
    set_of_protection_flags
)
```
memory region must consists of entire pages, and must be aligned to the system, length should be a page size multiple.

```
int fd = open ("/dev/zero", O_RDONLY);
char* memory = mmap (NULL, page_size, PROT_READ, MAP_PRIVATE, fd, 0);
close (fd);

```
Later, your program could make the memory read-only by calling
`mprotect (memory, page_size, PROT_READ);`

An advanced technique to monitor memory access is to protect the region of memory using mmap or mprotect and then handle the SIGSEGV signal that Linux sends to the program when it tries to access that memory.

# nanosleep
High precsion version  UNIX `sleep (seconds_in_integer)` call. Actual precision is 10 milliseconds
```
struct timespec
{
    tv_sec,//integral number of seconds
    tv_nsec //integral milliseconds must be less than 10 ^9
}
```

The nanosleep call provides another advantage over sleep . As with sleep , the delivery of a signal interrupts the execution of nanosleep , which sets errno to EINTR and returns –1. However, nanosleep takes a second argument, another pointer to a struct timespec object, which, if not null, is filled with the amount of time remaining (that is, the difference between the requested sleep time and the actual sleep time)

# readlink
retrieves the target of a symbolic link. It takes three arguments, the path to the link, a buffer to receive the target of the link, i.e doesn't NUL terminate the buffer and length of that buffer.  
If the first argument to readlink points to a file that isn’t a symbolic link, readlink sets errno to EINVAL and returns –1

# sendfile

An efficient mechanism for copying data from one file descriptor to another.The file descriptors may be open to disk files, sockets, or other devices.

```
sendfile (
    fd_to_write,
    fd_to_read_from,
    offset_variable, // offset in the input file from which the read should start
    number_of_bytes
)

```

Other steps need to be taken to make the network transfer as efficient as possible, such as setting the socket parameters correctly

# setitimer: Setting Interval Timers

The setitimer system call is a generalization of the alarm call. It schedules the delivery of a signal at some point in the future after a fixed amount of time has elapsed.
```
setitimer (
    timer_code, // ITIMER_REAL or ITIMER_VIRTUAL or ITIMER_PROF,
    struct itimerval object,// specify the new setting for that timer
    if not null pointer to another struct itimerval object that receives the old timer settings
)

struct itimerval 
{
    it_value is a struct timeval field that contains the time until the time next expires and a signal is sent. If this is 0 timer is diasabled,  
    it_interval is another timeval field containing the value to which the timer will be reset after it expires. If this is 0 the timer will be disabled after it expires
}
```
# Sysinfo: Obtaining System Statistics
The sysinfo system call fills a structure `sysinfo` with system statistics.   
Some of the intersting fields are
```
sysinfo (
    uptime, //Time elapsed since the system booted, in seconds
    totalram, //Total available physical RAM
    freeram, //Free physical RAM
    procs //Number of processes on the system
)
```
# uname
Fills structure with various system information, including thecomputer’s network name and domain name, and the operating system
```
#include <sys/utsname.h>

struct utsname u;
uname (&u);
printf ("%s release %s (version %s) on %s\n", u.sysname, u.release,
u.version, u.machine);
```


