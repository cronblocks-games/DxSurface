#ifndef _DXSURFACE_DXSTYPES_H_
#define _DXSURFACE_DXSTYPES_H_

#include <atomic>
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
  using Atomic = std::atomic<T>;
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
  using TimeDurationSec = std::chrono::duration<double>;

  class Window;
  class Keyboard;
  class Mouse;

  using RenderingCallbackInit = void(*)(const Window&);
  using RenderingCallbackRunning = void(*)(const Window&, const Keyboard&, const Mouse&, double deltaTimeSec);
  using RenderingCallbackPaused = void(*)(const Window&, const Keyboard&, const Mouse&, double deltaTimeSec);
  using RenderingCallbackExitted = void(*)(const Window&, const Keyboard&, const Mouse&);
  using RenderingCallbackStatusChanged = void(*)(const Window&, const Keyboard&, const Mouse&);

  using ProcessingCallbackInit = void(*)(const Window&);
  using ProcessingCallbackRunning = void(*)(const Window&, const Keyboard&, const Mouse&, double deltaTimeSec);
  using ProcessingCallbackPaused = void(*)(const Window&, const Keyboard&, const Mouse&, double deltaTimeSec);
  using ProcessingCallbackExitted = void(*)(const Window&, const Keyboard&, const Mouse&);
  using ProcessingCallbackStatusChanged = void(*)(const Window&, const Keyboard&, const Mouse&);

  struct WindowRect { int x = 0, y = 0, w = 0, h = 0; };
  struct WindowCallbacks
  {
    WindowCallbacks(
      RenderingCallbackInit renderingCallbackInit = nullptr,
      RenderingCallbackRunning renderingCallbackRunning = nullptr,
      RenderingCallbackPaused renderingCallbackPaused = nullptr,
      RenderingCallbackExitted renderingCallbackExitted = nullptr,
      RenderingCallbackStatusChanged renderingCallbackStatusChanged = nullptr,

      ProcessingCallbackInit processingCallbackInit = nullptr,
      ProcessingCallbackRunning processingCallbackRunning = nullptr,
      ProcessingCallbackPaused processingCallbackPaused = nullptr,
      ProcessingCallbackExitted processingCallbackExitted = nullptr,
      ProcessingCallbackStatusChanged processingCallbackStatusChanged = nullptr)
    {
      this->renderingCallbackInit.store(renderingCallbackInit);
      this->renderingCallbackRunning.store(renderingCallbackRunning);
      this->renderingCallbackPaused.store(renderingCallbackPaused);
      this->renderingCallbackExitted.store(renderingCallbackExitted);
      this->renderingCallbackStatusChanged.store(renderingCallbackStatusChanged);

      this->processingCallbackInit.store(processingCallbackInit);
      this->processingCallbackRunning.store(processingCallbackRunning);
      this->processingCallbackPaused.store(processingCallbackPaused);
      this->processingCallbackExitted.store(processingCallbackExitted);
      this->processingCallbackStatusChanged.store(processingCallbackStatusChanged);
    }
    WindowCallbacks(const WindowCallbacks& o)
    {
      *this = o;
    }
    WindowCallbacks(WindowCallbacks&& o) noexcept
    {
      *this = std::move(o);
    }
    WindowCallbacks& operator=(const WindowCallbacks& o)
    {
      if (this != &o)
      {
        renderingCallbackInit.store(o.renderingCallbackInit.load());
        renderingCallbackRunning.store(o.renderingCallbackRunning.load());
        renderingCallbackPaused.store(o.renderingCallbackPaused.load());
        renderingCallbackExitted.store(o.renderingCallbackExitted.load());
        renderingCallbackStatusChanged.store(o.renderingCallbackStatusChanged.load());

        processingCallbackInit.store(o.processingCallbackInit.load());
        processingCallbackRunning.store(o.processingCallbackRunning.load());
        processingCallbackPaused.store(o.processingCallbackPaused.load());
        processingCallbackExitted.store(o.processingCallbackExitted.load());
        processingCallbackStatusChanged.store(o.processingCallbackStatusChanged.load());
      }
      return *this;
    }
    WindowCallbacks& operator=(WindowCallbacks&& o) noexcept
    {
      if (this != &o)
      {
        renderingCallbackInit.store(o.renderingCallbackInit.exchange(nullptr));
        renderingCallbackRunning.store(o.renderingCallbackRunning.exchange(nullptr));
        renderingCallbackPaused.store(o.renderingCallbackPaused.exchange(nullptr));
        renderingCallbackExitted.store(o.renderingCallbackExitted.exchange(nullptr));
        renderingCallbackStatusChanged.store(o.renderingCallbackStatusChanged.exchange(nullptr));

        processingCallbackInit.store(o.processingCallbackInit.exchange(nullptr));
        processingCallbackRunning.store(o.processingCallbackRunning.exchange(nullptr));
        processingCallbackPaused.store(o.processingCallbackPaused.exchange(nullptr));
        processingCallbackExitted.store(o.processingCallbackExitted.exchange(nullptr));
        processingCallbackStatusChanged.store(o.processingCallbackStatusChanged.exchange(nullptr));
      }
      return *this;
    }

    Atomic<RenderingCallbackInit> renderingCallbackInit;
    Atomic<RenderingCallbackRunning> renderingCallbackRunning;
    Atomic<RenderingCallbackPaused> renderingCallbackPaused;
    Atomic<RenderingCallbackExitted> renderingCallbackExitted;
    Atomic<RenderingCallbackStatusChanged> renderingCallbackStatusChanged;

    Atomic<ProcessingCallbackInit> processingCallbackInit;
    Atomic<ProcessingCallbackRunning> processingCallbackRunning;
    Atomic<ProcessingCallbackPaused> processingCallbackPaused;
    Atomic<ProcessingCallbackExitted> processingCallbackExitted;
    Atomic<ProcessingCallbackStatusChanged> processingCallbackStatusChanged;
  };
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
    
    unsigned int maxRenderingThreadRefreshRateHz = DxsDefaultThreadRefreshRateHz;
    unsigned int maxProcessingThreadRefreshRateHz = DxsDefaultThreadRefreshRateHz;

    WindowCallbacks callbacks = WindowCallbacks();
  };

  enum class WindowCreationState
  {
    NONE = 0,
    Success,
    Fail
  };
  enum class ExecutionCommand
  {
    Run, Pause, Exit,
  };
  enum class ExecutionState
  {
    NONE = 0, Init, Running, Paused, Exitted,
  };
  enum class ExecutionExitReason
  {
    Normal = 0, Exception
  };




} //- namespace CB::DxSurface



#endif
