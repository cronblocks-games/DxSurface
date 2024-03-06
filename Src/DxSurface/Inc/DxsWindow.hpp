#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include "DxsTypes.hpp"
#include "DxsResources.hpp"
#include "DxsTimedExecutor.hpp"
#include "DxsKeyboard.hpp"
#include "DxsMouse.hpp"


namespace CB::DxSurface {

  class Window {
  public:
    Window() = delete;
    Window(HINSTANCE hInstance, const WindowCreationOptions& options);
    Window(const Window& other);
    Window(Window&&) = delete;

    Window& operator=(const Window& other);
    Window& operator=(Window&& other) = delete;

    virtual ~Window();

    void CreateWindowAndRun();
    ExecutionState RenderingState() const;
    void Pause();
    void Resume();
    void Exit();
    void WaitForExit();

    const TString& GetTitle() const;
    void SetTitle(const TString&);

    void SetCursor(SystemCursor, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE);
    void SetCursor(unsigned int resourceId, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE, HINSTANCE hInstance = nullptr);
    void SetCursor(const TString& filepath, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE);

    void Show();
    void Hide();
    
    bool GetPrimary() const;
    void SetPrimary(bool isPrimary);

    void SetCallbacks(const WindowCallbacks&);

  protected:
    virtual void OnRenderingStateInit() {}
    virtual void OnRenderingStateRunning(const double deltaSec) {}
    virtual void OnRenderingStatePaused(const double deltaSec) {}
    virtual void OnRenderingStateExitted(ExecutionExitReason, const TString& message) {}
    virtual void OnRenderingStateChanged(ExecutionState last, ExecutionState next) {}

    virtual void OnProcessingStateInit() {}
    virtual void OnProcessingStateRunning(const double deltaSec) {}
    virtual void OnProcessingStatePaused(const double deltaSec) {}
    virtual void OnProcessingStateExitted(ExecutionExitReason, const TString& message) {}
    virtual void OnProcessingStateChanged(ExecutionState last, ExecutionState next) {}

  private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    mutable TString m_sClassName;
    mutable WindowCreationOptions m_stOptions;

    volatile WindowCreationState m_eWindowCreationState;
    
    Ptr<TimedExecutor<Window>> m_pRenderingExecutor;
    Ptr<TimedExecutor<Window>> m_pProcessingExecutor;

    Keyboard m_cKeyboard;
    Mouse m_cMouse;

    void RegisterClassAndCreateWindow();
    void UnRegisterClassAndDestroyWindow();

    void OnRenderingStateInitInternal();
    void OnRenderingStateRunningInternal(const double deltaSec);
    void OnRenderingStatePausedInternal(const double deltaSec);
    void OnRenderingStateExittedInternal(ExecutionExitReason, const TString& message);
    void OnRenderingStateChangedInternal(ExecutionState last, ExecutionState next);

    void OnProcessingStateInitInternal();
    void OnProcessingStateRunningInternal(const double deltaSec);
    void OnProcessingStatePausedInternal(const double deltaSec);
    void OnProcessingStateExittedInternal(ExecutionExitReason, const TString& message);
    void OnProcessingStateChangedInternal(ExecutionState last, ExecutionState next);

    void ProcessWindowsMessagesQueue();
    LRESULT OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT WINAPI WindowsMessageProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
  };

} //- namespace CB::DxSurface



#endif
