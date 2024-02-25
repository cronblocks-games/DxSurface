#ifndef _DXSURFACE_DXSTYPES_H_
#define _DXSURFACE_DXSTYPES_H_



#include <string>

#include "DxsMacros.h"

namespace CB::DxSurface {

  struct WindowRect
  {
    int x = 0, y = 0, w = 0, h = 0;

    WindowRect(int _x = 0, int _y = 0, int _w = 0, int _h = 0)
    {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
    }

    WindowRect& operator=(const WindowRect& other) noexcept
    {
      x = other.x;
      y = other.y;
      w = other.w;
      h = other.h;

      return *this;
    }
    WindowRect& operator=(WindowRect&& other) noexcept
    {
      x = other.x;
      y = other.y;
      w = other.w;
      h = other.h;

      x = 0;
      y = 0;
      w = 0;
      h = 0;

      return *this;
    }
  };

  struct DxSurfaceOptions
  {
    bool debugEnabled = false;
    std::string defaultWindowTitle;
    WindowRect defaultWindowRect;

    DxSurfaceOptions()
    {
      debugEnabled = true;
      defaultWindowTitle = "DxSurface";
      defaultWindowRect = WindowRect(10, 10, 250, 250);
    }
    DxSurfaceOptions(const DxSurfaceOptions& options)
    {
      *this = options;
    }

    DxSurfaceOptions& operator=(const DxSurfaceOptions& other) noexcept
    {
      debugEnabled = other.debugEnabled;
      defaultWindowTitle = other.defaultWindowTitle;
      defaultWindowRect = other.defaultWindowRect;

      return *this;
    }
    DxSurfaceOptions& operator=(DxSurfaceOptions&& other) noexcept
    {
      debugEnabled = other.debugEnabled;
      other.debugEnabled = false;

      defaultWindowTitle = std::move(other.defaultWindowTitle);
      defaultWindowRect = std::move(other.defaultWindowRect);

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
}



#endif
