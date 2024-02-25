#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include <memory>
#include <thread>

#include "DxsTypes.h"
#include "DxsWinApi.h"


namespace CB::DxSurface {

  class Window {
  public:
    Window() = delete;
    Window(const String& title, int x, int y, int width, int height, bool isPrimary, bool debugEnabled);
    Window(const Window& other);
    Window(Window&& other) noexcept;

    Window& operator=(const Window& other);
    Window& operator=(Window&& other) noexcept;

    virtual ~Window();

    bool Primary() const;
    void Primary(bool isPrimary);

    const String& Title() const;
    void Title(const String&);

    void Show();
    void Hide();

    void PauseRendering();
    void ResumeRendering();
    void ExitRendering();
    RenderingState RenderingState() const;

    void WaitForExit() const;

  protected:
    virtual void OnRenderingStateInit();
    virtual void OnRenderingStateRunning();
    virtual void OnRenderingStateExiting();
    virtual void OnRenderingStateChanged(
      enum class RenderingState last,
      enum class RenderingState next);

  private:
    mutable String m_sTitle;
    int m_iX, m_iY, m_iWidth, m_iHeight;
    bool m_bIsPrimary, m_bIsDebugEnabled;

    std::unique_ptr<std::thread> m_pThread;
    volatile enum class WindowCreationState m_eWindowCreationState;
    volatile enum class RenderingState m_eRenderingState, m_eRenderingStateCommand;

    HWND m_hWnd;

    HWND RegisterClassAndCreateWindow();

    void OnRenderingStateInitInternal();
    void OnRenderingStateRunningInternal();
    void OnRenderingStateExitingInternal();
    void OnRenderingStateChangedInternal(
      enum class RenderingState last,
      enum class RenderingState next);

    static void RenderingThread(Window*);
  };

}



#endif
