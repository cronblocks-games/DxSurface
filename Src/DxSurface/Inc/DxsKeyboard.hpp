#ifndef _DXSURFACE_DXSKEYBOARD_H_
#define _DXSURFACE_DXSKEYBOARD_H_



#include "DxsHelpers.hpp"
#include <bitset>

namespace CB::DxSurface {

  class Keyboard {
  public:
    enum class KeyStatus KeyStatus(enum class KeyCode);

  private:
    std::bitset<(1 << (8 * sizeof(enum class KeyCode)))> m_bKeyPressedStatus;

    void SetKeyStatus(enum class KeyCode, enum class KeyStatus);

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
