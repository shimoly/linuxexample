Devices
=========
Linux provides a mechanism by which processes can communicate with a device driver and through it with a hardware device via file like objects

# Device types
Device file aren't ordinary files. They represent data read from or written to device file that is communicated to the device driver from there to the device.

## Device files
### A character device
    represents a hardware device that reads and writes a serial stream of data  
    Eg. serial and parallel ports, tape drives, sound cards
### A block device
    represents a device that reads or write data in a fixed-size blocks
    Eg. Disk drive


# Device numbers
Linux identify devices using two numbers, major device number and mino device number.
## Major device number
    The major device driver specifies which driver the device corresponds to.
    The correspondance between major device driver to drivers is fixed and part of the linux kernel sources.
## Mino device number
    Minor device numbers distinguish individual device or components controlled by single driver

# Device Entries


You can create device entry in the file system using mknod command  
``` man 1 mknod ``` for the command  ``` man 2 mknod ``` for the sytem call
to create device using ` mknod ` command  
``` mknod path c for character and b for block device Majon no minor no ``` Eg ``` mknod ./lp0 c 6 0 ``` creates a device file for the first parallel port.

## The /dev directory
By convention, a GNU/Linux system includes a directory /dev containing the full complement of character and block device entries for devices that Linux knows about.

## Accessing Devices by Opening Files

In the case of character devices, it can be quite simple, Open the device as if it were a normal file, and read from or write to it.

If you have a printer connnected to the parallel port ``` cat document.txt > /dev/lp0 ```

``` 
int fd = open (“/dev/lp0”, O_WRONLY);
write (fd, buffer, buffer_length);
close (fd);

```
``` cat /usr/share/sndconfig/sample.au > /dev/audio ``` play the sound file


# Special Devices

## /dev/null
* Linux discards any data written to /dev/null ` verbose_command > /dev/null`, when output is not needed
* Reading from /dev/null always results in an end-of-file
* If you copy from /dev/null to another file, the destination will be a zero-length file

## /dev/zero
behaves as if it were an infinitely long file filled with 0 bytes

## /dev/full
* The /dev/full entry is primarily useful to test how your program behaves if it runs out of disk space while writing to a file.

* A write to /dev/full fails and sets errno to ENOSPC , which ordinarily indicates that the written-to device is full.

## Random Number Devices
The special devices `/dev/random` and `/dev/urandom` provide access to the Linux kernel’s built-in random number generation facility.
The difference between the two devices exhibits itself when Linux exhausts its store of randomness. If you try to read a large number of bytes from /dev/random but don’t generate any input actions you don’t type, move the mouse, or perform a similar action), Linux blocks the read operation. Only when you provide some randomness does Linux generate some more random bytes and return them to your program


` od -t x1 /dev/random `
A read from `/dev/urandom ` , in contrast, will never block. If Linux runs out of randomness, it uses a cryptographic algorithm to generate pseudorandom bytes from the past sequence of random bytes. Although these bytes are random enough for many purposes, they don’t pass as many tests of randomness as those obtained from `/dev/random`.


## Loopback devices
   Enables to simulate block devices using an ordinary disk file. You can construct a file system on the device and then mount that file system as you would mount the file system on an ordinary disk or partition. Such a file system, which resides in its entirety within an ordinary disk file, is called a virtual file system.

To construct a 10MB file named disk-image , invoke the following:
```
% dd if=/dev/zero of=/tmp/disk-image count=20480
20480+0 records in
20480+0 records out
```

```
% ls -l /tmp/disk-image
-rw-rw---- 1 root root 10485760 Mar 8 01:56 /tmp/disk-imag
```
to construct a ext2 file system 

```
mke2fs -q /tmp/disk-image
```
Mount the device as loopback

```
% mkdir /tmp/virtual-fs
% mount -o loop=/dev/loop0 /tmp/disk-image /tmp/virtual-fs
```
check 

```
df -h /tmp/virtual-fs
Filesystem          Size     Used   Avail   Use%    Mounted on  
/tmp/disk-image     9.7M      13k   9.2M    0%      /tmp/virtual-fs
 
```
unmount it

```
% cd /tmp
% umount /tmp/virtual-fs
```
# PTYs
Like the /dev directory, /dev/pts contains entries corresponding to devices. But unlike /dev , which is an ordinary directory, /dev/pts is a special directory that is created dynamically by the Linux kernel.

```
ps -o pid,tty,cmd

```

Try opening a new terminal window, and determine its PTY number by invoking ` ps -o pid,tty,cmd` . From another window, write some text to the PTY device. For example, if the new terminal window’s PTY number is 7, invoke this command from
another window:
```% echo ‘Hello, other window!’ > /dev/pts/7```

#ioctl
system call is an all-purpose interface for controlling hardware devices.  
Using ioctl require detailed understanding of the driver.
```
ioctl
(
    file_discriptor_which_would_be_opened_to_the_device_that_want_to_control,  
    request_code_that_indicates_the_operation: listed in ioctl_list man page


)
```
Eject a CD-ROM
```
#include <fcntl.h>
#include <linux/cdrom.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main (int argc, char* argv[])
{
/* Open a file descriptor to the device specified on the command line.
int fd = open (argv[1], O_RDONLY);
/* Eject the CD-ROM. */
ioctl (fd, CDROMEJECT);
/* Close the file descriptor. */
close (fd);
return 0;
}
```
If CD-ROM device is /dev/hdc .To eject the disk from the drive, invoke this line:
` % ./cdrom-eject /dev/hdc `
