#ifndef _DXSURFACE_DXSTYPES_H_
#define _DXSURFACE_DXSTYPES_H_



#include <string>
#include <mutex>
#include <memory>
#include <vector>

#include "DxsMacros.h"
#include "DxsWinApi.h"

namespace CB::DxSurface {

#if defined(_UNICODE) || defined(UNICODE)
  using TString = std::wstring;
  using ConstTCharPtr = const wchar_t*;
#else
  using TString = std::string;
  using ConstTCharPtr = const char*;
#endif

  template <typename T>
  using SharedPtr = std::shared_ptr<T>;
  template <typename T>
  using UniquePtr = std::unique_ptr<T>;
  template <typename T>
  using Ptr = SharedPtr<T>;
  template <typename T>
  using Vector = std::vector<T>;

  using Mutex = std::mutex;
  using MutexLock = const std::lock_guard<Mutex>;

  struct WindowRectangle
  {
    int x = 0, y = 0, w = 0, h = 0;
  };
  struct WindowCreationOptions
  {
    bool isPrimary;
    bool isDebugEnabled = false;
    TString title;
    WindowRectangle rect;

    unsigned int maxRefreshRateHz;
  };

  enum class WindowCreationState
  {
    NONE = 0,
    Success,
    Fail
  };
  enum class RenderingState
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
