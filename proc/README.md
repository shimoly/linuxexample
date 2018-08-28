Proc window into the running Linux kernel
=========================================
Files in the /proc file system don’t correspond to actual files on a physical device. Instead, they are magic objects that behave like files but provide access to parameters, data structures, and statistics in the kernel.  

# Extract information

A simple way to extract a value from this output is to read the file into a buffer and parse it in memory using `sscanf`

Example: read_cpu_info.c
# Process Entries
Each running process has a corresponding directory in proc folder, folder name is the process id.  
Each directory apear and disapear as the process start and terminate.
Each directory contains serveral entries providing access and information about the process.

## maps
For each mapped file maps display the range of addresses in the process address space.
The maps table for each process displays the executable running in the process, any loaded shared libraries, and other files that the process has mapped in.
## root
sysmbolic link to /, the system root directory. The root directory for system can be changed with ` chroot`
## stat
contains status and statistical information about the process. The same as status but in a raw numerical format and in a single line.

## Cpu entry
apears only in SMP linux kernel, it contains a breakdown of process time (user and system) by CPU

## /proc/self
It is a sybolink to the /proc directory corresponding to the current process, makes it easy to find information about its own process

## Process argument list
The `cmdline` entry contains the process argument list, the argument are presented as a single character string with the argument separted with  `NUL`
```
NUL is char with value 0 represented as '\0'
NULL is a pointer with the value 0 represented as (void *)0
```


## Process Environment
The environ entry contains a process’s environment. As with cmdline, the individual environment variables are separated by `NULs`.The format is `VARIABLE=value`


## Process executable
Determine which excutable is running in a reliable way. The first argument might not be reliable way to know the name of executable that is running.

## Process file descriptors

the ` fd ` entry is a subdirectory that contains entries for the file descriptor opened by the process.

``` ls -l /proc/2570/fd ``` list all the opened fd by the process 2570

## Process Memory Statistics
``` statm ``` contains a list of 7 numbers separated by space, Each number is a counte of the number of pages memory used in particular category.
* The total process size
* The size of the process resident in physical memory
* The memory shared with other processes that is, memory mapped both by this process and at least one other (such as shared libraries or untouched copy on-write pages)
* The text size of the process—that is, the size of loaded executable code
* The size of shared libraries mapped into this process
* The memory used by this process for its stack
* The number of dirty pages—that is, pages of memory that have been modified by the program

## Process Statistics
The status entry contains a variety of information about the process, formatted for comprehension by humans. Among this information is the 
* process ID
* parent process ID
* the real and effective user and group IDs
* memory usage
* bit masks
* specifying which signals are caught, ignored, and blocked.

# Hardware information
## CPU Information
``` /proc/cpuinfo ``` contains info about the CPU
## Device Information
``` /proc/devices ``` lists the major device numbers for character and block devices
## PCI Bus information
``` /proc/pci ``` lists summary of devices attached to the PCI bus or buses
## Seral port Information
``` /proc/tty/driver/serial ``` list configuration info and statistics about serial ports can be obtained as well as modified using ```setserial```

# Kernel Information
## Version Information
```
% cat /proc/sys/kernel/ostype
Linux
% cat /proc/sys/kernel/osrelease
4.15.0-33-generic
% cat /proc/sys/kernel/version
#36-Ubuntu SMP Wed Aug 15 16:00:05 UTC 2018
```
## Hostname and Domain Name
```
% cat /proc/sys/kernel/ostype
Linux
% cat /proc/sys/kernel/osrelease
4.15.0-33-generic
% cat /proc/sys/kernel/version
#36-Ubuntu SMP Wed Aug 15 16:00:05 UTC 2018
```

## Memory Usage
The /proc/meminfo entry contains information about the system’s memory usage.Information is presented both for physical memory and for swap space

# Drives, Mounts, and File Systems
``` /proc/filesystems``` entry displays the file system types statically linked and loaded to the kernel

## Drives and Partitions
Ide devices
```
Contrller   Device  Subdirectory
Primary     Master  /proc/ide/ide0/hda/
Primary     Slave   /proc/ide/ide0/hdb/
Secondary   Master  /proc/ide/ide1/hdc/
Secondary   Slave   /proc/ide/ide1/hdd/
```

## Mounts

The ` /proc/mounts ` file provides a summary of mounted file systems. Each line corresponds to a single mount descriptor and lists the mounted device, the mount point, and other information. Note that ` /proc/mounts`  contains the same information as the ordinary file ` /etc/mtab`  , which is automatically updated by the mount command.

## Locks
The ` /proc/locks ` entry describes all the file locks currently outstanding in the system, each row corresponding to one lock.

# System statistics
/proc/loadavg information about system load.
The first three numbers represent the number of active tasks on the system—processes that are actually running—averaged over the last 1, 5, and 15 minutes.  
The next entry shows the instantaneous current number of runnable tasks—processes that are currently scheduled to run rather than being blocked in a system call—and the total number of processes on the system.The final entry is the process ID of the process that most recently ran
