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
