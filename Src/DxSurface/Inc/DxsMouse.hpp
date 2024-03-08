#ifndef _DXSURFACE_DXSMOUSE_H_
#define _DXSURFACE_DXSMOUSE_H_



#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class Mouse {
  public:
    struct ScrollStatus
    {
      long value;     // -ve => Scroll Down (towards user), +ve => Scroll Up (away from user)
      POINT position; // Last point's position w.r.t. client's origin where scroll happened
    };

    Mouse();

    POINT GetPosition() const noexcept;
    POINT GetPositionAtButtonStatus(MouseButton, KeyStatus) const noexcept;
    KeyStatus GetButtonStatus(MouseButton) const noexcept;
    ScrollStatus GetScrollStatus() const noexcept;
    void ResetScrollStatus() noexcept;

  private:
    POINT m_ptPosition,
      m_ptLeftButtonDownPosition, m_ptRightButtonDownPosition, m_ptMiddleButtonDownPosition,
      m_ptLeftButtonUpPosition,   m_ptRightButtonUpPosition,   m_ptMiddleButtonUpPosition;
    KeyStatus m_eLeftButtonStatus, m_eRightButtonStatus, m_eMiddleButtonStatus;
    ScrollStatus m_stScrollStatus;

    void ResetButtonsStatus() noexcept;
    void SetPosition(POINT) noexcept;
    void SetButtonStatus(MouseButton, KeyStatus, POINT) noexcept;
    void SetScrollStatus(long, POINT) noexcept;

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
