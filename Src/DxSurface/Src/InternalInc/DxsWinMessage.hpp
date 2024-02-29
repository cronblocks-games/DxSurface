#ifndef _DXSURFACE_DXSWINDOWSMESSAGE_H_
#define _DXSURFACE_DXSWINDOWSMESSAGE_H_


#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class WindowsMessage {
  public:
    static TString ToTString(UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
  };

} //- namespace CB::DxSurface


#endif
