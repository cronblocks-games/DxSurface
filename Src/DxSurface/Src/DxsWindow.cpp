#include "pch.hpp"
#include "DxsWindow.hpp"


using namespace std;
using namespace CB::DxSurface;


Window::Window(const WindowCreationOptions& options, HINSTANCE hInstance) :
  m_hWnd(nullptr), m_sClassName(DxsT("")), m_stOptions(options),
  m_hInstance(hInstance == nullptr ? GetModuleHandle(nullptr) : hInstance),
  m_stWindowRect({ 0 }), m_stClientRect({ 0 }),
  m_eWindowCreationState(WindowCreationState::NotInitiated)
{
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
  m_stWindowRect = { 0 };
  m_stClientRect = { 0 };

  m_eWindowCreationState = WindowCreationState::NotInitiated;
  m_pRenderingExecutor.reset();
  m_pProcessingExecutor.reset();
  m_pGraphics.reset();

  return *this;
}

static Mutex _windowCreationMutex;

void Window::CreateWindowAndRun()
{
  Lock l(_windowCreationMutex);

  if (m_pRenderingExecutor) return;

  m_eWindowCreationState = WindowCreationState::NotInitiated;

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

  while (m_eWindowCreationState == WindowCreationState::NotInitiated &&
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
    DxsThrow((GetTitle() + DxsT(" - Cannot pause when rendering isn't initialized")).c_str());
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
    DxsThrow((GetTitle() + DxsT(" - Cannot resume when rendering isn't initialized")).c_str());
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

const TString& Window::GetTitle() const
{
  return m_stOptions.title;
}
void Window::SetTitle(const TString& title)
{
  if (m_stOptions.title == title) return;

  if (m_hWnd == nullptr)
  {
    DxsThrow((GetTitle() + DxsT(" - Cannot change title of window with invalid handle")).c_str());
  }

  if (DxsFailed(SetWindowText(m_hWnd, title.c_str())))
  {
    DxsThrowWindows((GetTitle() + DxsT(" - Cannot change title")).c_str());
  }

  m_stOptions.title = title;
}

WindowRect Window::GetWindowRect() const
{
  return m_stWindowRect;
}
ClientRect Window::GetClientRect() const
{
  return m_stClientRect;
}

void Window::SetIcon(SystemIcon si, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.icon = m_stOptions.iconSmall = Resources::LoadIconResource(si, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HICON, (LONG_PTR)((HICON)*m_stOptions.icon));
  SetClassLongPtrA(m_hWnd, GCLP_HICONSM, (LONG_PTR)((HICON)*m_stOptions.iconSmall));
}
void Window::SetIcon(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags, HINSTANCE hInstance)
{
  m_stOptions.icon = m_stOptions.iconSmall = Resources::LoadIconResource(resourceId, prefWidth, prefHeight, loadFlags, hInstance);
  SetClassLongPtrA(m_hWnd, GCLP_HICON, (LONG_PTR)((HICON)*m_stOptions.icon));
  SetClassLongPtrA(m_hWnd, GCLP_HICONSM, (LONG_PTR)((HICON)*m_stOptions.iconSmall));
}
void Window::SetIcon(const TString& filepath, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.icon = m_stOptions.iconSmall = Resources::LoadIconResource(filepath, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HICON, (LONG_PTR)((HICON)*m_stOptions.icon));
  SetClassLongPtrA(m_hWnd, GCLP_HICONSM, (LONG_PTR)((HICON)*m_stOptions.iconSmall));
}

void Window::SetIconLarge(SystemIcon si, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.icon = Resources::LoadIconResource(si, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HICON, (LONG_PTR)((HICON)*m_stOptions.icon));
}
void Window::SetIconLarge(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags, HINSTANCE hInstance)
{
  m_stOptions.icon = Resources::LoadIconResource(resourceId, prefWidth, prefHeight, loadFlags, hInstance);
  SetClassLongPtrA(m_hWnd, GCLP_HICON, (LONG_PTR)((HICON)*m_stOptions.icon));
}
void Window::SetIconLarge(const TString& filepath, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.icon = Resources::LoadIconResource(filepath, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HICON, (LONG_PTR)((HICON)*m_stOptions.icon));
}

void Window::SetIconSmall(SystemIcon si, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.iconSmall = Resources::LoadIconResource(si, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HICONSM, (LONG_PTR)((HICON)*m_stOptions.iconSmall));
}
void Window::SetIconSmall(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags, HINSTANCE hInstance)
{
  m_stOptions.iconSmall = Resources::LoadIconResource(resourceId, prefWidth, prefHeight, loadFlags, hInstance);
  SetClassLongPtrA(m_hWnd, GCLP_HICONSM, (LONG_PTR)((HICON)*m_stOptions.iconSmall));
}
void Window::SetIconSmall(const TString& filepath, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.iconSmall = Resources::LoadIconResource(filepath, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HICONSM, (LONG_PTR)((HICON)*m_stOptions.iconSmall));
}

void Window::SetCursor(SystemCursor sc, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.cursor = Resources::LoadCursorResource(sc, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)((HCURSOR)*m_stOptions.cursor));
  ::SetCursor(*m_stOptions.cursor);
}
void Window::SetCursor(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags, HINSTANCE hInstance)
{
  m_stOptions.cursor = Resources::LoadCursorResource(resourceId, prefWidth, prefHeight, loadFlags, hInstance);
  SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)((HCURSOR)*m_stOptions.cursor));
  ::SetCursor(*m_stOptions.cursor);
}
void Window::SetCursor(const TString& filepath, int prefWidth, int prefHeight, UINT loadFlags)
{
  m_stOptions.cursor = Resources::LoadCursorResource(filepath, prefWidth, prefHeight, loadFlags);
  SetClassLongPtrA(m_hWnd, GCLP_HCURSOR, (LONG_PTR)((HCURSOR)*m_stOptions.cursor));
  ::SetCursor(*m_stOptions.cursor);
}

