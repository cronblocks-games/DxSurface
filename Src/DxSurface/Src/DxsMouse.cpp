#include "pch.hpp"
#include "DxsMouse.hpp"

using namespace CB::DxSurface;

Mouse::Mouse()
{
  m_ptPosition =
    m_ptLeftButtonDownPosition = m_ptRightButtonDownPosition = m_ptMiddleButtonDownPosition =
    m_ptLeftButtonUpPosition = m_ptRightButtonUpPosition = m_ptMiddleButtonUpPosition = { 0 };
  m_eLeftButtonStatus = m_eRightButtonStatus = m_eMiddleButtonStatus = KeyStatus::Released;
  m_stScrollStatus = { 0 };
  m_bIsCaptured = false;
}

POINT Mouse::GetPosition() const noexcept
{
  return m_ptPosition;
}

POINT Mouse::GetPositionAtButtonStatus(MouseButton b, KeyStatus s) const noexcept
{
  switch (b)
  {
  case MouseButton::Left:
    switch (s)
    {
    case KeyStatus::Pressed: return m_ptLeftButtonDownPosition;
    case KeyStatus::Released: return m_ptLeftButtonUpPosition;
    }
    break;
  case MouseButton::Right:
    switch (s)
    {
    case KeyStatus::Pressed: return m_ptRightButtonDownPosition;
    case KeyStatus::Released: return m_ptRightButtonUpPosition;
    }
    break;
  case MouseButton::Middle:
    switch (s)
    {
    case KeyStatus::Pressed: return m_ptMiddleButtonDownPosition;
    case KeyStatus::Released: return m_ptMiddleButtonUpPosition;
    }
    break;
  }

  return { 0 };
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

bool Mouse::IsAnyButtonPressed() const noexcept
{
  return
    m_eLeftButtonStatus == KeyStatus::Pressed ||
    m_eRightButtonStatus == KeyStatus::Pressed ||
    m_eMiddleButtonStatus == KeyStatus::Pressed;
}

Mouse::ScrollStatus Mouse::GetScrollStatus() const noexcept
{
  return m_stScrollStatus;
}

void Mouse::ResetScrollStatus() noexcept
{
  m_stScrollStatus = { 0 };
}

void Mouse::ResetButtonsStatus() noexcept
{
  m_eLeftButtonStatus = m_eRightButtonStatus = m_eMiddleButtonStatus = KeyStatus::Released;
  m_ptLeftButtonDownPosition = m_ptRightButtonDownPosition = m_ptMiddleButtonDownPosition =
    m_ptLeftButtonUpPosition = m_ptRightButtonUpPosition = m_ptMiddleButtonUpPosition = { 0 };
  m_bIsCaptured = false;
}

void Mouse::SetPosition(POINT p) noexcept
{
  m_ptPosition = p;
}

void Mouse::SetButtonStatus(MouseButton b, KeyStatus s, POINT pt) noexcept
{
  switch (b)
  {
  case MouseButton::Left:
    m_eLeftButtonStatus = s;
    switch (s)
    {
    case KeyStatus::Pressed: m_ptLeftButtonDownPosition = pt; break;
    case KeyStatus::Released: m_ptLeftButtonUpPosition = pt; break;
    }
    break;

  case MouseButton::Right:
    m_eRightButtonStatus = s;
    switch (s)
    {
    case KeyStatus::Pressed: m_ptRightButtonDownPosition = pt; break;
    case KeyStatus::Released: m_ptRightButtonUpPosition = pt; break;
    }
    break;

  case MouseButton::Middle:
    m_eMiddleButtonStatus = s;
    switch (s)
    {
    case KeyStatus::Pressed: m_ptMiddleButtonDownPosition = pt; break;
    case KeyStatus::Released: m_ptMiddleButtonUpPosition = pt; break;
    }
    break;
  }
}

void Mouse::SetScrollStatus(long scroll, POINT pt) noexcept
{
  m_stScrollStatus.value += scroll;
  m_stScrollStatus.position = pt;
}

bool Mouse::IsCaptured() noexcept
{
  return m_bIsCaptured;
}
void Mouse::SetCaptured(bool c) noexcept
{
  m_bIsCaptured = c;
}
