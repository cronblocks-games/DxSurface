#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include <string>
#include <memory>
#include <thread>

#include "DxsTypes.h"
#include "DxsWinApi.h"


namespace CB::DxSurface {

  class Window {
  public:
    Window() = delete;
    Window(const std::string& title, int x, int y, int width, int height, bool isPrimary, bool debugEnabled);
    Window(const Window& other);
    Window(Window&& other) noexcept;

    Window& operator=(const Window& other);
    Window& operator=(Window&& other) noexcept;

    virtual ~Window();

    bool IsPrimary();
    const std::string& Title();
    const std::string& Title(const std::string&);

    void Show();
    void Hide();

    void PauseRendering();
    void ResumeRendering();
    void ExitRendering();
    RenderingState RenderingState() const;

  protected:
    virtual void OnRenderingStateInit();
    virtual void OnRenderingStateRunning();
    virtual void OnRenderingStateExiting();
    virtual void OnRenderingStateChanged(
      enum class RenderingState last,
      enum class RenderingState next);

  private:
    mutable std::string m_sTitle;
    int m_iX, m_iY, m_iWidth, m_iHeight;
    bool m_bIsPrimary, m_bIsDebugEnabled;

    std::shared_ptr<std::thread> m_pThread;
    volatile enum class RenderingState m_eRenderingState, m_eRenderingStateCommand;

    HWND m_hWnd;

    HWND RegisterClassAndCreateWindow();

    void OnRenderingStateRunningDefault();
    static void RenderingThread(Window*);
  };

}



#endif
