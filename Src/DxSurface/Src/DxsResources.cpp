#include "pch.hpp"
#include "DxsResources.hpp"

using namespace std;
using namespace CB::DxSurface;

WinImageResource::WinImageResource(
  unsigned int resourceId,
  UINT type, int cx, int cy, UINT flags,
  ResourceType r, Source s, HINSTANCE hInstance)
{
  if (s == Source::ExeEmbedded && hInstance == nullptr)
  {
    hInstance = GetModuleHandle(nullptr);
  }
}
