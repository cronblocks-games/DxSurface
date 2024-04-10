#include "pch.hpp"
#include "DxsGraphics.hpp"

#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")

using namespace CB::DxSurface;
using namespace std;


static DxFeatureLevel prefFeatureLevels[] = {
    DxFeatureLevel::_11_1,  DxFeatureLevel::_11_0,
    DxFeatureLevel::_10_1,  DxFeatureLevel::_10_0,
    DxFeatureLevel::_9_3,   DxFeatureLevel::_9_2,  DxFeatureLevel::_9_1
};
static unsigned int prefFeatureLevelsCount = sizeof(prefFeatureLevels) / sizeof(DxFeatureLevel);


Graphics::Graphics(HWND hWnd, float clrR, float clrG, float clrB, float clrA)
{
#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
    m_pDebugIface = make_unique<DxgiDebugInterface>();
#endif

  SetClearColor(clrR, clrG, clrB, clrA);

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
  scd.BufferCount = 2;
  scd.OutputWindow = hWnd;
  scd.Windowed = TRUE;
  scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  scd.Flags = 0;

  DxCall(
    D3D11CreateDeviceAndSwapChain(
      nullptr,                  // pAdapter
      D3D_DRIVER_TYPE_HARDWARE, // DriverType
      nullptr,                  // Software
      DxDeviceCreationFlags,    // Flags
      (D3D_FEATURE_LEVEL*)prefFeatureLevels, prefFeatureLevelsCount, // pFeatureLevels, FeatureLevels
      D3D11_SDK_VERSION,        // SDKVersion
      &scd,                     // pSwapChainDesc
      &m_pSwapChain,            // ppSwapChain
      &m_pDevice,               // ppDevice
      reinterpret_cast<D3D_FEATURE_LEVEL*>(&m_eFeatureLevel), // pFeatureLevel (created device's feature level)
      &m_pDeviceContext         // ppImmediateContext
    ), DxsT("Device and swap chain creation failed."));

  PtrCom<DxResource> backBuffer;
  DxCall(
    m_pSwapChain->GetBuffer(
      0, __uuidof(DxResource), &backBuffer  // Buffer, REFIID, ppSurface
    ), DxsT("Failed getting back buffer from swap chain."));

  DxCall(
    m_pDevice->CreateRenderTargetView(
      backBuffer.Get(), nullptr, &m_pRenderTargetView  // pResource, pDesc, ppRTView
    ), DxsT("Failed creating Render Target View."));
}

void Graphics::SetClearColor(float clrR, float clrG, float clrB, float clrA)
{
  m_aClearColor[0] = clrR;
  m_aClearColor[1] = clrG;
  m_aClearColor[2] = clrB;
  m_aClearColor[3] = clrA;
}
void Graphics::SetClearColorAndStartFrame(float clrR, float clrG, float clrB, float clrA)
{
  SetClearColor(clrR, clrG, clrB, clrA);
  StartFrame();
}

DxFeatureLevel             Graphics::GetFeatureLevel()     { return m_eFeatureLevel;     }
PtrCom<DxDevice>           Graphics::GetDevice()           { return m_pDevice;           }
PtrCom<DxDeviceContext>    Graphics::GetDeviceContext()    { return m_pDeviceContext;    }
PtrCom<DxSwapChain>        Graphics::GetSwapChain()        { return m_pSwapChain;        }
PtrCom<DxRenderTargetView> Graphics::GetRenderTargetView() { return m_pRenderTargetView; }

void Graphics::StartFrame()
{
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), m_aClearColor);
}
void Graphics::EndFrame()
{
#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
  if (m_pDebugIface) { m_pDebugIface->Mark(); }
#endif

  HRESULT hr = m_pSwapChain->Present(1, 0);

  if (hr == DXGI_ERROR_DEVICE_REMOVED)
  {
    DxsThrowGraphicsHr(DxsT("DXGI_ERROR_DEVICE_REMOVED"), m_pDevice->GetDeviceRemovedReason());
  }
  else if (hr != S_OK)
  {
#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
    if (m_pDebugIface) { DxsThrowGraphicsHr(m_pDebugIface->GetMessages().c_str(), hr); }
    else { DxsThrowGraphicsHr(DxsT("Failure in presenting frame"), hr); }
#else
    DxsThrowGraphicsHr(DxsT("Failure in presenting frame"), hr);
#endif
  }
}

//- 
//- Shaders
//- 
void Graphics::SetVertexShaderFromCso(const TString& filename)
{
  PtrCom<DxBlob> blob;

  wstringstream ss;
  ss << filename;

  DxCall(D3DReadFileToBlob(ss.str().c_str(), &blob));
  DxCall(m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pVertexShader));
}