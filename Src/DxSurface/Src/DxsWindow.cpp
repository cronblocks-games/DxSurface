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
  m_eRenderingState = ExecutionState::NONE;
  m_eProcessingState = ExecutionState::NONE;
  m_eThreadsStateCommand = ExecutionState::NONE;
  m_pRenderingThread.reset();
  m_pProcessingThread.reset();
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
  m_eThreadsStateCommand = ExecutionState::Exitted;

  if (m_pProcessingThread)
  {
    m_pProcessingThread->join();
    m_pProcessingThread.reset();
  }
  if (m_pRenderingThread)
  {
    m_pRenderingThread->join();
    m_pRenderingThread.reset();
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
  m_eRenderingState = ExecutionState::NONE;
  m_eProcessingState = ExecutionState::NONE;
  m_eThreadsStateCommand = ExecutionState::NONE;
  m_pRenderingThread.reset();
  m_pProcessingThread.reset();

  return *this;
}
Window& Window::operator=(Window&& other) noexcept
{
  if (this == &other) return *this;

  m_hInstance = other.m_hInstance;
  m_hWnd = other.m_hWnd;

  m_sClassName = std::move(other.m_sClassName);
  m_stOptions = std::move(other.m_stOptions);

  m_eWindowCreationState = other.m_eWindowCreationState;
  m_eRenderingState = other.m_eRenderingState;
  m_eProcessingState = other.m_eProcessingState;
  m_eThreadsStateCommand = other.m_eThreadsStateCommand;
  m_pRenderingThread = other.m_pRenderingThread;
  m_pProcessingThread = other.m_pProcessingThread;

  other.m_hInstance = nullptr;
  other.m_hWnd = nullptr;

  other.m_eWindowCreationState = WindowCreationState::NONE;
  other.m_eRenderingState = ExecutionState::NONE;
  other.m_eProcessingState = ExecutionState::NONE;
  other.m_eThreadsStateCommand = ExecutionState::NONE;
  other.m_pRenderingThread.reset();
  other.m_pProcessingThread.reset();

  return *this;
}

static Mutex _windowCreationMutex;

