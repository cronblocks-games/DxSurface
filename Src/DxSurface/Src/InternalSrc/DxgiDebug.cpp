#include "pch.hpp"
#include "DxgiDebug.hpp"

#pragma comment(lib, "dxguid.lib")

using namespace CB::DxSurface;

DxgiDebug::DxgiDebug(bool isDebuggingEnabled)
  : m_bIsDebuggingEnabled(isDebuggingEnabled)
{
  m_uCount = 0;

  //- No need to load library when debugging is not enabled
  if (!m_bIsDebuggingEnabled) return;
}

void DxgiDebug::Mark() noexcept
{
}

Vector<TString> DxgiDebug::GetMessages() const
{
  return Vector<TString>();
}
