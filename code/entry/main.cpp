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
#define USE_VALIDATION_LAYERS 1

#if defined (OS_WIN32)
#include <vulkan/vulkan_win32.h>
#elif defined(OS_UNIX)
#include <vulkan/vulkan_xcb.h>
#endif

#if DEBUG
LPVOID base_address = (LPVOID)Terabytes(2);
#else
LPVOID base_address = 0;
#endif


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
    
    
    size_t mem_size = Megabytes(256);
    void *memory = VirtualAlloc(base_address, mem_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    AssertM(memory, "you dont have 256 mb of free memory");
    
    size_t arena_size = mem_size / 2;
    
    Arena arena = {};
    arena_innit(&arena, arena_size, memory);
    
    Arena scratch = {};
    arena_innit(&scratch, (mem_size - arena_size) ,  (u8*)memory + arena_size);
    
    // vulkan lets go
    
    u32 version = 0;
    vkEnumerateInstanceVersion(&version);
    
    print("\nVulkan Version: %d.%d.%d\n"
          ,VK_VERSION_MAJOR(version)
          ,VK_VERSION_MINOR(version)
          ,VK_VERSION_PATCH(version)
          );
    
    
    // extentions and validation layers
    
    const char *extentions[3] = {};
    u32 extention_num = 0;
    
    const char *validation_layers[1] = {};
    u32 validation_layers_num = 0;
    
    extentions[extention_num++] = VK_KHR_SURFACE_EXTENSION_NAME;
    
#if defined(OS_WIN32)
    extentions[extention_num++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
#elif defined(OS_UNIX)
    extentions[extention_num++] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
#endif
    
#if USE_VALIDATION_LAYERS
    
    validation_layers[validation_layers_num++] = {
      "VK_LAYER_KHRONOS_validation"
    };
    
    extentions[extention_num++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    
#endif
    VkApplicationInfo vk_app_info = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pNext = 0,
      .pApplicationName = "app",
      .applicationVersion = 0,
      .pEngineName = "engine",
      .engineVersion = 0,
      .apiVersion = VK_API_VERSION_1_3
    };
    
    VkInstanceCreateInfo vk_create_info = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pNext = 0,
      .flags = 0,
      .pApplicationInfo = &vk_app_info,
      .enabledLayerCount = validation_layers_num,
      .ppEnabledLayerNames = validation_layers,
      .enabledExtensionCount = extention_num,
      .ppEnabledExtensionNames = extentions
    };
    
    VkInstance vk_inst = {};
    VkResult res = vkCreateInstance(&vk_create_info, 0, &vk_inst);
    
    AssertM(res == VK_SUCCESS, "instance creation is not great success");
    
    
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