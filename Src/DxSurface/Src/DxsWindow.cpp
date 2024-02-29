#include "pch.hpp"
#include "DxsWindow.hpp"


using namespace std;
using namespace CB::DxSurface;


Window::Window(HINSTANCE hInstance, const WindowCreationOptions& options)
{
  if (hInstance == nullptr)
  {
    hInstance = GetModuleHandle(nullptr);
  }
  
  m_hInstance = hInstance;
  m_hWnd = nullptr;

  m_sClassName = DxsT("");
  m_stOptions = options;

  m_eWindowCreationState = WindowCreationState::NONE;
  m_pRenderingExecutor.reset();
  m_pProcessingExecutor.reset();
}
Window::Window(const Window& other)
{
  *this = other;
}
Window::~Window()
{
  if (m_pProcessingExecutor)
  {
    m_pProcessingExecutor->Exit();
    m_pProcessingExecutor->WaitForExit();
    m_pProcessingExecutor.reset();
  }
  if (m_pRenderingExecutor)
  {
    m_pRenderingExecutor->Exit();
    m_pRenderingExecutor->WaitForExit();
    m_pRenderingExecutor.reset();
  }
}

Window& Window::operator=(const Window& other)
{
  if (this == &other) return *this;

  m_hInstance = other.m_hInstance;
  m_hWnd = nullptr; // each window object should have its own window and underlying rendering thread

  m_sClassName = DxsT("");
  m_stOptions = other.m_stOptions;

  m_eWindowCreationState = WindowCreationState::NONE;
  m_pRenderingExecutor.reset();
  m_pProcessingExecutor.reset();

  return *this;
}

static Mutex _windowCreationMutex;

void Window::CreateWindowAndRun()
{
  Lock l(_windowCreationMutex);

  if (m_pRenderingExecutor) return;

  m_eWindowCreationState = WindowCreationState::NONE;

  m_pRenderingExecutor = make_shared<TimedExecutor<Window>>(
    this,
    m_stOptions.title + DxsT("/RenderingThread"),
    m_stOptions.maxRenderingThreadRefreshRateHz == 0 ? DxsDefaultThreadRefreshRateHz : m_stOptions.maxRenderingThreadRefreshRateHz,
    &Window::OnRenderingStateInitInternal,
    &Window::OnRenderingStateRunningInternal,
    &Window::OnRenderingStatePausedInternal,
    &Window::OnRenderingStateExittedInternal,
    &Window::OnRenderingStateChangedInternal);
  m_pRenderingExecutor->Run();

  if (m_stOptions.maxProcessingThreadRefreshRateHz != 0)
  {
    m_pProcessingExecutor = make_shared<TimedExecutor<Window>>(
      this,
      m_stOptions.title + DxsT("/ProcessingThread"),
      m_stOptions.maxProcessingThreadRefreshRateHz,
      &Window::OnProcessingStateInitInternal,
      &Window::OnProcessingStateRunningInternal,
      &Window::OnProcessingStatePausedInternal,
      &Window::OnProcessingStateExittedInternal,
      &Window::OnProcessingStateChangedInternal);
    m_pProcessingExecutor->Run();
  }

  while (m_eWindowCreationState == WindowCreationState::NONE &&
    m_pRenderingExecutor->GetState() != ExecutionState::Exitted);
}
ExecutionState Window::RenderingState() const
{
  if (m_pRenderingExecutor)
    return m_pRenderingExecutor->GetState();

  return ExecutionState::NONE;
}
void Window::Pause()
{
  if (m_pRenderingExecutor)
  {
    m_pRenderingExecutor->Pause();
  }
  else
  {
    DxsThrow((Title() + DxsT(" - Cannot pause when rendering isn't initialized")).c_str());
  }
}
void Window::Resume()
{
  if (m_pRenderingExecutor)
  {
    m_pRenderingExecutor->Resume();
  }
  else
  {
    DxsThrow((Title() + DxsT(" - Cannot resume when rendering isn't initialized")).c_str());
  }
}
void Window::Exit()
{
  if (m_pRenderingExecutor)
  {
    m_pRenderingExecutor->Exit();
  }
  if (m_pProcessingExecutor)
  {
    m_pProcessingExecutor->Exit();
  }
}
void Window::WaitForExit()
{
  if (m_pProcessingExecutor)
  {
    m_pProcessingExecutor->WaitForExit();
    m_pProcessingExecutor.reset();
  }
  if (m_pRenderingExecutor)
  {
    m_pRenderingExecutor->WaitForExit();
    m_pRenderingExecutor.reset();
  }
}

