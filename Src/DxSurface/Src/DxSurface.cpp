#include "pch.hpp"
#include "DxSurface.hpp"

using namespace std;
using namespace CB::DxSurface;

DxSurface::DxSurface(HINSTANCE hInstance) noexcept
{
  m_hInstance = hInstance;
  m_stOptions.isGraphicsDebugEnabled = true;
  m_stOptions.isWindowsMessagesDebugEnabled = false;
  m_stOptions.isKeyboardDebugEnabled = false;
  m_stOptions.isMouseDebugEnabled = false;
  m_stOptions.icon = Resources::LoadIconResource(SystemIcon::Note, 32, 32);
  m_stOptions.iconSmall = Resources::LoadIconResource(SystemIcon::Note, 16, 16);
  m_stOptions.cursor = Resources::LoadCursorResource(SystemCursor::Hand);
}
DxSurface::DxSurface(const WindowCreationOptions& options, HINSTANCE hInstance) noexcept
{
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
  m_hInstance = exchange(other.m_hInstance, nullptr);
  m_stOptions = move(other.m_stOptions);

  m_vWindows.clear();
  m_vWindows = move(other.m_vWindows);

  return *this;
}

void DxSurface::UpdateWindowCreationOptions(const WindowCreationOptions& options) noexcept
{
  m_stOptions = options;
}

Ptr<Window> DxSurface::CreateNewWindow()
{
  return CreateNewWindow(m_stOptions);
}
Ptr<Window> DxSurface::CreateNewWindow(const TString& title)
{
  WindowCreationOptions op = m_stOptions;
  op.title = title;

  return CreateNewWindow(op);
}
Ptr<Window> DxSurface::CreateNewWindow(const TString& title, int x, int y)
{
  WindowCreationOptions op = m_stOptions;
  op.title = title;
  op.rect.x = x;
  op.rect.y = y;

  return CreateNewWindow(op);
}
Ptr<Window> DxSurface::CreateNewWindow(
  const TString& title,
  int x, int y, int width, int height,
  bool isPrimary,
  bool graphicsDebugEnabled)
{
  WindowCreationOptions op = m_stOptions;
  op.title = title;
  op.rect.x = x;
  op.rect.y = y;
  op.rect.w = width;
  op.rect.h = height;
  op.isPrimary = isPrimary;
  op.isGraphicsDebugEnabled = graphicsDebugEnabled;

  return CreateNewWindow(op);
}
Ptr<Window> DxSurface::CreateNewWindow(const WindowCreationOptions& options)
{
  Ptr<Window> w = make_shared<Window>(options, m_hInstance);
  m_vWindows.push_back(w);
  return w;
}

void DxSurface::RunAndWaitForExit()
{
  for (auto& w : m_vWindows)
  {
    w->CreateWindowAndRun();
  }

  while (1)
  {
    if (m_vWindows.size() == 0) break;
    
    for (auto w = m_vWindows.cbegin(); w != m_vWindows.cend();)
    {
      if ((*w)->RenderingState() == ExecutionState::Exitted)
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