void Window::Show()
{
  if (m_hWnd == nullptr)
  {
    DxsThrow((GetTitle() + DxsT(" - Cannot show window with invalid handle")).c_str());
  }

  ShowWindow(m_hWnd, SW_SHOW);
}
void Window::Hide()
{
  if (m_hWnd == nullptr)
  {
    DxsThrow((GetTitle() + DxsT(" - Cannot hide window with invalid handle")).c_str());
  }

  ShowWindow(m_hWnd, SW_HIDE);
}

bool Window::GetPrimary() const
{
  return m_stOptions.isPrimary;
}
void Window::SetPrimary(bool isPrimary)
{
  m_stOptions.isPrimary = isPrimary;
}

void Window::SetCallbacks(const WindowCallbacks& callbacks)
{
  m_stOptions.callbacks = callbacks;
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
  wndcls.hIcon = m_stOptions.icon ? *m_stOptions.icon : nullptr;
  wndcls.hIconSm = m_stOptions.iconSmall ? *m_stOptions.iconSmall : nullptr;
  wndcls.hCursor = m_stOptions.cursor ? *m_stOptions.cursor : nullptr;
  wndcls.hbrBackground = nullptr;
  wndcls.lpszMenuName = nullptr;
  wndcls.lpszClassName = m_sClassName.c_str();

  if (DxsFailed(RegisterClassEx(&wndcls)))
  {
    DxsThrowWindows(DxsT("Class registration failed"));
  }

  RECT winSize(
    m_stOptions.rect.x,
    m_stOptions.rect.y,
    m_stOptions.rect.x + m_stOptions.rect.w,
    m_stOptions.rect.y + m_stOptions.rect.h);

  if (DxsFailed(AdjustWindowRect(&winSize, m_stOptions.dwStyle, false)))
  {
    DxsThrowWindows(DxsT("Client area adjustment failed"));
  }

  m_stWindowRect.x = winSize.left;
  m_stWindowRect.y = winSize.top;
  m_stWindowRect.w = winSize.right - winSize.left;
  m_stWindowRect.h = winSize.bottom - winSize.top;

  m_hWnd = CreateWindowEx(
    m_stOptions.dwExStyle,                  // dwExStyle
    m_sClassName.c_str(),                   // lpClassName
    GetTitle().c_str(),                     // lpWindowName
    m_stOptions.dwStyle,                    // dwStyle
    m_stWindowRect.x, m_stWindowRect.y,     // x, y
    m_stWindowRect.w, m_stWindowRect.h,     // width, height
    nullptr,                                // hWndParent
    nullptr,                                // hMenu
    m_hInstance,                            // hInstance
    nullptr                                 // lpParam
  );

  if (m_hWnd != nullptr)
  {
    m_pGraphics = make_unique<Graphics>(m_hWnd, m_stOptions.isGraphicsDebugEnabled);

    MutexLock lock(_mutGlobalData);
    _handle2WindowMap[m_hWnd] = this;

    //- Should be set last so that the waiting thread waits for it to complete
    m_eWindowCreationState = WindowCreationState::Successful;
  }
  else
  {
    m_eWindowCreationState = WindowCreationState::Failed;
    DxsThrowWindows(DxsT("Window creation failed"));
  }

  return;
}
void Window::UnRegisterClassAndDestroyWindow()
{
  if (m_hWnd == nullptr) return;

  m_pGraphics.reset();

  MutexLock lock(_mutGlobalData);
  
  _handle2WindowMap.erase(m_hWnd);

  if (DxsFailed(DestroyWindow(m_hWnd)))
  {
    //- To continue cleanup we are not throwing exception
    // DxsThrowWindows(DxsT("Cleanup failed - window deletion"));
  }

  if (DxsFailed(UnregisterClass(m_sClassName.c_str(), m_hInstance)))
  {
    DxsThrowWindows(DxsT("Cleanup failed - class unregister"));
  }

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
  if (m_stOptions.isWindowsMessagesDebugEnabled)
    WinDebug::PrintWindowsMessage(msg, wParam, lParam);

  switch (msg)
  {
  case WM_CLOSE: Exit(); break;
  case WM_KILLFOCUS:
  {
    m_cKeyboard.ResetKeysStatus();

    //-   Releasing capture before calling ResetButtonsStatus because it also
    //- resets the capture flag.
    if (m_cMouse.IsCaptured())
    {
      ReleaseCapture();
      m_cMouse.SetCaptured(false);
    }

    //- Calling after processing the capture flag because this function resets the flag.
    m_cMouse.ResetButtonsStatus();
  }
  break;
  case WM_MOVE:
  {
    RECT winRect = { 0 };

    if (DxsFailed(::GetWindowRect(m_hWnd, &winRect)))
    {
      DxsThrowWindows(DxsT("Getting window area failed upon moving"));
    }

    m_stWindowRect.x = winRect.left;
    m_stWindowRect.y = winRect.top;
    m_stWindowRect.w = winRect.right - winRect.left;
    m_stWindowRect.h = winRect.bottom - winRect.top;

    m_stClientRect.x = (long)(short)LOWORD(lParam);
    m_stClientRect.y = (long)(short)HIWORD(lParam);

    OnWindowPositionChangedInternal();
  }
  break;
  case WM_SIZE:
  {
    RECT winRect = { 0 };

    if (DxsFailed(::GetWindowRect(m_hWnd, &winRect)))
    {
      DxsThrowWindows(DxsT("Getting window area failed upon size change"));
    }

    m_stWindowRect.x = winRect.left;
    m_stWindowRect.y = winRect.top;
    m_stWindowRect.w = winRect.right - winRect.left;
    m_stWindowRect.h = winRect.bottom - winRect.top;

    m_stClientRect.w = LOWORD(lParam);
    m_stClientRect.h = HIWORD(lParam);

    OnWindowSizeChangedInternal();
  }
  break;

  //- 
  //- Keyboard Messages
  //- 
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
  {
    KeyCode k = (KeyCode)wParam;
    KeyStatus s = KeyStatus::Pressed;

    m_cKeyboard.SetKeyStatus(k, s);

    if (m_stOptions.isKeyboardDebugEnabled)
      WinDebug::PrintKeyStatus(k, s);
  }
  break;
  
  case WM_KEYUP:
  case WM_SYSKEYUP:
  {
    KeyCode k = (KeyCode)wParam;
    KeyStatus s = KeyStatus::Released;

    m_cKeyboard.SetKeyStatus(k, s);

    if (m_stOptions.isKeyboardDebugEnabled)
      WinDebug::PrintKeyStatus(k, s);
  }
  break;

  case WM_CHAR:
  {
    TChar c = (TChar)wParam;

    m_cKeyboard.InsertChar(c);

    if (m_stOptions.isKeyboardDebugEnabled)
      WinDebug::PrintCharReceived(c);
  }
  break;

  //- 
  //- Mouse Messages
  //- 
  case WM_MOUSEMOVE:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    m_cMouse.SetPosition(pt);

    if (m_cMouse.IsAnyButtonPressed() && !m_cMouse.IsCaptured())
    {
      SetCapture(m_hWnd);
      m_cMouse.SetCaptured(true);
    }
    else if (!m_cMouse.IsAnyButtonPressed() && m_cMouse.IsCaptured())
    {
      ReleaseCapture();
      m_cMouse.SetCaptured(false);
    }

    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientPosition(pt);
  }
  break;

  //- Button Down

  case WM_LBUTTONDOWN:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    m_cMouse.SetButtonStatus(MouseButton::Left, KeyStatus::Pressed, pt);

    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientButtonStatus(MouseButton::Left, KeyStatus::Pressed, pt);
  }
  break;
  case WM_MBUTTONDOWN:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    m_cMouse.SetButtonStatus(MouseButton::Middle, KeyStatus::Pressed, pt);

    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientButtonStatus(MouseButton::Middle, KeyStatus::Pressed, pt);
  }
  break;
  case WM_RBUTTONDOWN:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    m_cMouse.SetButtonStatus(MouseButton::Right, KeyStatus::Pressed, pt);
    
    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientButtonStatus(MouseButton::Right, KeyStatus::Pressed, pt);
  }
  break;

  //- Button Up

  case WM_LBUTTONUP:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    m_cMouse.SetButtonStatus(MouseButton::Left, KeyStatus::Released, pt);
    
    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientButtonStatus(MouseButton::Left, KeyStatus::Released, pt);
  }
  break;
  case WM_MBUTTONUP:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    
    m_cMouse.SetButtonStatus(MouseButton::Middle, KeyStatus::Released, pt);

    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientButtonStatus(MouseButton::Middle, KeyStatus::Released, pt);
  }
  break;
  case WM_RBUTTONUP:
  {
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    m_cMouse.SetButtonStatus(MouseButton::Right, KeyStatus::Released, pt);
    
    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseClientButtonStatus(MouseButton::Right, KeyStatus::Released, pt);
  }
  break;

  //- Scroll Wheel

  case WM_MOUSEWHEEL:
  {
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);
    POINT pt{ 0 };
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);
    ScreenToClient(m_hWnd, &pt);

    m_cMouse.SetScrollStatus(delta, pt);

    if (m_stOptions.isMouseDebugEnabled)
      WinDebug::PrintMouseWheelStatus(delta, pt);
  }
  break;

  } //- switch (msg)

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
//- Window events
//- 
//------------------------------------------------------------------------
void Window::OnWindowPositionChangedInternal()
{
  OnWindowPositionChanged(m_stWindowRect, m_stClientRect);

  //- Invoking the externally provided callback
  WindowCallbackPositionChanged callback = m_stOptions.callbacks.windowCallbackPositionChanged.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_stWindowRect, m_stClientRect);
  }
}
void Window::OnWindowSizeChangedInternal()
{
  OnWindowSizeChanged(m_stWindowRect, m_stClientRect);

  //- Invoking the externally provided callback
  WindowCallbackSizeChanged callback = m_stOptions.callbacks.windowCallbackSizeChanged.load();
  if (callback != nullptr)
  {
    (*callback)(*this, m_stWindowRect, m_stClientRect);
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

  m_pGraphics->StartFrame(); //- Graphics Frame Start

  OnRenderingStateInit();

  //- Invoking the externally provided callback
  RenderingCallbackInit callback = m_stOptions.callbacks.renderingCallbackInit.load();
  if (callback != nullptr)
  {
    (*callback)(*this, *m_pGraphics);
  }

  m_pGraphics->EndFrame(); //- Graphics Frame End
}
void Window::OnRenderingStateRunningInternal(const double deltaSec)
{
  ProcessWindowsMessagesQueue();

  m_pGraphics->StartFrame(); //- Graphics Frame Start

  OnRenderingStateRunning(deltaSec);

  //- Invoking the externally provided callback
  RenderingCallbackRunning callback = m_stOptions.callbacks.renderingCallbackRunning.load();
  if (callback != nullptr)
  {
    (*callback)(*this, *m_pGraphics, m_cKeyboard, m_cMouse, deltaSec);
  }

  m_pGraphics->EndFrame(); //- Graphics Frame End
}
void Window::OnRenderingStatePausedInternal(const double deltaSec)
{
  ProcessWindowsMessagesQueue();

  m_pGraphics->StartFrame(); //- Graphics Frame Start

  OnRenderingStatePaused(deltaSec);

  //- Invoking the externally provided callback
  RenderingCallbackPaused callback = m_stOptions.callbacks.renderingCallbackPaused.load();
  if (callback != nullptr)
  {
    (*callback)(*this, *m_pGraphics, m_cKeyboard, m_cMouse, deltaSec);
  }

  m_pGraphics->EndFrame(); //- Graphics Frame End
}
void Window::OnRenderingStateExittedInternal(ExecutionExitReason reason, const TString& message)
{
  if (m_pGraphics) m_pGraphics->StartFrame(); //- Graphics Frame Start

  OnRenderingStateExitted(reason, message);

  //- Invoking the externally provided callback
  RenderingCallbackExitted callback = m_stOptions.callbacks.renderingCallbackExitted.load();
  if (callback != nullptr) { (*callback)(*this, *m_pGraphics, m_cKeyboard, m_cMouse, reason, message); }

  if (m_pGraphics) m_pGraphics->EndFrame(); //- Graphics Frame End

  //- Exit processing as well
  if (m_pProcessingExecutor) { m_pProcessingExecutor->Exit(); }

  DxsEncloseThrowNoMessage(UnRegisterClassAndDestroyWindow());

  if (GetPrimary()) { ExitProcess(reason == ExecutionExitReason::Exception ? -1 : 0); }
}
void Window::OnRenderingStateChangedInternal(ExecutionState last, ExecutionState next)
{
  if (last == ExecutionState::NONE || next == ExecutionState::Init || !m_pGraphics)
  {
    // Till this time we have not got window created
    return;
  }

  m_pGraphics->StartFrame(); //- Graphics Frame Start

  OnRenderingStateChanged(last, next);

  //- Invoking the externally provided callback
  RenderingCallbackStateChanged callback = m_stOptions.callbacks.renderingCallbackStateChanged.load();
  if (callback != nullptr)
  {
    (*callback)(*this, *m_pGraphics, m_cKeyboard, m_cMouse, last, next);
  }

  m_pGraphics->EndFrame(); //- Graphics Frame End
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



