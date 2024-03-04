#include "pch.hpp"
#include "DxsKeyboard.hpp"


using namespace CB::DxSurface;


KeyStatus Keyboard::GetKeyStatus(KeyCode k)
{
  if (m_bKeyPressedStatus[(unsigned char)k])
    return KeyStatus::Pressed;

  return KeyStatus::Released;
}

void Keyboard::StartGettingText(unsigned int maxLength, bool stopOnEnterKey, const TString& initialText)
{
  Lock l(m_mutText);

  m_sText = initialText;
  m_bIsGettingText = true;
  m_uiMaxTextLength = maxLength;
  m_bTextStopOnEnterKey = stopOnEnterKey;
}

void Keyboard::StopGettingText()
{
  m_bIsGettingText = false;
}

TString Keyboard::GetCurrentText()
{
  Lock l(m_mutText);
  return m_sText;
}

void Keyboard::SetKeyStatus(KeyCode k, KeyStatus s) noexcept
{
  if (s == KeyStatus::Pressed)
    m_bKeyPressedStatus[(unsigned char)k] = true;
  else
    m_bKeyPressedStatus[(unsigned char)k] = false;
}

void Keyboard::InsertChar(const TChar ch)
{
  if (m_bIsGettingText)
  {
    if (m_bTextStopOnEnterKey &&
      (ch == DxsT('\r') || ch == DxsT('\n')))
    {
      m_bIsGettingText = false;
      return;
    }

    Lock l(m_mutText);

    if (ch == DxsT('\b'))
    {
      if (m_sText.length() > 0)
      {
        m_sText.pop_back();
      }
    }
    else
    {
      if (m_sText.length() < m_uiMaxTextLength)
      {
        m_sText += ch;
      }
    }
  }
}




