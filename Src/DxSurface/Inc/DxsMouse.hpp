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
    POINT GetPositionAtButtonStatus(MouseButton, ButtonStatus) const noexcept;
    ButtonStatus GetButtonStatus(MouseButton) const noexcept;
    bool IsAnyButtonPressed() const noexcept;
    ScrollStatus GetScrollStatus() const noexcept;
    void ResetScrollStatus() noexcept;

  private:
    POINT m_ptPosition,
      m_ptLeftButtonDownPosition, m_ptRightButtonDownPosition, m_ptMiddleButtonDownPosition,
      m_ptLeftButtonUpPosition,   m_ptRightButtonUpPosition,   m_ptMiddleButtonUpPosition;
    ButtonStatus m_eLeftButtonStatus, m_eRightButtonStatus, m_eMiddleButtonStatus;
    ScrollStatus m_stScrollStatus;
    bool m_bIsCaptured;

    void ResetButtonsStatus() noexcept;
    void SetPosition(POINT) noexcept;
    void SetButtonStatus(MouseButton, ButtonStatus, POINT) noexcept;
    void SetScrollStatus(long, POINT) noexcept;
    bool IsCaptured() const noexcept;
    void SetCaptured(bool) noexcept;

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
