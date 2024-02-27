#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include "DxsHelpers.hpp"


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

    void CreateWindowAndRun();
    RenderingState RenderingState() const;
    void Pause();
    void Resume();
    void Exit();
    void WaitForExit();

    const TString& Title() const;
    void Title(const TString&);

    void Show();
    void Hide();
    
    bool Primary() const;
    void Primary(bool isPrimary);

  protected:
    virtual void OnRenderingStateInit();
    virtual void OnRenderingStateRunning();
    virtual void OnRenderingStateExiting();
    virtual void OnRenderingStateChanged(enum class RenderingState last, enum class RenderingState next);

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
    void OnRenderingStateChangedInternal(enum class RenderingState last, enum class RenderingState next);

    void ProcessWindowsMessagesQueue();
    LRESULT OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT WINAPI WindowsMessageProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    void RenderingThreadDoInit();
    void RenderingThreadDoExit(enum class ThreadExitReason);
    void RenderingThreadDoSetRenderingState(enum class RenderingState);
    void RenderingThreadDoProcessRenderingState();
    static void RenderingThread(Window*);
  };

} //- namespace CB::DxSurface



#endif
