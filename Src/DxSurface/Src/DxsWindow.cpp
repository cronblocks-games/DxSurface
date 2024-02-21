#include "pch.h"
#include "DxsWindow.h"


using namespace CB::DxSurface;

Window::~Window()
{
}

Window::Window(const std::string& name, int x, int y, int width, int height, bool isPrimary, bool debugEnabled)
{
  this->name = name;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->isPrimary = isPrimary;
  this->debugEnabled = debugEnabled;
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
  this->name = other.name;
  this->x = other.x;
  this->y = other.y;
  this->width = other.width;
  this->height = other.height;
  this->isPrimary = other.isPrimary;
  this->debugEnabled = other.debugEnabled;

  return *this;
}

Window& Window::operator=(Window&& other) noexcept
{
  this->name = std::move(other.name);
  this->x = other.x;
  this->y = other.y;
  this->width = other.width;
  this->height = other.height;
  this->isPrimary = other.isPrimary;
  this->debugEnabled = other.debugEnabled;

  other.x = 0;
  other.y = 0;
  other.width = 0;
  other.height = 0;
  other.isPrimary = false;
  other.debugEnabled = false;

  return *this;
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

const std::string& Window::GetName()
{
  return name;
}
