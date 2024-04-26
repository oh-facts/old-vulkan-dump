/* date = April 23rd 2024 2:42 pm */

#ifndef OS_H
#define OS_H


#if defined(_WIN32)
#define OS_WIN32
#elif defined (__linux__)
#define OS_LINUX
#elif defined(__APPLE__)
#define OS_APPLE
#endif

#if defined(OS_LINUX) || defined(OS_APPLE)
#pragma GCC push_options
#pragma GCC optimize ("no-stack-protector")
#endif


#if defined(OS_WIN32)

extern "C"
{
  int _fltused;
  
#ifdef _M_IX86 // following functions are needed only for 32-bit architecture
  
  __declspec(naked) void _ftol2()
  {
    __asm
    {
      fistp qword ptr 
        mov   edx,
      mov   eax,
      ret
    }
  }
  
  __declspec(naked) void _ftol2_sse()
  {
    __asm
    {
      fistp dword ptr 
        mov   eax,
      ret
    }
  }
  
#endif
}


#include <windows.h>

#endif

#endif //OS_H