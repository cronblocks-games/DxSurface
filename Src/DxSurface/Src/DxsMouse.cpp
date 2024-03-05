#include "pch.hpp"
#include "DxsMouse.hpp"

using namespace CB::DxSurface;

Mouse::Mouse()
{
  m_bIsValid = false;
  m_ptPosition = { 0 };
  m_eLeftButtonStatus = m_eRightButtonStatus = m_eMiddleButtonStatus = KeyStatus::Released;
}

POINT Mouse::GetPosition() const noexcept
{
  return m_ptPosition;
}

KeyStatus Mouse::GetButtonStatus(MouseButton b) const noexcept
{
  switch (b)
  {
  case MouseButton::Left: return m_eLeftButtonStatus;
  case MouseButton::Right: return m_eRightButtonStatus;
  case MouseButton::Middle: return m_eMiddleButtonStatus;
  }
  return KeyStatus::Released;
}

void Mouse::ResetKeysStatus() noexcept
{
  m_eLeftButtonStatus = m_eRightButtonStatus = m_eMiddleButtonStatus = KeyStatus::Released;
}

void Mouse::SetPosition(POINT p) noexcept
{
  m_ptPosition = p;
}

void Mouse::SetButtonStatus(MouseButton b, KeyStatus s) noexcept
{
  switch (b)
  {
  case MouseButton::Left: m_eLeftButtonStatus = s; break;
  case MouseButton::Right: m_eRightButtonStatus = s; break;
  case MouseButton::Middle: m_eMiddleButtonStatus = s; break;
  }
}
