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
    ExecutionState RenderingState() const;
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
    virtual void OnRenderingStateChanged(ExecutionState last, ExecutionState next);

  private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    mutable TString m_sClassName;
    mutable WindowCreationOptions m_stOptions;

    Ptr<Thread> m_pRenderingThread;
    Ptr<Thread> m_pProcessingThread;
    volatile WindowCreationState m_eWindowCreationState;
    volatile ExecutionState m_eRenderingState, m_eProcessingState;
    volatile ExecutionCommand m_eThreadsCommand;

    void RegisterClassAndCreateWindow();
    void UnRegisterClassAndDestroyWindow();

    void OnRenderingStateInitInternal();
    void OnRenderingStateRunningInternal();
    void OnRenderingStateExitingInternal();
    void OnRenderingStateChangedInternal(ExecutionState last, ExecutionState next);

    void ProcessWindowsMessagesQueue();
    LRESULT OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT WINAPI WindowsMessageProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    void RenderingThreadDoInit();
    void RenderingThreadDoExit(ExecutionExitReason);
    void RenderingThreadDoSetRenderingState(ExecutionState);
    void RenderingThreadDoProcessRenderingState();
    static void RenderingThread(Window* const);
    static void ProcessingThread(Window* const);
  };

} //- namespace CB::DxSurface



#endif
