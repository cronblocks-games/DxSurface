#include "pch.hpp"
#include "DxsGraphics.hpp"

using namespace CB::DxSurface;

Graphics::Graphics(HWND hWnd, bool isDebugEnabled)
  : m_hWnd(hWnd), m_bIsDebugEnabled(isDebugEnabled)
{ 
}

Graphics::~Graphics()
{
}

void Graphics::StartFrame()
{
}

void Graphics::EndFrame()
{
}
