#ifndef _DXSURFACE_DXSWINDEBUG_H_
#define _DXSURFACE_DXSWINDEBUG_H_


#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class WinDebug {
  public:
    static void PrintWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);
  };

} //- namespace CB::DxSurface


#endif
