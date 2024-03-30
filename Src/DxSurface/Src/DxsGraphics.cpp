#include "pch.hpp"
#include "DxsGraphics.hpp"

#pragma comment(lib, "d3d11.lib")

#define DxCall(call,fail_msg) { HRESULT hr = call; if (hr != S_OK) DxsThrowGraphicsHr(fail_msg,hr); }


using namespace CB::DxSurface;
using namespace Microsoft::WRL;


Graphics::Graphics(HWND hWnd, bool isDebugEnabled)
{
  DXGI_SWAP_CHAIN_DESC scd = { 0 };
  scd.BufferDesc.Width = 0;
  scd.BufferDesc.Height = 0;
  scd.BufferDesc.RefreshRate.Numerator = 0;
  scd.BufferDesc.RefreshRate.Denominator = 0;
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
  scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  scd.SampleDesc.Count = 1;
  scd.SampleDesc.Quality = 0;
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.BufferCount = 1;
  scd.OutputWindow = hWnd;
  scd.Windowed = TRUE;
  scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  scd.Flags = 0;

  DxCall(
    D3D11CreateDeviceAndSwapChain(
      nullptr,                  // pAdapter
      D3D_DRIVER_TYPE_HARDWARE, // DriverType
      nullptr,                  // Software
      isDebugEnabled ? D3D11_CREATE_DEVICE_DEBUG  | D3D11_CREATE_DEVICE_DEBUGGABLE : 0, // Flags
      nullptr, 0,               // pFeatureLevels, FeatureLevels
      D3D11_SDK_VERSION,        // SDKVersion
      &scd,                     // pSwapChainDesc
      &m_pSwapChain,            // ppSwapChain
      &m_pDevice,               // ppDevice
      nullptr,                  // pFeatureLevel (created)
      &m_pContext               // ppImmediateContext
    ), DxsT("Failed creating device and swap chain"));
}

Graphics::~Graphics()
{
}

void Graphics::StartFrame()
{
}

void Graphics::EndFrame()
{
  m_pSwapChain->Present(1, 0);
}
