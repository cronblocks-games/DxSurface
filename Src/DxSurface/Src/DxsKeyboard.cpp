#include "pch.hpp"
#include "DxsKeyboard.hpp"


using namespace CB::DxSurface;


KeyStatus Keyboard::GetKeyStatus(KeyCode k)
{
  if (m_bKeyPressedStatus[(unsigned char)k])
    return KeyStatus::Pressed;

  return KeyStatus::Released;
}

void Keyboard::SetKeyStatus(KeyCode k, KeyStatus s) noexcept
{
  if (s == KeyStatus::Pressed)
    m_bKeyPressedStatus[(unsigned char)k] = true;
  else
    m_bKeyPressedStatus[(unsigned char)k] = false;
}




