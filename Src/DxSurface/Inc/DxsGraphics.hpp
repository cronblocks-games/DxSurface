#ifndef _DXSURFACE_DXSGRAPHICS_H_
#define _DXSURFACE_DXSGRAPHICS_H_



#include "DxsTypes.hpp"

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

  private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
    D3D_FEATURE_LEVEL m_eFeatureLevel;

    float m_clrR, m_clrG, m_clrB, m_clrA;

    void StartFrame();
    void EndFrame();

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
