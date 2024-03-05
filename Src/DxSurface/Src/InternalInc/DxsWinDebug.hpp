#ifndef _DXSURFACE_DXSWINDEBUG_H_
#define _DXSURFACE_DXSWINDEBUG_H_


#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class WinDebug {
  public:
    static void PrintWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    static void PrintKeyStatus(KeyCode, KeyStatus);
    static void PrintCharReceived(TChar);
    static void PrintMouseClientPosition(POINT);
    static void PrintMouseClientButtonStatus(MouseButton, KeyStatus, POINT);
  };

} //- namespace CB::DxSurface


#endif
