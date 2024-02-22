#include "pch.h"
#include "DxsWindow.h"


using namespace std;
using namespace CB::DxSurface;


Window::~Window()
{
}

Window::Window(const std::string& title, int x, int y, int width, int height, bool isPrimary, bool debugEnabled)
{
  this->m_sTitle = title;
  this->m_iX = x;
  this->m_iY = y;
  this->m_iWidth = width;
  this->m_iHeight = height;
  this->m_bIsPrimary = isPrimary;
  this->m_bIsDebugEnabled = debugEnabled;

  this->m_hWnd = nullptr;

  this->m_hWnd = RegisterClassAndCreateWindow();

  Show();
}
Window::Window(const Window& other)
{
  *this = other;
}
Window::Window(Window&& other) noexcept
{
  *this = std::move(other);
}

Window& Window::operator=(const Window& other)
{
  this->m_sTitle = other.m_sTitle;
  this->m_iX = other.m_iX;
  this->m_iY = other.m_iY;
  this->m_iWidth = other.m_iWidth;
  this->m_iHeight = other.m_iHeight;
  this->m_bIsPrimary = other.m_bIsPrimary;
  this->m_bIsDebugEnabled = other.m_bIsDebugEnabled;

  this->m_hWnd = other.m_hWnd;

  return *this;
}

Window& Window::operator=(Window&& other) noexcept
{
  this->m_sTitle = std::move(other.m_sTitle);
  this->m_iX = other.m_iX;
  this->m_iY = other.m_iY;
  this->m_iWidth = other.m_iWidth;
  this->m_iHeight = other.m_iHeight;
  this->m_bIsPrimary = other.m_bIsPrimary;
  this->m_bIsDebugEnabled = other.m_bIsDebugEnabled;

  this->m_hWnd = other.m_hWnd;

  other.m_iX = 0;
  other.m_iY = 0;
  other.m_iWidth = 0;
  other.m_iHeight = 0;
  other.m_bIsPrimary = false;
  other.m_bIsDebugEnabled = false;

  other.m_hWnd = nullptr;

  return *this;
}

bool Window::IsPrimary()
{
  return m_bIsPrimary;
}

const std::string& Window::Title()
{
  return m_sTitle;
}

const std::string& Window::Title(const string& title)
{
  if (m_sTitle == title) return m_sTitle;

  if (m_hWnd == nullptr)
  {
    DXSURFACE_THROW("Cannot change title of window with invalid handle");
  }

  SetWindowText(m_hWnd, title.c_str());

  m_sTitle = title;
  return m_sTitle;
}

void Window::Show()
{
  if (m_hWnd == nullptr)
  {
    DXSURFACE_THROW("Cannot show window with invalid handle");
  }

  ShowWindow(m_hWnd, SW_SHOW);
}

void Window::Hide()
{
  if (m_hWnd == nullptr)
  {
    DXSURFACE_THROW("Cannot hide window with invalid handle");
  }

  ShowWindow(m_hWnd, SW_HIDE);
}

void Window::RunRendering()
{
}



//------------------------------------------------------------------------
//- 
//- Window class registration and creation
//- 
//------------------------------------------------------------------------
static mutex reg_class_mutex;
static map<string, WNDCLASSEX> reg_classes;

HWND Window::RegisterClassAndCreateWindow()
{
  const lock_guard<mutex> lock(reg_class_mutex);

  DXSURFACE_THROW("Not implemented: RegisterClassAndCreateWindow");
}