void Window::CreateWindowAndRun()
{
  Lock l(_windowCreationMutex);

  if (m_pRenderingThread) return;

  m_eWindowCreationState = WindowCreationState::NONE;
  m_eRenderingState = ExecutionState::NONE;
  m_eProcessingState = ExecutionState::NONE;
  m_eThreadsStateCommand = ExecutionState::NONE;
  m_pRenderingThread = make_shared<thread>(Window::RenderingThread, this);
  m_pProcessingThread = make_shared<thread>(Window::ProcessingThread, this);

  while (m_eWindowCreationState == WindowCreationState::NONE &&
         m_eRenderingState != ExecutionState::Exitted);
}
ExecutionState Window::RenderingState() const
{
  return m_eRenderingState;
}
void Window::Pause()
{
  if (m_eRenderingState == ExecutionState::Exitted)
  {
    DxsThrow((Title() + DxsT(" - Cannot pause rendering when it has exitted")).c_str());
  }

  m_eThreadsStateCommand = ExecutionState::Paused;
}
void Window::Resume()
{
  if (m_eRenderingState == ExecutionState::Exitted)
  {
    DxsThrow((Title() + DxsT(" - Cannot resume rendering when it has already exitted")).c_str());
  }

  m_eThreadsStateCommand = ExecutionState::Running;
}
void Window::Exit()
{
  m_eThreadsStateCommand = ExecutionState::Exitted;
}
void Window::WaitForExit()
{
  if (m_pProcessingThread)
  {
    m_pProcessingThread->join();
    m_pProcessingThread.reset();
  }
  if (m_pRenderingThread)
  {
    m_pRenderingThread->join();
    m_pRenderingThread.reset();
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
//- Universal messages handler
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
//- Window specific message handler
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
void Window::OnRenderingStateInit() {}
void Window::OnRenderingStateRunning() {}
void Window::OnRenderingStateExiting() {}
void Window::OnRenderingStateChanged(ExecutionState last, ExecutionState next) {}

void Window::RenderingThreadDoInit()
{
  RegisterClassAndCreateWindow();
  Show();

  //- Calling Init when doing default construction
  if (m_eRenderingState == m_eThreadsStateCommand &&
      m_eThreadsStateCommand == ExecutionState::NONE)
  {
    m_eRenderingState = ExecutionState::Init;

    OnRenderingStateInitInternal();
    OnRenderingStateInit();

    OnRenderingStateChangedInternal(ExecutionState::NONE, ExecutionState::Init);
    OnRenderingStateChanged(ExecutionState::NONE, ExecutionState::Init);
  }
}
void Window::RenderingThreadDoExit(enum class ThreadExitReason reason)
{
  auto lastRenderingState = m_eRenderingState;

  DxsEncloseThrow(OnRenderingStateExitingInternal());
  DxsEncloseThrow(OnRenderingStateExiting());

  m_eRenderingState = ExecutionState::Exitted;

  DxsEncloseThrow(OnRenderingStateChangedInternal(lastRenderingState, ExecutionState::Exitted));
  DxsEncloseThrow(OnRenderingStateChanged(lastRenderingState, ExecutionState::Exitted));

  DxsEncloseThrow(UnRegisterClassAndDestroyWindow());

  if (Primary())
  {
    ExitProcess(reason == ThreadExitReason::Exception ? -1 : 0);
  }
}
void Window::RenderingThreadDoSetRenderingState(ExecutionState newState)
{
  if (m_eRenderingState != newState)
  {
    auto last = m_eRenderingState;
    m_eRenderingState = newState;

    OnRenderingStateChangedInternal(last, newState);
    OnRenderingStateChanged(last, newState);
  }
}
void Window::RenderingThreadDoProcessRenderingState()
{
  switch (m_eRenderingState)
  {
  case ExecutionState::NONE:
  case ExecutionState::Init:
    //- we shouldn't be here - will assert later
    break;

  case ExecutionState::Running:
    OnRenderingStateRunningInternal();
    OnRenderingStateRunning();
    break;

  case ExecutionState::Paused:
    break;
  case ExecutionState::Exitted:
    break;
  }
}
void Window::RenderingThread(Window* const w)
{
  ThreadExitReason threadExitReason = ThreadExitReason::Normal;

  try
  {
    w->RenderingThreadDoInit();

    if (w->m_stOptions.maxRenderingThreadRefreshRateHz != 0)
    {
      const double timePerIterationMSec = 1000.0 / w->m_stOptions.maxRenderingThreadRefreshRateHz;
      TimePoint startTime;
      TimeDurationMilli timeDuration;

      while (w->m_eThreadsStateCommand != ExecutionState::Exitted)
      {
        startTime = Clock::now();

        {
          //- Let's see if there are any messages available from Windows?
          w->ProcessWindowsMessagesQueue();

          //- If there are any state changes commanded?
          switch (w->m_eThreadsStateCommand)
          {
          case ExecutionState::NONE:
          case ExecutionState::Init:
          case ExecutionState::Running:
            w->RenderingThreadDoSetRenderingState(ExecutionState::Running);
            break;
          case ExecutionState::Paused:
            w->RenderingThreadDoSetRenderingState(ExecutionState::Paused);
            break;
          }

          //- What to do within a rendering state?
          w->RenderingThreadDoProcessRenderingState();
        }
        
        timeDuration = Clock::now() - startTime;

        if (w->m_eThreadsStateCommand == ExecutionState::Exitted)
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

  w->RenderingThreadDoExit(threadExitReason);
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
void Window::OnRenderingStateChangedInternal(ExecutionState last, ExecutionState next)
{
  //-
}


//------------------------------------------------------------------------
//- 
//- Processing thread
//- 
//------------------------------------------------------------------------
void Window::ProcessingThread(Window* const w)
{
  ThreadExitReason threadExitReason = ThreadExitReason::Normal;

  try
  {
    w->m_eProcessingState = ExecutionState::Init;

    if (w->m_stOptions.processingFunc != nullptr)
    {
      if (w->m_stOptions.maxProcessingThreadRefreshRateHz != 0)
      {
        const double timePerIterationMSec = 1000.0 / w->m_stOptions.maxProcessingThreadRefreshRateHz;

        TimePoint iterationStartTime, lastExecutionTime = Clock::now();

        while (w->m_eThreadsStateCommand != ExecutionState::Exitted &&
               w->m_eRenderingState != ExecutionState::Exitted)
        {
          iterationStartTime = Clock::now();

          //- Run the process and take the timestamp
          if (w->m_eThreadsStateCommand == ExecutionState::NONE ||
              w->m_eThreadsStateCommand == ExecutionState::Init ||
              w->m_eThreadsStateCommand == ExecutionState::Running)
          {
            w->m_eProcessingState = ExecutionState::Running;
            w->m_stOptions.processingFunc(TimeDurationMilli(Clock::now() - lastExecutionTime).count());
          }
          else if (w->m_eThreadsStateCommand == ExecutionState::Paused)
          {
            w->m_eProcessingState = ExecutionState::Paused;
          }

          lastExecutionTime = Clock::now();

          //- Exit if commanded, before going to sleep
          if (w->m_eThreadsStateCommand == ExecutionState::Exitted) { break; }

          //- Sleep if needed
          double timeTakenInIteration = TimeDurationMilli(Clock::now() - iterationStartTime).count();
          if (timeTakenInIteration < timePerIterationMSec)
          {
            this_thread::sleep_for(TimeDurationMilli(timePerIterationMSec - timeTakenInIteration));
          }
        }
      }
      else
      {
        DxsThrow((w->Title() + DxsT(" - Zero (Hz) processing refresh rate is invalid")).c_str());
      }
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

  w->m_eProcessingState = ExecutionState::Exitted;
}