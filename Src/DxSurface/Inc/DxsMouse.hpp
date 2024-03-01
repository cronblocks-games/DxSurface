#ifndef _DXSURFACE_DXSMOUSE_H_
#define _DXSURFACE_DXSMOUSE_H_



#include "DxsHelpers.hpp"

namespace CB::DxSurface {

  class Mouse {
  public:
    Mouse();
    POINT GetPosition() const noexcept;

  private:
    bool m_bIsValid;
    POINT m_ptPosition;

    void SetPosition(POINT) noexcept;

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
