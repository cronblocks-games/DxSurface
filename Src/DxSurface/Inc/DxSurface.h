#ifndef _DXSURFACE_DXSURFACE_H_
#define _DXSURFACE_DXSURFACE_H_

#include <vector>

#include "DxsTypes.h"
#include "DxsWinApi.h"
#include "DxsWindow.h"
#include "DxsExceptions.h"


namespace CB::DxSurface {

  class DxSurface {

  public:
    DxSurface();
    DxSurface(const DxSurfaceOptions& options);

    DxSurface(const DxSurface&);
    DxSurface(DxSurface&&) noexcept;
    DxSurface& operator=(const DxSurface&);
    DxSurface& operator=(DxSurface&&) noexcept;

    ~DxSurface() = default;

    Window& CreateNewWindow();
    Window& CreateNewWindow(const String& title, int x, int y, int width, int height, bool isPrimary = false, bool debugEnabled = true);

    void Run();

  private:
    DxSurfaceOptions m_stOptions;

    std::vector<Window> m_vWindows;
  };

}



#endif
