Proc window into the running Linux kernel
=========================================
Files in the /proc file system don’t correspond to actual files on a physical device. Instead, they are magic objects that behave like files but provide access to parameters, data structures, and statistics in the kernel
# Extract information

A simple way to extract a value from this output is to read the file into a buffer and parse it in memory using `sscanf`

# Process Entries

