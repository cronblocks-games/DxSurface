#include "pch.h"
#include "DxSurface.h"

using namespace std;
using namespace CB::DxSurface;

DxSurface::DxSurface(HINSTANCE hInstance)
{
  if (hInstance == nullptr)
  {
    DxsThrow(DxsT("Invalid HINSTANCE provided"));
  }
  m_hInstance = hInstance;
  m_stOptions.debugEnabled = true;
  m_stOptions.defaultWindowTitle = DxsT("DxSurface");
  m_stOptions.defaultWindowRect = WindowRectangle(10, 10, 250, 250);
}
DxSurface::DxSurface(HINSTANCE hInstance, const WindowCreationOptions& options)
{
  if (hInstance == nullptr)
  {
    DxsThrow(DxsT("Invalid HINSTANCE provided"));
  }
  m_hInstance = hInstance;
  m_stOptions = options;
}
DxSurface::DxSurface(const DxSurface& other)
{
  *this = other;
}
DxSurface::DxSurface(DxSurface&& other) noexcept
{
  *this = move(other);
}
DxSurface& DxSurface::operator=(const DxSurface& other)
{
  m_hInstance = other.m_hInstance;
  m_stOptions = other.m_stOptions;
  m_vWindows.clear();
  for (auto& w : other.m_vWindows)
  {
    m_vWindows.push_back(w);
  }

  return *this;
}
DxSurface& DxSurface::operator=(DxSurface&& other) noexcept
{
  m_hInstance = other.m_hInstance;
  other.m_hInstance = nullptr;

  m_stOptions = move(other.m_stOptions);
  m_vWindows.clear();
  m_vWindows = move(other.m_vWindows);

  return *this;
}

Window* DxSurface::CreateNewWindow()
{
  return CreateNewWindow(
    m_stOptions.defaultWindowTitle,
    m_stOptions.defaultWindowRect.x,
    m_stOptions.defaultWindowRect.y,
    m_stOptions.defaultWindowRect.w,
    m_stOptions.defaultWindowRect.h,
    m_stOptions.debugEnabled);
}
Window* DxSurface::CreateNewWindow(
  const TString& title,
  int x, int y, int width, int height,
  bool isPrimary,
  bool debugEnabled)
{
  Window* w = new Window(m_hInstance, title, x, y, width, height, isPrimary, debugEnabled);
  m_vWindows.push_back(w);
  return w;
}

void DxSurface::Run()
{
  while (1)
  {
    if (m_vWindows.size() == 0) break;
    
    for (auto w = m_vWindows.cbegin(); w != m_vWindows.cend();)
    {
      if ((*w)->RenderingState() == RenderingState::Exitted)
      {
        w = m_vWindows.erase(w);
      }
      else
      {
        w++;
      }
    }

    Sleep(50);
  }
}