const TString& Window::Title() const
{
  return m_stOptions.title;
}
void Window::Title(const TString& title)
{
  if (m_stOptions.title == title) return;

  if (m_hWnd == nullptr)
  {
    DxsThrow((Title() + DxsT(" - Cannot change title of window with invalid handle")).c_str());
  }

  SetWindowText(m_hWnd, title.c_str());

  m_stOptions.title = title;
}

void Window::Show()
{
  if (m_hWnd == nullptr)
  {
    DxsThrow((Title() + DxsT(" - Cannot show window with invalid handle")).c_str());
  }

  ShowWindow(m_hWnd, SW_SHOW);
}
void Window::Hide()
{
  if (m_hWnd == nullptr)
  {
    DxsThrow((Title() + DxsT(" - Cannot hide window with invalid handle")).c_str());
  }

  ShowWindow(m_hWnd, SW_HIDE);
}

bool Window::Primary() const
{
  return m_stOptions.isPrimary;
}
void Window::Primary(bool isPrimary)
{
  m_stOptions.isPrimary = isPrimary;
}


//------------------------------------------------------------------------
//- 
//-   Windows class registration, creation, deletion and
//- message processing.
//- 
//------------------------------------------------------------------------
static Mutex _mutGlobalData;
static int _classRegNum = 0;
static map<HWND, Window*> _handle2WindowMap;

void Window::RegisterClassAndCreateWindow()
{
  if (m_hWnd != nullptr) return;

  if (m_sClassName == DxsT(""))
  {
    Lock lock(_mutGlobalData);

    m_sClassName = DxsT("WinClass_");
    m_sClassName += ++_classRegNum;
  }

  WNDCLASSEX wndcls = { 0 };
  wndcls.cbSize = sizeof(wndcls);
  wndcls.style = CS_OWNDC;
  wndcls.lpfnWndProc = Window::WindowsMessageProc;
  wndcls.cbClsExtra = 0; // extra bytes to allocate following the window-class structure
  wndcls.cbWndExtra = 0; // extra bytes to allocate following the window instance
  wndcls.hInstance = m_hInstance;
  wndcls.hIcon = m_stOptions.hIcon;
  wndcls.hIconSm = m_stOptions.hIconSmall;
  wndcls.hCursor = m_stOptions.hCursor;
  wndcls.hbrBackground = nullptr;
  wndcls.lpszMenuName = nullptr;
  wndcls.lpszClassName = m_sClassName.c_str();
  RegisterClassEx(&wndcls);

  m_hWnd = CreateWindowEx(
    m_stOptions.dwExStyle,                  // dwExStyle
    m_sClassName.c_str(),                   // lpClassName
    Title().c_str(),                        // lpWindowName
    m_stOptions.dwStyle,                    // dwStyle
    m_stOptions.rect.x, m_stOptions.rect.y, // x, y
    m_stOptions.rect.w, m_stOptions.rect.h, // width, height
    nullptr,                                // hWndParent
    nullptr,                                // hMenu
    m_hInstance,                            // hInstance
    nullptr                                 // lpParam
  );

  if (m_hWnd != nullptr)
  {
    MutexLock lock(_mutGlobalData);
    _handle2WindowMap[m_hWnd] = this;

    //- Should be set last so that the waiting thread waits for it to complete
    m_eWindowCreationState = WindowCreationState::Success;
  }
  else
  {
    m_eWindowCreationState = WindowCreationState::Fail;
    DxsThrow(DxsT("Window creation failed"));
  }

  return;
}
void Window::UnRegisterClassAndDestroyWindow()
{
  if (m_hWnd == nullptr) return;

  MutexLock lock(_mutGlobalData);
  
  _handle2WindowMap.erase(m_hWnd);

  DestroyWindow(m_hWnd);
  UnregisterClass(m_sClassName.c_str(), m_hInstance);

  m_hWnd = nullptr;
}

