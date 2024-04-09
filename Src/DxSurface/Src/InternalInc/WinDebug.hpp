#ifndef _DXSURFACE_DXSWINDEBUG_H_
#define _DXSURFACE_DXSWINDEBUG_H_


#include "DxsTypes.hpp"

namespace CB::DxSurface {

  // Allows printing specifically formatted messages to the attached
  // debug console.
  class WinDebug {
  public:
    // Prints given Windows API 'msg' like WM_* as string to the attached debug console.
    static void PrintWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    // Prints a member name of enum KeyCode as string and the status as provided to the attached debug console.
    static void PrintKeyStatus(KeyCode, KeyStatus);
    // Prints provided char to the attached debug console as if it is received from keyboard.
    static void PrintCharReceived(TChar);
    // Prints provided point as mouse position within client region to the attached debug console.
    static void PrintMouseClientPosition(POINT);
    // Prints provided mouse button, its status and point to the attached debug console.
    static void PrintMouseClientButtonStatus(MouseButton, ButtonStatus, POINT);
    // Prints provided mouse wheel status and point to the attached debug console.
    static void PrintMouseWheelStatus(short, POINT);
  };

} //- namespace CB::DxSurface


#endif
