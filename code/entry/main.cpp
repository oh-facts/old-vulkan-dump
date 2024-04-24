
// code
#include <os/os.h>

// 3rd party
#define STB_SPRINTF_IMPLEMENTATION
#include <third_party/stb_sprintf.h>
#include <third_party/HandmadeMath.h>


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
  
  if (3 < 4)
  {
    AssertM(false , "wow");
  }
  
  ExitProcess(0);
  
  ID3D12Device* device;
  IDXGISwapChain3* swapChain;
  
  // Create the device
  D3D12CreateDevice(0, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
  
  return 0;
}