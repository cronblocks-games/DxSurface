#include "pch.hpp"
#include "DxsGraphics.hpp"

#pragma comment(lib, "d3d11.lib")

#define DxCall(call,fail_msg) { HRESULT hr = call; if (hr != S_OK) DxsThrowGraphicsHr(fail_msg,hr); }


using namespace CB::DxSurface;


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

  D3D_FEATURE_LEVEL prefFL[] = {
    D3D_FEATURE_LEVEL_11_1,  D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,  D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,   D3D_FEATURE_LEVEL_9_2,  D3D_FEATURE_LEVEL_9_1,
  };

  DxCall(
    D3D11CreateDeviceAndSwapChain(
      nullptr,                  // pAdapter
      D3D_DRIVER_TYPE_HARDWARE, // DriverType
      nullptr,                  // Software
      (isDebugEnabled ? D3D11_CREATE_DEVICE_DEBUG : 0),   // Flags
      prefFL, sizeof(prefFL) / sizeof(D3D_FEATURE_LEVEL), // pFeatureLevels, FeatureLevels
      D3D11_SDK_VERSION,        // SDKVersion
      &scd,                     // pSwapChainDesc
      &m_pSwapChain,            // ppSwapChain
      &m_pDevice,               // ppDevice
      &m_eFeatureLevel,         // pFeatureLevel (created)
      &m_pContext               // ppImmediateContext
    ), DxsT("Device creation failed, if debug flag is set, check debugger output window for more details."));

  PtrCom<DxResource> backBuffer;
  DxCall(
    m_pSwapChain->GetBuffer(
      0, __uuidof(DxResource), &backBuffer  // Buffer, REFIID, ppSurface
    ), DxsT("Failed getting back buffer from swap chain"));

  DxCall(
    m_pDevice->CreateRenderTargetView(
      backBuffer.Get(), nullptr, &m_pRenderTargetView  // pResource, pDesc, ppRTView
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

D3D_FEATURE_LEVEL Graphics::GetFeatureLevel()              { return m_eFeatureLevel;     }
PtrCom<DxDevice> Graphics::GetDevice()                     { return m_pDevice;           }
PtrCom<DxSwapChain> Graphics::GetSwapChain()               { return m_pSwapChain;        }
PtrCom<DxDeviceContext> Graphics::GetContext()             { return m_pContext;          }
PtrCom<DxRenderTargetView> Graphics::GetRenderTargetView() { return m_pRenderTargetView; }

void Graphics::StartFrame()
{
  float clr[] = { m_clrR, m_clrG, m_clrB, m_clrA };
  m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clr);
}

void Graphics::EndFrame()
{
  HRESULT hr = m_pSwapChain->Present(1, 0);

  if (hr == DXGI_ERROR_DEVICE_REMOVED)
  {
    DxsThrowGraphicsHr(DxsT("DXGI_ERROR_DEVICE_REMOVED"), m_pDevice->GetDeviceRemovedReason());
  }
  else if (hr != S_OK)
  {
    DxsThrowGraphicsHr(DxsT("Failure during frame presentation"), hr);
  }
}
