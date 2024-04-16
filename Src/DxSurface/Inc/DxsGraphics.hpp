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

    unsigned long long GetTotalFrames();
    float GetAverageFPS();
    float GetCurrentFPS();

    void SetVertexShaderFromCso(const TString& filename);
    void SetPixelShaderFromCso(const TString& filename);

    void SetVertexShaderFromText(const TString& shaderText);
    void SetPixelShaderFromText(const TString& shaderText);

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

    unsigned long long m_ullFramesCountTotal, m_ullFramesCountFps; TimePoint m_tpTotalT0, m_tpFpsT0; float m_fFps;
    float m_aClearColor[4];

    void StartFrame();
    void EndFrame();

    friend class CB::DxSurface::Window;


    /* ---------------------------------------------------------------- */
    /* Internal handling                                                */
    /* ---------------------------------------------------------------- */
    //- Invoked from `StartFrame` - Updating frames' count and FPS
    void _UpdateFrameCount();
    //- Invoked from `ctor` - Setting-up a default pipeline
    void _SetDefaultPipeline();

  }; //- class Graphics

} //- namespace CB::DxSurface



#endif
