#ifndef _DXSURFACE_DXSTYPES_H_
#define _DXSURFACE_DXSTYPES_H_

#include <string>
#include <mutex>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include "DxsMacros.hpp"
#include "DxsWinApi.hpp"

namespace CB::DxSurface {

#if defined(_UNICODE) || defined(UNICODE)
  using TString = std::wstring;
  using ConstTCharPtr = const wchar_t*;
#else
  using TString = std::string;
  using ConstTCharPtr = const char*;
#endif

  template <typename T>
  using PtrShared = std::shared_ptr<T>;
  template <typename T>
  using PtrUnique = std::unique_ptr<T>;
  template <typename T>
  using Ptr = PtrShared<T>;
  template <typename T>
  using Vector = std::vector<T>;
  using Thread = std::thread;

  using Mutex = std::mutex;
  using MutexLock = const std::lock_guard<Mutex>;
  using Lock = MutexLock;

  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using TimeDurationMilli = std::chrono::duration<double, std::milli>;

  using PtrProcessingFunc = void(*)(double deltaTimeMillis);

  struct WindowRect { int x = 0, y = 0, w = 0, h = 0; };
  struct WindowCreationOptions
  {
    TString      title            = DxsT("DxSurface");
    bool         isPrimary        = false;
    bool         isDebugEnabled   = false;
    HICON        hIcon            = nullptr;
    HICON        hIconSmall       = nullptr;
    HCURSOR      hCursor          = nullptr;
    WindowRect   rect             = WindowRect(10, 10, 600, 450);
    DWORD        dwStyle          = WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
    DWORD        dwExStyle        = WS_EX_WINDOWEDGE;
    
    unsigned int maxRenderingThreadRefreshRateHz = 30;
    unsigned int maxProcessingThreadRefreshRateHz = 30;

    PtrProcessingFunc processingFunc = nullptr;
  };

  enum class WindowCreationState
  {
    NONE = 0,
    Success,
    Fail
  };
  enum class ExecutionState
  {
    NONE = 0,
    Init,
    Running,
    Paused,
    Exitted,
  };
  enum class ThreadExitReason
  {
    Normal = 0,
    Exception
  };




} //- namespace CB::DxSurface



#endif
