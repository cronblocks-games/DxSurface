#ifndef _DXSURFACE_DXSGRAPHICS_H_
#define _DXSURFACE_DXSGRAPHICS_H_



#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class Graphics {
  public:
    Graphics(HWND, bool isDebugEnabled);

  private:
    HWND m_hWnd;
    bool m_bIsDebugEnabled;

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
