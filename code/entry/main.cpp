
// code
#include <os/os.h>

// 3rd party
#define STB_SPRINTF_IMPLEMENTATION
#include <third_party/stb_sprintf.h>
#include <third_party/HandmadeMath.h>

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

// I am assuming this handle cannot be illegal at any point. msdn said so
HANDLE hStdout;

// todo
// write a text buffer and print that

int strlen(char *string)
{
  char *c = string;
  int len = 0;
  
  while(*c != '\0')
  {
    len++;
    c++;
  }
  return len;
}

void print(const char *format, ...)
{
  char buffer[30];
  DWORD bytesWritten;
  va_list args;
  
  va_start(args, format);
  
  stbsp_vsnprintf(buffer, sizeof(buffer), format, args);
  
  va_end(args);
  
  int failed = WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &bytesWritten, NULL);
  
  if (!failed)
  {
    MessageBoxA(NULL, ":(", ":)", MB_OK | MB_ICONINFORMATION);
  }
}

#define YK_CRT
#include <base/base_inc.h>

int __stdcall mainCRTStartup()
{
  HMM_Vec2 a = {1,2};
  HMM_Vec2 b = {2,3};
  
  HMM_Vec2 c = HMM_AddV2(a,b);
  
  print("age: %d\nname: %s", 4, "chris pine");
  print(" ");
  print("said hi");
  
  WNDCLASSA wc = {};
  
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.hInstance = GetModuleHandle(0);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszClassName = "main";
  
  
  RegisterClass(&wc);
  
  HWND win = CreateWindowA(wc.lpszClassName, "window",WS_OVERLAPPED, 100,100, 800, 600, 0, 0, wc.hInstance, 0);
  
  if(!win)
  {
    print("where is window");
  }
  
  ShowWindow(win, SW_SHOWNORMAL);
  UpdateWindow(win);
  
  while(1)
  {
    
  }
  
  
  ExitProcess(0);
}