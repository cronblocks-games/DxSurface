#include "pch.h"
#include "DxSurface.h"

using namespace std;
using namespace CB::DxSurface;

DxSurface::DxSurface()
{
  m_stOptions.debugEnabled = true;
  m_stOptions.defaultWindowName = "DxSurface";
  m_stOptions.defaultWindowRect = WindowRect(10, 10, 250, 250);
}
DxSurface::DxSurface(const DxSurfaceOptions& options)
{
  m_stOptions = options;
}

Window& DxSurface::CreateNewWindow()
{
  return CreateNewWindow(
    m_stOptions.defaultWindowName,
    m_stOptions.defaultWindowRect.x,
    m_stOptions.defaultWindowRect.y,
    m_stOptions.defaultWindowRect.w,
    m_stOptions.defaultWindowRect.h,
    m_stOptions.debugEnabled);
}
Window& DxSurface::CreateNewWindow(
  const string& name,
  int x, int y, int width, int height,
  bool debugEnabled)
{
  m_vWindows.emplace_back(name, x, y, width, height, debugEnabled);
  return m_vWindows[m_vWindows.size() - 1];
}
