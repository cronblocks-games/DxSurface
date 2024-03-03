#ifndef _DXSURFACE_DXSMOUSE_H_
#define _DXSURFACE_DXSMOUSE_H_



#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class Mouse {
  public:
    Mouse();

    POINT GetPosition() const noexcept;
    KeyStatus GetButtonStatus(MouseButton) const noexcept;

  private:
    bool m_bIsValid;
    POINT m_ptPosition;
    KeyStatus m_eLeftButtonStatus, m_eRightButtonStatus, m_eMiddleButtonStatus;

    void SetPosition(POINT) noexcept;
    void SetButtonStatus(MouseButton, KeyStatus) noexcept;

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
