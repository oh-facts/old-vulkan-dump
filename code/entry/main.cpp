// code
#include <os/os.h>

// I am assuming this handle cannot be illegal at any point. msdn said so
HANDLE hStdout;

// todo
// write to a text buffer and print that

// 3rd party
#define STB_SPRINTF_IMPLEMENTATION
#include <third_party/stb_sprintf.h>
#include <third_party/handmade_math.h>

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


b32 is_running = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  
  switch (msg)
  {
    case WM_DESTROY:
    {
      is_running = false;
      PostQuitMessage(0);
    }break;
    case WM_KEYDOWN:
    {
      switch (wParam) 
      {
        case 'Q':
        case VK_ESCAPE:
        {  
          is_running = false;
        }break;
        
        default:
        {
          return DefWindowProc(hwnd, msg, wParam, lParam);
        }break;
      }
    }break;
    
  }
  
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

#include <vulkan/vulkan.h>

int __stdcall mainCRTStartup()
{
  HMM_Vec2 a = {1,2};
  HMM_Vec2 b = {2,3};
  
  HMM_Vec2 c = a + b;
  
  print("age: %d\nname: %s", 4, "chris pine");
  print(" ");
  print("said hi");
  print("\n%.2f %.2f", c[0], c.Y);
  
  WNDCLASSA wc = {};
  
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.hInstance = GetModuleHandle(0);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszClassName = "main";
  
  
  RegisterClass(&wc);
  
  HWND win = CreateWindowA(wc.lpszClassName, "window", WS_OVERLAPPED, 100,100, 800, 600, 0, 0, wc.hInstance, 0);
  
  
  if(win)
  {
    is_running = true;
    
    ShowWindow(win, SW_SHOWNORMAL);
    UpdateWindow(win);
    
    
    // vulkan lets go
    
    u32 version = 0;
    vkEnumerateInstanceVersion(&version);
    
    print("\n%d.%d.%d\n"
          ,VK_VERSION_MAJOR(version)
          ,VK_VERSION_MINOR(version)
          ,VK_VERSION_PATCH(version)
          );
    
    
    MSG msg;
    while(is_running)
    {
      while(PeekMessageA(&msg, 0, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
      }
      
    }
    
  }
  else
  {
    print("window creation failed");
  }
  
  ExitProcess(0);
  
}

#if defined(__unix__) || defined(__APPLE__)

int main()
{
  mainCRTStartup();
}

#endif