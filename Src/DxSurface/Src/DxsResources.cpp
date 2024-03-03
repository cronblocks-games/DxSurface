#include "pch.hpp"
#include "DxsResources.hpp"

using namespace std;
using namespace CB::DxSurface;

WinImageResource::WinImageResource(
  unsigned int resourceId,
  int cx, int cy, UINT flags,
  ResourceType r, Source s, HINSTANCE hInstance)
{
  assert(r != ResourceType::NONE);
  assert(s != Source::NONE);
  assert(s != Source::File);

  m_eSource = s;

  flags &= ~LR_LOADFROMFILE; // we are not loading from a file in this constructor

  if (s == Source::ExeEmbedded && hInstance == nullptr)
  {
    hInstance = GetModuleHandle(nullptr);
  }

  if (s == Source::System)
  {
    hInstance = nullptr;
    flags |= LR_SHARED;
  }

  UINT type;
  switch (r)
  {
  case ResourceType::Icon: type = IMAGE_ICON; break;
  case ResourceType::Cursor: type = IMAGE_CURSOR; break;
  case ResourceType::Bitmap: type = IMAGE_BITMAP; break;
  default: DxsThrow(DxsT("Cannot load an unspecified resource type")); break;
  }

  if (DxsFailedPtr(m_hResource = LoadImage(hInstance, MAKEINTRESOURCE(resourceId), type, cx, cy, flags)))
  {
    DxsThrowWindows(DxsT("Cannot load the specified resource"));
  }

  IncrementHandleCount(m_hResource);
}

WinImageResource::WinImageResource(TString filepath, int cx, int cy, UINT flags, ResourceType r, Source s)
{
  assert(r != ResourceType::NONE);
  assert(s != Source::NONE);
  assert(s != Source::System);
  assert(s != Source::ExeEmbedded);

  m_eSource = s;

  flags |= LR_LOADFROMFILE; // we are only loading from a file in this constructor
  flags &= ~LR_SHARED; // Windows API recommends to unset this flag when loading from file

  UINT type;
  switch (r)
  {
  case ResourceType::Icon: type = IMAGE_ICON; break;
  case ResourceType::Cursor: type = IMAGE_CURSOR; break;
  case ResourceType::Bitmap: type = IMAGE_BITMAP; break;
  default: DxsThrow(DxsT("Cannot load an unspecified resource type")); break;
  }

  if (DxsFailedPtr(m_hResource = LoadImage(nullptr, filepath.c_str(), type, cx, cy, flags)))
  {
    DxsThrowWindows((TString(DxsT("Cannot load the specified resource from file ")) + filepath).c_str());
  }

  IncrementHandleCount(m_hResource);
}

WinImageResource::WinImageResource(const WinImageResource& o)
{
  if (this == &o) return;
  m_hResource = o.m_hResource;
  m_eSource = o.m_eSource;
  IncrementHandleCount(m_hResource);
}

WinImageResource::WinImageResource(WinImageResource&& o) noexcept
{
  if (this == &o) return;
  m_hResource = exchange(o.m_hResource, nullptr);
  m_eSource = exchange(o.m_eSource, Source::NONE);
}

WinImageResource::~WinImageResource()
{
  assert(m_hResource != nullptr); // Child class should do the freeing of resource
}

WinImageResource& WinImageResource::operator=(const WinImageResource& o)
{
  if (this == &o) return *this;
  *this = WinImageResource(o);
  return *this;
}

WinImageResource& WinImageResource::operator=(WinImageResource&& o) noexcept
{
  if (this == &o) return *this;
  *this = WinImageResource(move(o));
  return *this;
}

unsigned int WinImageResource::IncrementHandleCount(HANDLE rh) noexcept
{
  Lock l(s_mutResourceCount);

  if (s_mapResourceCount.contains(rh))
  {
    s_mapResourceCount[rh] += 1;
  }
  else
  {
    s_mapResourceCount[rh] = 1;
  }

  return s_mapResourceCount[rh];
}

unsigned int WinImageResource::DecrementHandleCount(HANDLE rh)
{
  Lock l(s_mutResourceCount);

  assert(s_mapResourceCount.contains(rh));
  assert(s_mapResourceCount[rh] > 0);

  s_mapResourceCount[rh] -= 1;

  return s_mapResourceCount[rh];
}

Icon::Icon(SystemIcon si, int prefWidth, int prefHeight, UINT loadFlags)
  : WinImageResource((unsigned int)si, prefWidth, prefHeight, loadFlags, ResourceType::Icon, Source::System)
{
}

Icon::Icon(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags, HINSTANCE hInstance)
  : WinImageResource(resourceId, prefWidth, prefHeight, loadFlags, ResourceType::Icon, Source::ExeEmbedded, hInstance)
{
}

Icon::Icon(TString filepath, int prefWidth, int prefHeight, UINT loadFlags)
  : WinImageResource(filepath, prefWidth, prefHeight, loadFlags, ResourceType::Icon, Source::File)
{
}

Icon::Icon(const Icon& o)
  : WinImageResource(o)
{
}

Icon::Icon(Icon&& o) noexcept
  : WinImageResource(move(o))
{
}

Icon::~Icon()
{
  unsigned int count = DecrementHandleCount(m_hResource);

  if (m_eSource != Source::System)
  {
    DestroyIcon((HICON)m_hResource);
  }

  m_hResource = nullptr;
}

Icon& Icon::operator=(const Icon& o)
{
  if (this == &o) return *this;
  WinImageResource::operator=(o);
  return *this;
}

Icon& CB::DxSurface::Icon::operator=(Icon&& o) noexcept
{
  if (this == &o) return *this;
  WinImageResource::operator=(move(o));
  return *this;
}
