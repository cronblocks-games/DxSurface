#include "pch.hpp"
#include "DxsKeyboard.hpp"


using namespace CB::DxSurface;


enum class KeyStatus Keyboard::KeyStatus(enum class KeyCode k)
{
  if (m_bKeyPressedStatus[(unsigned char)k])
    return KeyStatus::Pressed;

  return KeyStatus::Released;
}

void Keyboard::SetKeyStatus(enum class KeyCode k, enum class KeyStatus s)
{
  if (s == KeyStatus::Pressed)
    m_bKeyPressedStatus[(unsigned char)k] = true;
  else
    m_bKeyPressedStatus[(unsigned char)k] = false;
}




