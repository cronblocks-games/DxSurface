#ifndef _DXSURFACE_DXSKEYBOARD_H_
#define _DXSURFACE_DXSKEYBOARD_H_



#include "DxsHelpers.hpp"
#include <bitset>

namespace CB::DxSurface {

  class Keyboard {
  public:
    KeyStatus GetKeyStatus(KeyCode);

  private:
    std::bitset<(1 << (8 * sizeof(KeyCode)))> m_bKeyPressedStatus;

    void SetKeyStatus(KeyCode, KeyStatus) noexcept;

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
