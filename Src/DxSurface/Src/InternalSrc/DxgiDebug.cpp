#include "pch.hpp"
#include "DxgiDebug.hpp"

#pragma comment(lib, "dxguid.lib")

using namespace CB::DxSurface;

DxgiDebug::DxgiDebug()
{
  m_uCount = 0;
}

void DxgiDebug::Mark() noexcept
{
}

Vector<TString> DxgiDebug::GetMessages() const
{
  return Vector<TString>();
}
