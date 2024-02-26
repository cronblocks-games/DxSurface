#include "pch.h"
#include "DxsWindow.h"


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
  m_eRenderingState = RenderingState::NONE;
  m_eRenderingStateCommand = RenderingState::NONE;
  m_pThread = make_shared<thread>(Window::RenderingThread, this);

  while (m_eWindowCreationState == WindowCreationState::NONE &&
         m_eRenderingState != RenderingState::Exitted);
}
Window::Window(const Window& other)
{
  *this = other;
}
Window::Window(Window&& other) noexcept
{
  *this = std::move(other);
}
Window::~Window()
{
  m_eRenderingStateCommand = RenderingState::Exitted;

  if (m_pThread != nullptr)
  {
    m_pThread->join();
  }
}

Window& Window::operator=(const Window& other)
{
  m_hInstance = other.m_hInstance;
  m_hWnd = nullptr;

  m_sClassName = DxsT("");
  m_stOptions = other.m_stOptions;

  m_eWindowCreationState = WindowCreationState::NONE;
  m_eRenderingState = other.m_eRenderingState;
  m_eRenderingStateCommand = other.m_eRenderingStateCommand;
  m_pThread = make_shared<thread>(Window::RenderingThread, this);

  while (m_eWindowCreationState == WindowCreationState::NONE &&
         m_eRenderingState != RenderingState::Exitted);

  return *this;
}
Window& Window::operator=(Window&& other) noexcept
{
  m_hInstance = other.m_hInstance;
  m_hWnd = other.m_hWnd;

  m_sClassName = std::move(other.m_sClassName);
  m_stOptions = std::move(other.m_stOptions);

  m_eWindowCreationState = other.m_eWindowCreationState;
  m_eRenderingState = other.m_eRenderingState;
  m_eRenderingStateCommand = other.m_eRenderingStateCommand;
  m_pThread = other.m_pThread;

  other.m_hInstance = nullptr;
  other.m_hWnd = nullptr;

  other.m_eWindowCreationState = WindowCreationState::NONE;
  other.m_eRenderingState = RenderingState::NONE;
  other.m_eRenderingStateCommand = RenderingState::NONE;
  other.m_pThread = nullptr;

  return *this;
}

