#ifndef _DXSURFACE_DXSKEYBOARD_H_
#define _DXSURFACE_DXSKEYBOARD_H_



#include "DxsTypes.hpp"
#include <bitset>

namespace CB::DxSurface {

  class Keyboard {
  public:
    KeyStatus GetKeyStatus(KeyCode);
    bool IsGettingText();
    void StartGettingText(unsigned int maxLength = 50, bool stopOnEnterKey = true, const TString& initialText = DxsT(""));
    void StopGettingText();
    TString GetCurrentText();

  private:
    std::bitset<(1 << (8 * sizeof(KeyCode)))> m_bKeyPressedStatus;
    Mutex m_mutText;
    bool m_bIsGettingText = false, m_bTextStopOnEnterKey = true;
    unsigned int m_uiMaxTextLength;
    TString m_sText = DxsT("");

    void SetKeyStatus(KeyCode, KeyStatus) noexcept;
    void InsertChar(const TChar);

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