//- 
//- Universal Windows' messages handler
//- 
LRESULT WINAPI Window::WindowsMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  try
  {
    if (_handle2WindowMap.contains(hWnd))
      return _handle2WindowMap[hWnd]->OnWindowsMessage(msg, wParam, lParam);
  } catch (...) {}

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
//- 
//- Local Window's message handler for Windows messages
//- 
LRESULT Window::OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_CLOSE: Exit(); break;
  }

  return DefWindowProc(m_hWnd, msg, wParam, lParam);
}
//- 
//- Windows messages puller
//- 
void Window::ProcessWindowsMessagesQueue()
{
  MSG message;
  while (PeekMessage(&message, m_hWnd, 0, 0, PM_REMOVE) != 0)
  {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}


//------------------------------------------------------------------------
//- 
//- Rendering thread
//- 
//------------------------------------------------------------------------
void Window::OnRenderingStateInitInternal()
{
  RegisterClassAndCreateWindow();
  Show();

  OnRenderingStateInit();

  //- Invoking the externally provided callback
  RenderingCallbackInit callback = m_stOptions.callbacks.renderingCallbackInit.load();
  if (callback != nullptr)
  {
    (*callback)(*this);
  }
}
void Window::OnRenderingStateRunningInternal(const double deltaSec)
{
  ProcessWindowsMessagesQueue();
  OnRenderingStateRunning(deltaSec);

  //- Invoking the externally provided callback
  RenderingCallbackRunning callback = m_stOptions.callbacks.renderingCallbackRunning.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, deltaSec);
  }
}
void Window::OnRenderingStatePausedInternal(const double deltaSec)
{
  ProcessWindowsMessagesQueue();
  OnRenderingStatePaused(deltaSec);

  //- Invoking the externally provided callback
  RenderingCallbackPaused callback = m_stOptions.callbacks.renderingCallbackPaused.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, deltaSec);
  }
}
void Window::OnRenderingStateExittedInternal(ExecutionExitReason reason, const TString& message)
{
  if (m_pProcessingExecutor)
  {
    m_pProcessingExecutor->Exit();
  }

  DxsEncloseThrow(UnRegisterClassAndDestroyWindow());

  OnRenderingStateExitted(reason, message);

  //- Invoking the externally provided callback
  RenderingCallbackExitted callback = m_stOptions.callbacks.renderingCallbackExitted.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, reason, message);
  }

  if (Primary())
  {
    ExitProcess(reason == ExecutionExitReason::Exception ? -1 : 0);
  }
}
void Window::OnRenderingStateChangedInternal(ExecutionState last, ExecutionState next)
{
  OnRenderingStateChanged(last, next);

  //- Invoking the externally provided callback
  RenderingCallbackStateChanged callback = m_stOptions.callbacks.renderingCallbackStateChanged.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, last, next);
  }
}


//------------------------------------------------------------------------
//- 
//- Processing thread
//- 
//------------------------------------------------------------------------
void Window::OnProcessingStateInitInternal()
{
  OnProcessingStateInit();

  //- Invoking the externally provided callback
  ProcessingCallbackInit callback = m_stOptions.callbacks.processingCallbackInit.load();
  if (callback != nullptr)
  {
    (*callback)(*this);
  }
}
void Window::OnProcessingStateRunningInternal(const double deltaSec)
{
  OnProcessingStateRunning(deltaSec);

  //- Invoking the externally provided callback
  ProcessingCallbackRunning callback = m_stOptions.callbacks.processingCallbackRunning.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, deltaSec);
  }
}
void Window::OnProcessingStatePausedInternal(const double deltaSec)
{
  OnProcessingStatePaused(deltaSec);

  //- Invoking the externally provided callback
  ProcessingCallbackPaused callback = m_stOptions.callbacks.processingCallbackPaused.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, deltaSec);
  }
}
void Window::OnProcessingStateExittedInternal(ExecutionExitReason r, const TString& message)
{
  if (m_stOptions.maxProcessingThreadRefreshRateHz != 0 && m_pRenderingExecutor)
  {
    m_pRenderingExecutor->Exit();
  }

  OnProcessingStateExitted(r, message);

  //- Invoking the externally provided callback
  ProcessingCallbackExitted callback = m_stOptions.callbacks.processingCallbackExitted.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, r, message);
  }
}
void Window::OnProcessingStateChangedInternal(ExecutionState last, ExecutionState next)
{
  OnProcessingStateChanged(last, next);

  //- Invoking the externally provided callback
  ProcessingCallbackStateChanged callback = m_stOptions.callbacks.processingCallbackStateChanged.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_cKeyboard, m_cMouse, last, next);
  }
}