bool Window::Primary() const
{
  return m_stOptions.isPrimary;
}
void Window::Primary(bool isPrimary)
{
  m_stOptions.isPrimary = isPrimary;
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

void Window::PauseRendering()
{
  if (m_eRenderingState == RenderingState::Exitted)
  {
    DxsThrow((Title() + DxsT(" - Cannot pause rendering when it has exitted")).c_str());
  }

  m_eRenderingStateCommand = RenderingState::Paused;
}
void Window::ResumeRendering()
{
  if (m_eRenderingState == RenderingState::Exitted)
  {
    DxsThrow((Title() + DxsT(" - Cannot resume rendering when it has already exitted")).c_str());
  }

  m_eRenderingStateCommand = RenderingState::Running;
}
void Window::ExitRendering()
{
  m_eRenderingStateCommand = RenderingState::Exitted;
}
RenderingState Window::RenderingState() const
{
  return m_eRenderingState;
}
void Window::WaitForExit() const
{
  if (m_pThread != nullptr)
  {
    m_pThread->join();
  }
}



//------------------------------------------------------------------------
//- 
//- Window class registration and creation
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
    MutexLock lock(_mutGlobalData);

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
  wndcls.hIcon = nullptr;
  wndcls.hCursor = nullptr;
  wndcls.hbrBackground = nullptr;
  wndcls.lpszMenuName = nullptr;
  wndcls.lpszClassName = m_sClassName.c_str();
  wndcls.hIconSm = nullptr;
  RegisterClassEx(&wndcls);

  m_hWnd = CreateWindowEx(
    0,                                      // dwExStyle
    m_sClassName.c_str(),                   // lpClassName
    Title().c_str(),                        // lpWindowName
    0,                                      // dwStyle
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
//- Universal messages handler
//- 
LRESULT WINAPI Window::WindowsMessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  try {
    if (_handle2WindowMap.contains(hWnd))
      return _handle2WindowMap[hWnd]->OnWindowsMessage(msg, wParam, lParam);
  } catch (...) {}

  return DefWindowProc(hWnd, msg, wParam, lParam);
}
//- 
//- Window specific message handler
//- 
LRESULT Window::OnWindowsMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
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
void Window::OnRenderingStateInit() {}
void Window::OnRenderingStateRunning() {}
void Window::OnRenderingStateExiting() {}
void Window::OnRenderingStateChanged(enum class RenderingState last, enum class RenderingState next) {}

void Window::RenderingThreadInit()
{
  RegisterClassAndCreateWindow();
  Show();

  //- Calling Init when doing default construction
  if (m_eRenderingState == m_eRenderingStateCommand &&
      m_eRenderingStateCommand == RenderingState::NONE)
  {
    m_eRenderingState = RenderingState::Init;

    OnRenderingStateInitInternal();
    OnRenderingStateInit();

    OnRenderingStateChangedInternal(RenderingState::NONE, RenderingState::Init);
    OnRenderingStateChanged(RenderingState::NONE, RenderingState::Init);
  }
}
void Window::RenderingThreadExit(enum class ThreadExitReason reason)
{
  auto lastRenderingState = m_eRenderingState;

  DxsEncloseThrow(OnRenderingStateExitingInternal());
  DxsEncloseThrow(OnRenderingStateExiting());

  m_eRenderingState = RenderingState::Exitted;

  DxsEncloseThrow(OnRenderingStateChangedInternal(lastRenderingState, RenderingState::Exitted));
  DxsEncloseThrow(OnRenderingStateChanged(lastRenderingState, RenderingState::Exitted));

  DxsEncloseThrow(UnRegisterClassAndDestroyWindow());

  if (Primary())
  {
    ExitProcess(reason == ThreadExitReason::Exception ? -1 : 0);
  }
}
void Window::RenderingThreadSetRenderingState(enum class RenderingState newState)
{
  if (m_eRenderingState != newState)
  {
    auto last = m_eRenderingState;
    m_eRenderingState = newState;

    OnRenderingStateChangedInternal(last, newState);
    OnRenderingStateChanged(last, newState);
  }
}
void Window::RenderingThreadProcessRenderingState()
{
  switch (m_eRenderingState)
  {
  case RenderingState::NONE:
  case RenderingState::Init:
    //- we shouldn't be here - will assert later
    break;

  case RenderingState::Running:
    OnRenderingStateRunningInternal();
    OnRenderingStateRunning();
    break;

  case RenderingState::Paused:
    break;
  case RenderingState::Exitted:
    break;
  }
}
void Window::RenderingThread(Window* w)
{
  ThreadExitReason threadExitReason = ThreadExitReason::Normal;

  try
  {
    w->RenderingThreadInit();

    if (w->m_stOptions.maxRefreshRateHz != 0)
    {
      const double timePerIterationMSec = 1000.0 / w->m_stOptions.maxRefreshRateHz;
      TimePoint startTime;
      TimeDurationMilli timeDuration;

      while (w->m_eRenderingStateCommand != RenderingState::Exitted)
      {
        startTime = Clock::now();

        {
          //- Let's see if there are any messages available from Windows?
          w->ProcessWindowsMessagesQueue();

          //- If there are any state changes commanded?
          switch (w->m_eRenderingStateCommand)
          {
          case RenderingState::NONE:
          case RenderingState::Init:
          case RenderingState::Running:
            w->RenderingThreadSetRenderingState(RenderingState::Running);
            break;
          case RenderingState::Paused:
            w->RenderingThreadSetRenderingState(RenderingState::Paused);
            break;
          }

          //- What to do within a rendering state?
          w->RenderingThreadProcessRenderingState();
        }
        
        timeDuration = Clock::now() - startTime;

        if (w->m_eRenderingStateCommand == RenderingState::Exitted)
        {
          break;
        }

        if (timeDuration.count() < timePerIterationMSec)
        {
          double sleepTime = timePerIterationMSec - timeDuration.count();
          this_thread::sleep_for(TimeDurationMilli(sleepTime));
        }
      }
    }
    else
    {
      DxsThrow((w->Title() + DxsT(" - Zero (Hz) refresh rate is invalid")).c_str());
    }
  }
  catch (Exception& ex)
  {
    MessageBox(nullptr, ex.Message().c_str(), DxsT("Error - DxSurface"), 0);
    threadExitReason = ThreadExitReason::Exception;
  }
  catch (std::exception& ex)
  {
    MessageBoxA(nullptr, ex.what(), "Error", 0);
    threadExitReason = ThreadExitReason::Exception;
  }
  catch (...)
  {
    MessageBox(nullptr, DxsT("Unknown error occurred."), DxsT("Error"), 0);
    threadExitReason = ThreadExitReason::Exception;
  }

  w->RenderingThreadExit(threadExitReason);
}

void Window::OnRenderingStateInitInternal()
{
  //-
}
void Window::OnRenderingStateRunningInternal()
{
  //-
}
void Window::OnRenderingStateExitingInternal()
{
  //-
}
void Window::OnRenderingStateChangedInternal(
  enum class RenderingState last,
  enum class RenderingState next)
{
  //-
}
