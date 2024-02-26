#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include "DxsHelpers.h"


namespace CB::DxSurface {

  class Window {
  public:
    Window() = delete;
    Window(HINSTANCE hInstance, const WindowCreationOptions& options);
    Window(const Window& other);
    Window(Window&& other) noexcept;

    Window& operator=(const Window& other);
    Window& operator=(Window&& other) noexcept;

    virtual ~Window();

    bool Primary() const;
    void Primary(bool isPrimary);

    const TString& Title() const;
    void Title(const TString&);

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
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    mutable TString m_sClassName;
    mutable WindowCreationOptions m_stOptions;

    Ptr<Thread> m_pThread;
    volatile enum class WindowCreationState m_eWindowCreationState;
    volatile enum class RenderingState m_eRenderingState, m_eRenderingStateCommand;

    void RegisterClassAndCreateWindow();
    void UnRegisterClassAndDestroyWindow();

    void OnRenderingStateInitInternal();
    void OnRenderingStateRunningInternal();
    void OnRenderingStateExitingInternal();
    void OnRenderingStateChangedInternal(
      enum class RenderingState last,
      enum class RenderingState next);

    void ProcessWindowsMessagesQueue();
    LRESULT OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT WINAPI WindowsMessageProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    void RenderingThreadInit();
    void RenderingThreadExit(enum class ThreadExitReason);
    void RenderingThreadSetRenderingState(enum class RenderingState);
    void RenderingThreadProcessRenderingState();
    static void RenderingThread(Window*);
  };

} //- namespace CB::DxSurface



#endif
