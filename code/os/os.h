/* date = April 23rd 2024 2:42 pm */

#ifndef OS_H
#define OS_H



extern "C"
{
#pragma function(memset)
  void *memset(void *dest, int c, size_t count)
  {
    char *bytes = (char *)dest;
    while (count--)
    {
      *bytes++ = (char)c;
    }
    return dest;
  }
  
#pragma function(memcpy)
  void *memcpy(void *dest, const void *src, size_t count)
  {
    char *dest8 = (char *)dest;
    const char *src8 = (const char *)src;
    while (count--)
    {
      *dest8++ = *src8++;
    }
    return dest;
  }
}

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

#endif

#if defined(__unix__) || defined(__APPLE__)
#pragma GCC push_options
#pragma GCC optimize ("no-stack-protector")
#endif



#endif //OS_H