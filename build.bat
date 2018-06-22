rem --------------------------------------------------------------
rem Batchfile that compiles the bef sources under Borland C++ 3.1.
rem Uses "large" memory model for befprof b/c it uses >64K data.
rem --------------------------------------------------------------

bcc src\bef.c bin\bef.exe
bcc -ml src\befprof.c bin\befprof.exe
bcc src\bef2c.c bin\bef2c.exe
