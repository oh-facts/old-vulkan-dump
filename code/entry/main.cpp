#if defined(__unix__) || defined(__APPLE__)
#pragma GCC push_options
#pragma GCC optimize ("no-stack-protector")
#endif
// code
#include <os/os.h>


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


// I am assuming this handle cannot be illegal at any point. msdn said so
HANDLE hStdout;

// todo
// write to a text buffer and print that

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

// 3rd party
#define STB_SPRINTF_IMPLEMENTATION
#include <third_party/stb_sprintf.h>
#include <third_party/handmade_math.h>


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
  
  HWND win = CreateWindowA(wc.lpszClassName, "window",WS_OVERLAPPED, 100,100, 800, 600, 0, 0, wc.hInstance, 0);
  
  if(win)
  {
    is_running = true;
    
    ShowWindow(win, SW_SHOWNORMAL);
    UpdateWindow(win);
    
    
    // here we go
    
    ID3D12Device *device = 0;
    
    ID3D12Debug *debug_controller = 0;
    D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller));
    
    debug_controller->EnableDebugLayer();
    
    IDXGIFactory4 *factory = 0;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    
    IDXGIAdapter *warp_adapter = 0;
    
    factory->EnumWarpAdapter(IID_PPV_ARGS(&warp_adapter));
    
    D3D12CreateDevice(warp_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    
    D3D12_COMMAND_QUEUE_DESC q_desc = {};
    q_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    q_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    
    ID3D12CommandQueue *cmd_q;
    
    device->CreateCommandQueue(&q_desc, IID_PPV_ARGS(&cmd_q));
    
#define FRAME_COUNT 2
    DXGI_SWAP_CHAIN_DESC scdesc = {};
    scdesc.BufferCount = FRAME_COUNT;
    scdesc.BufferDesc.Width = 900;
    scdesc.BufferDesc.Height = 600;
    scdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scdesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    scdesc.OutputWindow = win;
    scdesc.SampleDesc.Count = 1;
    scdesc.Windowed = true;
    
    IDXGISwapChain *_sc = 0;
    factory->CreateSwapChain(cmd_q, &scdesc, &_sc);
    
    factory->MakeWindowAssociation(win, DXGI_MWA_NO_ALT_ENTER);
    
    u32 frame_index = 0;
    
    IDXGISwapChain3 *sc = (IDXGISwapChain3*)_sc;
    
    frame_index = sc->GetCurrentBackBufferIndex();
    
    ID3D12DescriptorHeap *rtv_heap;
    u32 rtv_desc_size = 0;
    {
      D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
      rtv_heap_desc.NumDescriptors = FRAME_COUNT;
      rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&rtv_heap));
      rtv_desc_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    
    ID3D12Resource *render_targets[FRAME_COUNT] = {};
    
    {
      D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = rtv_heap->GetCPUDescriptorHandleForHeapStart();
      
      for(u32 i = 0; i < FRAME_COUNT; i ++)
      {
        sc->GetBuffer(i, IID_PPV_ARGS(&render_targets[i]));
        device->CreateRenderTargetView(render_targets[i], 0, rtv_handle);
        rtv_handle.ptr += rtv_desc_size;
        
      }
      
    }
    ID3D12CommandAllocator *cmd_allocator;
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmd_allocator));
    
    
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
