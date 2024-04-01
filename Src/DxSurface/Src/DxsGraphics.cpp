#include "pch.hpp"
#include "DxsGraphics.hpp"

#pragma comment(lib, "d3d11.lib")

#define DxCall(call,fail_msg) { HRESULT hr = call; if (hr != S_OK) DxsThrowGraphicsHr(fail_msg,hr); }


using namespace CB::DxSurface;
using namespace Microsoft::WRL;


Graphics::Graphics(HWND hWnd, bool isDebugEnabled, float clrR, float clrG, float clrB, float clrA)
  : m_clrR(clrR), m_clrG(clrG), m_clrB(clrB), m_clrA(clrA)
{
  DXGI_SWAP_CHAIN_DESC scd = { 0 };
  scd.BufferDesc.Width = 0;
  scd.BufferDesc.Height = 0;
  scd.BufferDesc.RefreshRate.Numerator = 0;
  scd.BufferDesc.RefreshRate.Denominator = 0;
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
      isDebugEnabled ? D3D11_CREATE_DEVICE_DEBUG : 0, // Flags
      nullptr, 0,               // pFeatureLevels, FeatureLevels
      D3D11_SDK_VERSION,        // SDKVersion
      &scd,                     // pSwapChainDesc
      &m_pSwapChain,            // ppSwapChain
      &m_pDevice,               // ppDevice
      &m_eFeatureLevel,         // pFeatureLevel (created)
      &m_pContext               // ppImmediateContext
    ), DxsT("Failed creating device and swap chain, check if you have SDK installed and enable debugging"));

  ComPtr<ID3D11Resource> backBuffer;
  DxCall(
    m_pSwapChain->GetBuffer(
      0,                        // Buffer
      __uuidof(ID3D11Resource), // REFIID
      &backBuffer               // ppSurface
  ), DxsT("Failed getting back buffer from the swap chain"));

  DxCall(
    m_pDevice->CreateRenderTargetView(
      backBuffer.Get(),    // pResource
      nullptr,             // pDesc
      &m_pRenderTargetView // ppRTView
    ), DxsT("Failed creating Render Target View"));
}

Graphics::~Graphics()
{
}

void Graphics::SetClearColor(float clrR, float clrG, float clrB, float clrA)
{
  m_clrR = clrR; m_clrG = clrG; m_clrB = clrB; m_clrA = clrA;
}

void Graphics::SetClearColorAndStartFrame(float clrR, float clrG, float clrB, float clrA)
{
  m_clrR = clrR; m_clrG = clrG; m_clrB = clrB; m_clrA = clrA;
  StartFrame();
}

D3D_FEATURE_LEVEL Graphics::GetFeatureLevel()
{
  return m_eFeatureLevel;
}

void Graphics::StartFrame()
{
  float clr[] = { m_clrR, m_clrG, m_clrB, m_clrA };
  m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clr);
}

void Graphics::EndFrame()
{
  m_pSwapChain->Present(1, 0);
}
