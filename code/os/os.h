/* date = April 23rd 2024 2:42 pm */

#ifndef OS_H
#define OS_H

#if _WIN32

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

#include <d3d12.h>
#include <dxgi1_4.h>

#endif

#endif //OS_H