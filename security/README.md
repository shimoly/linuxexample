Security
========================================

if you store permission in variable use `mode_t ` to store the permission.

For file
```
S_IWUSR corresponds to write permission for the owning user
S_IRGRP corresponds to read permission for the owning group
S_IXOTH excute permission to the other group
```
For directory
```
S_IWUSR allowed to see the list of files
S_IRGRP the group is allowed to add/ remove files from the directory, Even if the group has not the permission to modify the file, unless the sticky bits are set in that case you need to be the owner.
S_IXOTH allowed to the enter the directory
S_ISVTX is sticky bit set
```
Example
```

#include <sys/stat.h>
stat (filename, &buf);
/* If the permissions are set such that the file’s owner can write to it, print a message. */
if (buf.st_mode & S_IWUSR)
// stat returns -1 and set errno if it fails
printf (“Owning user can write `%s’.\n”, filename)
```
the following line would make hello readable and executable by its owning user but would disable all other permissions associated with hello
```
chmod ("hello", S_IRUSR|S_IXUSR);
```

here’s how you would exchange the effective and real user IDs: `setreuid (geteuid(), getuid ());  `


# Pluggable Authentication Modules (Pam)

makes it easy to write applications that authenticate their users as the system administrator sees fit
## install development pam
```
$ gcc -o bin/pam security/pam.c  -lpam -lpam_misc
$ sudo apt-get install libpam0g-dev

```

## Compile
`gcc -o pam pam.c  -lpam -lpam_misc `

# Buffer overrun
you should never write into a data structure, on the stack or elsewhere, without verifying that you’re not going to write beyond its region of memory.
` char* username = getline (NULL, 0, stdin);`
automaticall allocates a buffer big enoung

# Race Conditions in /tmp
involves the creation of files with predictable names,typically in the /tmp directory

Suppose that your program prog , running as root, always creates a temporary file called /tmp/prog and writes some vital information there. A malicious user can create a symbolic link from /tmp/prog to any other file on the system.When your program goes to create the file, the open system call will suc-
ceed. However, the data that you write will not go to /tmp/prog ; instead, it will be
written to some arbitrary file of the attacker’s choosing

# Solutions
* use a randomized name for the file Eg Read from `/dev/random` (might use a large number of guessing)
* use `O_EXCL ` flag when calling `open` (Not reliable when using NFS)
mkstemp does on Linux is open the file with O_EXCL after trying to pick a name that is hard to guess. In other words, using mkstemp is still insecure if /tmp is mounted over NFS

One approach that works is to call lstat on the newly created file. The lstat function is like stat , except that if the file referred to is a symbolic link, lstat tells you about the link, not the file to which it refers. If lstat tells you that your new file is an ordinary file, not a symbolic link, and that it is owned by you, then you should be okay

# Using system or popen
Using system is unsafe.This function invokes the standard system shell to run the command.
The same problem can arise with popen creates a pipe between the parent and child process but still uses the shell to run the command

## Solution
use the exec family of functions instead of system or popen
validate the string sing the `isalpha` function. If it doesn’t contain any other characters