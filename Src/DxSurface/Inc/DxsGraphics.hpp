#ifndef _DXSURFACE_DXSGRAPHICS_H_
#define _DXSURFACE_DXSGRAPHICS_H_



#include "DxsTypes.hpp"
#include "DxsDxgiDebug.hpp"

namespace CB::DxSurface {

  class Graphics {
  public:
    Graphics(HWND, bool isDebugEnabled, float clrR = 0.95, float clrG = 0.95, float clrB = 0.95, float clrA = 1.0);
    ~Graphics();

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

  private:
    PtrUnique<DxgiDebug> m_pDebugIface;

    PtrCom<DxDevice> m_pDevice;
    PtrCom<DxDeviceContext> m_pDeviceContext;
    PtrCom<DxSwapChain> m_pSwapChain;
    PtrCom<DxRenderTargetView> m_pRenderTargetView;
    DxFeatureLevel m_eFeatureLevel;

    float m_aClearColor[4];

    void StartFrame();
    void EndFrame();

    friend class CB::DxSurface::Window;

  }; //- class Graphics

} //- namespace CB::DxSurface



#endif
