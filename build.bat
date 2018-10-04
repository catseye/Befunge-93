@echo off
rem --------------------------------------------------------------
rem Batchfile that compiles the bef sources under Borland C++ 3.1.
rem Uses "large" memory model for befprof b/c it uses >64K data.
rem --------------------------------------------------------------

@echo on
bcc -ebin\bef.exe src\bef.c
bcc -ml -ebin\befprof.exe src\befprof.c
bcc -ebin\bef2c.exe src\bef2c.c
