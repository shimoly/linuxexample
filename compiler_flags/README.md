

# Gcc flags
``
## Compiling
The -c flag tells the gcc to compile the program to an object file only, with out it gcc would attempt to to link the program to make an excutable program  
` gcc -c main.c `  

-I option is used to tell GCC where to search header file by default GCC will look in the current directory and in the directory where headers for the standard libraries are installed


`g++ -c -I ../include reciprocal.cpp`  

To undefine DEBUG (Turn off the assert statement )
`g++ -c -D NDEBUG reciprocal.cpp`  

To define NDEBUG to some particular value

`g++ -c -D NDEBUG=3 reciprocal.cpp`  

To optimize the code, (N.B It is the capital O)

`g++ -c -O2 reciprocal.cpp`  

## Linking objects

Use g++ to link program that contain C++ code even it also contain C code
`g++ -o reciprocal main.o reciprocal.o`  


In Linux, library names almost always start with lib . For example, the Pluggable Authentication Module (PAM) library is called libpam.a . To link in libpam.a , you use a command like this:

`g++ -o reciprocal main.o reciprocal.o -lpam`  

The compiler automatically adds the lib prefix and the .a suffix  
As with header files, the linker looks for libraries in some standard places, including the /lib and /usr/lib  

if you want the linker to search other directories as well, you should use the -L option, which is the parallel of the -I  
  
`g++ -o reciprocal main.o reciprocal.o -L/usr/local/lib/pam -lpam`    
  
Although you don’t have to use the -I option to get the preprocessor to search the current directory, you do have to use the -L option to get the linker to search the current directory  
  
`gcc -o app app.o -L. -ltest`  


### Archives  
  
Archives (static library) collection of objects files as a single file (in winwdows it is .LIB file)  
when you provide linker with archive, it searchs the archive for the object (function and variables) it needs extract and link them to your program
  

Archives are created with ar command  
`ar cr libtest.a test1.o test2.o`    
  
Because the linker searches the archive when it is encountered on the command line, it usually makes sense to put archives at the end of the command line.  
In shared library the executable merely contains a reference to the shared library.  
To create a shared library, you must compile the objects that will make up the library using the -fPIC option to the compiler, like this:  
`gcc -c -fPIC test1.c`    

The -fPIC option tells the compiler that you are going to be using shared object.  

## Shared library
Then you combine the object files into a shared library, like this:  

`gcc -shared -fPIC -o libtest.so test1.o test2.o`  
  
Linking with a shared library is just like linking with a static archive  
  
`gcc -o app app.o -L. –ltest`    
  
You can use the -static option to demand static archives if both static and dyanmic lib are available  
  
`gcc -static -o app app.o -L. –ltest`    
  

### The ldd command displays the shared libraries that are linked into an executable.  

The system searches only /lib and /usr/lib , by default. If a shared library that is linked into your program is installed outside those directories, it will not be found, and the system will refuse to run the program. One solution to this problem is to use the -Wl,-rpath option when linking the program. Suppose that you use this:

`gcc -o app app.o -L. -ltest -Wl,-rpath,/usr/local/lib`    
When app is run, the system will search /usr/local/lib for any required shared libraries  


Occasionally, two libraries will be mutually dependent. (Very poor design), this line will cause libfoo.a to be searched multiple time

`gcc -o app app.o -lfoo -lbar –lfoo`    

### Open shared library at runtime  
You could open a shared library at run time named libtest.so by calling dlopen like this:  
`dlopen (“libtest.so”, RTLD_LAZY)


void* handle = dlopen (“libtest.so”, RTLD_LAZY);  



void (*test)() = dlsym (handle, “my_function”);  


(*test)();  


dlclose (handle);  
`  


Both dlopen and dlsym return NULL if they do not succeed. In that event, you can call dlerror (with no parameters) to obtain a human-readable error message describing the problem.  

### Shared in library in C++
If you’re writing the code in your shared library in C++, you will probably want to declare those functions and variables that you plan to access elsewhere with the extern “C” linkage specifier.   

For instance, if the C++ function my_function is in a shared library and you want to access it with dlsym , you should declare it like tis:  
`extern “C” void foo ();`  



