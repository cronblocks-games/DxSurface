#ifndef _DXSURFACE_DXSURFACE_H_
#define _DXSURFACE_DXSURFACE_H_

#include <string>
#include <vector>

#include "DxsTypes.h"
#include "DxsWinApi.h"
#include "DxsWindow.h"
#include "DxsExceptions.h"


namespace CB {
  namespace DxSurface {

    class DxSurface {

    public:
      DxSurface();
      DxSurface(const DxSurfaceOptions& options);

      DxSurface(DxSurface&) = delete;
      DxSurface(DxSurface&&) = delete;
      DxSurface& operator=(DxSurface&) = delete;
      DxSurface& operator=(DxSurface&&) = delete;

      ~DxSurface() = default;

      Window& CreateNewWindow();
      Window& CreateNewWindow(const std::string& name, int x, int y, int width, int height, bool debugEnabled = true);

    private:
      DxSurfaceOptions m_stOptions;

      std::vector<Window> m_vWindows;
    };
  }
}



#endif
