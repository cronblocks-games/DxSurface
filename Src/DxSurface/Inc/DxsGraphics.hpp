#ifndef _DXSURFACE_DXSGRAPHICS_H_
#define _DXSURFACE_DXSGRAPHICS_H_



#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class Graphics {
  public:
    Graphics(HWND, bool isDebugEnabled, float bgR = 0.88, float bgG = 0.88, float bgB = 0.88, float bgA = 1.0);
    ~Graphics();

    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) noexcept = delete;
    Graphics& operator =(const Graphics&) = delete;
    Graphics& operator =(Graphics&&) noexcept = delete;

    void SetBackground(float bgR, float bgG, float bgB, float bgA = 1.0);
    void SetBackgroundAndClearFrame(float bgR, float bgG, float bgB, float bgA = 1.0);

  private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
    D3D_FEATURE_LEVEL m_eFeatureLevel;

    float m_bgR, m_bgG, m_bgB, m_bgA;

    void StartFrame();
    void EndFrame();

    friend class CB::DxSurface::Window;
  };

} //- namespace CB::DxSurface



#endif
