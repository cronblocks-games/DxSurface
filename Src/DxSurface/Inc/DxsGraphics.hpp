#ifndef _DXSURFACE_DXSGRAPHICS_H_
#define _DXSURFACE_DXSGRAPHICS_H_



#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class Graphics {
  public:
    Graphics(HWND, bool isDebugEnabled);
    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) noexcept = delete;
    Graphics& operator =(const Graphics&) = delete;
    Graphics& operator =(Graphics&&) noexcept = delete;
    ~Graphics();

  private:
    HWND m_hWnd;
    bool m_bIsDebugEnabled;

    void StartFrame();
    void EndFrame();

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
