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

// change this when you have time
// print - prints directly to the console, 512 limit
// printb - prints to a buffer, no limit
// printbc - prints buffer to a console, no limit

void print(const char *format, ...)
{
  char buffer[512];
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

void vsnprint(char *buffer, size_t size, const char *format, ...)
{
  va_list args;
  
  va_start(args, format);
  
  stbsp_vsnprintf(buffer, size, format, args);
  
  va_end(args);
  
}

void print_buffer(char *buffer, size_t size)
{
  DWORD bytesWritten;
  
  int failed = WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &bytesWritten, NULL);
  
  if (!failed)
  {
    MessageBoxA(NULL, ":(", ":)", MB_OK | MB_ICONINFORMATION);
  }
}

#define YK_CRT
#include <base/base_inc.h>
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

enum qfam_type
{
  qfam_gfx,
  qfam_comp,
  qfam_trans,
  qfam_pres,
  qfam_num
};

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
    // todo(facts): preemptively make a vk_state and put this crap in there
    // all this is crap boilerplate. i have done this before so there's nothing
    // wrong to do it.
    
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
#elif defined(OS_LINUX)
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
    
    AssertM(res == VK_SUCCESS, "instance creation is not great success. This is reason to crash. Bye Bye :(");
    
    u32 phys_device_count = 0;
    res = vkEnumeratePhysicalDevices(
                                     vk_inst,
                                     &phys_device_count,
                                     0
                                     );
    
    print("\nPhysical Devices: %d",phys_device_count);
    
    AssertM(res == VK_SUCCESS, "Physical device retrieval failed.");
    
    VkPhysicalDevice *phys_devices = push_array(
                                                &scratch, 
                                                VkPhysicalDevice, 
                                                phys_device_count
                                                );
    
    res = vkEnumeratePhysicalDevices(
                                     vk_inst,
                                     &phys_device_count,
                                     phys_devices
                                     );
    AssertM(res == VK_SUCCESS, "Physical device retrieval failed.");
    
    VkPhysicalDevice phys_device = {};
    
    {
      ArenaTemp temp_arena = arena_temp_begin(&scratch);
      
      // todo(facts) : print api needs to be worked on
      char *buffer = push_array(&scratch, char, Megabytes(1)); 
      char *offset = buffer;
      
      vsnprint(offset, 256, "\n\nHello sailor. Here is device info\n");
      offset = buffer + strlen(buffer);
      
      for(u32 i = 0; i < phys_device_count; i++)
      {
        VkPhysicalDeviceProperties props = {};
        vkGetPhysicalDeviceProperties(phys_devices[i], &props);
        
        vsnprint(offset, 256, "%d. ", i);
        offset = buffer + strlen(buffer);
        
        // todo(facts) : do this busy work later
        switch(props.deviceType)
        {
          case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
          {
            vsnprint(offset, 256, "discrete gpu\n");
            phys_device = phys_devices[i];
          }break;
          
          case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
          {
            vsnprint(offset, 256, "integrated gpu\n");
          }break;
          default:
          {
            vsnprint(offset, 256, "other\n");
          }
        }
        
        offset = buffer + strlen(buffer);
      }
      
      print_buffer(buffer, sizeof(buffer));
      
      arena_temp_end(&temp_arena);
    }
    
    // todo(facts): set device features / properties
    
    // surfaces
    VkSurfaceKHR surface = {};
    {
      VkWin32SurfaceCreateInfoKHR win32_surf_info = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = 0,
        .flags = 0,
        .hinstance = GetModuleHandle(0),
        .hwnd = win
      };
      
      vkCreateWin32SurfaceKHR(vk_inst, &win32_surf_info, 0, &surface);
      
    }
    
    // queue stuff
    // No comparisions to find best queue is done. But the scaffolding is there
    // basically do an extra check to verify that the queue i picked does purely
    // what i hope it does, and not a hybrid. if it purely does that, move on
    // and set its found to true
    // if not, don't set found to true and hope some queue down the line finds what
    // its looking for
    
    u32 qfams[qfam_num] = {};
    
    {
      ArenaTemp temp_arena = arena_temp_begin(&scratch);
      
      b32 qfamsfound[qfam_num] = {};
      
      u32 qnum = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(phys_device, &qnum, 0);
      
      VkQueueFamilyProperties *qfam_props = push_array(
                                                       &scratch, 
                                                       VkQueueFamilyProperties,
                                                       qnum
                                                       );
      
      vkGetPhysicalDeviceQueueFamilyProperties(phys_device, &qnum, qfam_props);
      
      
      for(u32 i = 0; i < qnum; i++)
      {
        if(!qfamsfound[qfam_gfx])
        {
          if(qfam_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
          {
            qfams[qfam_gfx] = i; 
            qfamsfound[qfam_gfx] = true;
          }
        }
        
        if(!qfamsfound[qfam_comp])
        {
          if(qfam_props[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
          {
            qfams[qfam_comp] = i; 
            qfamsfound[qfam_comp] = true;
          }
        }
        
        if(!qfamsfound[qfam_trans])
        {
          if(qfam_props[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
          {
            qfams[qfam_trans] = i; 
            qfamsfound[qfam_trans] = true;
          }
        }
        
        if(!qfamsfound[qfam_pres])
        {
          VkBool32 present_support = VK_FALSE;
          vkGetPhysicalDeviceSurfaceSupportKHR(phys_device, i, surface, &present_support);
          if(present_support == VK_TRUE)
          {
            qfams[qfam_pres] = i;
            qfamsfound[qfam_pres] = true;
          }
        }
        
      }
      
      AssertM(qfamsfound, "Oh no, present mode not found");
      
      for(u32 i = 0; i < qfam_num; i ++)
      {
        print("%d %d\n", i, qfams[i] );
      }
      
      arena_temp_end(&temp_arena);
    }
    
    // logic device
    
    {
      ArenaTemp temp_arena = arena_temp_begin(&scratch);
      
      
      
      arena_temp_end(&temp_arena);
    }
    
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