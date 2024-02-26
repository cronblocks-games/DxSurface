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

    WindowRectangle(int _x = 0, int _y = 0, int _w = 0, int _h = 0)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
    }

    WindowRectangle& operator=(const WindowRectangle& other) noexcept
    {
      x = other.x;
      y = other.y;
      w = other.w;
      h = other.h;

      return *this;
    }
    WindowRectangle& operator=(WindowRectangle&& other) noexcept
    {
      x = other.x;
      y = other.y;
      w = other.w;
      h = other.h;

      other.x = 0;
      other.y = 0;
      other.w = 0;
      other.h = 0;

      return *this;
    }
  };

  struct WindowCreationOptions
  {
    bool isPrimary;
    bool isDebugEnabled = false;
    TString title;
    WindowRectangle rect;

    WindowCreationOptions()
    {
      isPrimary = false;
      isDebugEnabled = true;
      title = DxsT("DxSurface");
      rect = WindowRectangle(10, 10, 250, 250);
    }
    WindowCreationOptions(const WindowCreationOptions& options)
    {
      *this = options;
    }

    WindowCreationOptions& operator=(const WindowCreationOptions& other) noexcept
    {
      isPrimary = other.isPrimary;
      isDebugEnabled = other.isDebugEnabled;
      title = other.title;
      rect = other.rect;

      return *this;
    }
    WindowCreationOptions& operator=(WindowCreationOptions&& other) noexcept
    {
      isPrimary = other.isPrimary;
      other.isPrimary = false;

      isDebugEnabled = other.isDebugEnabled;
      other.isDebugEnabled = false;

      title = std::move(other.title);
      rect = std::move(other.rect);

      return *this;
    }
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
