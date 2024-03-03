#include "pch.hpp"
#include "DxsResources.hpp"

using namespace std;
using namespace CB::DxSurface;

WinImageResource::WinImageResource(
  unsigned int resourceId,
  UINT type, int cx, int cy, UINT flags,
  ResourceType r, Source s, HINSTANCE hInstance)
{
  assert(r != ResourceType::NONE);
  assert(s != Source::NONE);

  if (s == Source::ExeEmbedded && hInstance == nullptr)
  {
    hInstance = GetModuleHandle(nullptr);
  }
}
