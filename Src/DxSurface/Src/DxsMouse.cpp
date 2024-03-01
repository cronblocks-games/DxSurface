#include "pch.hpp"
#include "DxsMouse.hpp"

using namespace CB::DxSurface;

Mouse::Mouse()
{
  m_bIsValid = false;
  m_ptPosition = { 0 };
}

POINT CB::DxSurface::Mouse::GetPosition() const noexcept
{
  return m_ptPosition;
}

void Mouse::SetPosition(POINT p) noexcept
{
  m_ptPosition = p;
}
