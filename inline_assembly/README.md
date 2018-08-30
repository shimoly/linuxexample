Inline Assembly Code
=================================================
GNU Compiler Collection permits programmers to add architecture-dependent assembly language instructions to their programs.

```
asm (“fsin” : “=t” (answer) : “0” (angle));
is an x86-specific way of coding this C statement: 1
answer = sin (angle);
```