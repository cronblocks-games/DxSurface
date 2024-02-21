#include "pch.h"
#include "DxsWindow.h"


using namespace CB::DxSurface;

Window::~Window()
{
}

Window::Window(const std::string& name, int x, int y, int width, int height, bool isPrimary, bool debugEnabled)
{
  this->m_sName = name;
  this->m_iX = x;
  this->m_iY = y;
  this->m_iWidth = width;
  this->m_iHeight = height;
  this->m_bIsPrimary = isPrimary;
  this->m_bIsDebugEnabled = debugEnabled;

  this->m_hWnd = nullptr;

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
  this->m_sName = other.m_sName;
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
  this->m_sName = std::move(other.m_sName);
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

const std::string& Window::Name()
{
  return m_sName;
}

void Window::Show()
{
}

void Window::Hide()
{
}

void Window::RunRendering()
{
}
