#ifndef _DXSURFACE_DXSGRAPHICS_H_
#define _DXSURFACE_DXSGRAPHICS_H_



#include "DxsTypes.hpp"
#include "DxsDxgiDebugInterface.hpp"

namespace CB::DxSurface {

  class Graphics {
  public:
    Graphics(HWND, float clrR = 0.96, float clrG = 0.92, float clrB = 0.96, float clrA = 1.0);
    ~Graphics() = default;

    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) noexcept = delete;
    Graphics& operator =(const Graphics&) = delete;
    Graphics& operator =(Graphics&&) noexcept = delete;

    void SetClearColor(float clrR, float clrG, float clrB, float clrA = 1.0);
    void SetClearColorAndStartFrame(float clrR, float clrG, float clrB, float clrA = 1.0);

    DxFeatureLevel GetFeatureLevel();
    PtrCom<DxDevice> GetDevice();
    PtrCom<DxDeviceContext> GetDeviceContext();
    PtrCom<DxSwapChain> GetSwapChain();
    PtrCom<DxRenderTargetView> GetRenderTargetView();

    void SetVertexShaderFromCso(const TString& filename);
    void SetPixelShaderFromCso(const TString& filename);

  private:
#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
    PtrUnique<DxgiDebugInterface> m_pDebugIface;
#endif

    PtrCom<DxDevice> m_pDevice;
    PtrCom<DxDeviceContext> m_pDeviceContext;
    PtrCom<DxSwapChain> m_pSwapChain;
    PtrCom<DxRenderTargetView> m_pRenderTargetView;
    PtrCom<DxVertexShader> m_pVertexShader;
    PtrCom<DxPixelShader> m_pPixelShader;
    DxFeatureLevel m_eFeatureLevel;

    unsigned long long m_ullFramesCountTotal, m_ullFramesCountFps; TimePoint m_tpTotalT0, m_tpFpsT0; double m_fFps;
    float m_aClearColor[4];

    void StartFrame();
    void EndFrame();

    friend class CB::DxSurface::Window;

  }; //- class Graphics

} //- namespace CB::DxSurface



#endif
