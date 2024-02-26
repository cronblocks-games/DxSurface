#include "pch.h"
#include "DxsWindow.h"


using namespace std;
using namespace CB::DxSurface;


Window::~Window()
{
  m_eRenderingStateCommand = RenderingState::Exitted;

  if (m_pThread != nullptr)
  {
    m_pThread.get()->join();
  }
}

Window::Window(HINSTANCE hInstance, const TString& title, int x, int y, int width, int height, bool isPrimary, bool debugEnabled)
{
  if (hInstance == nullptr)
  {
    DxsThrow(DxsT("Invalid HINSTANCE provided"));
  }
  this->m_hInstance = hInstance;
  this->m_sTitle = title;
  this->m_sClassName = DxsT("");
  this->m_iX = x;
  this->m_iY = y;
  this->m_iWidth = width;
  this->m_iHeight = height;
  this->m_bIsPrimary = isPrimary;
  this->m_bIsDebugEnabled = debugEnabled;

  this->m_hWnd = nullptr;

  this->m_eWindowCreationState = WindowCreationState::NONE;
  this->m_eRenderingState = RenderingState::NONE;
  this->m_eRenderingStateCommand = RenderingState::NONE;
  this->m_pThread = make_unique<thread>(Window::RenderingThread, this);

  while (this->m_eWindowCreationState == WindowCreationState::NONE &&
         this->m_eRenderingState != RenderingState::Exitted);
}
Window::Window(const Window& other)
{
  *this = other;
}
Window::Window(Window&& other) noexcept
{
  *this = std::move(other);
}

Window& Window::operator=(const Window& other)
{
  this->m_hInstance = other.m_hInstance;
  this->m_sTitle = other.m_sTitle;
  this->m_sClassName = DxsT("");
  this->m_iX = other.m_iX;
  this->m_iY = other.m_iY;
  this->m_iWidth = other.m_iWidth;
  this->m_iHeight = other.m_iHeight;
  this->m_bIsPrimary = other.m_bIsPrimary;
  this->m_bIsDebugEnabled = other.m_bIsDebugEnabled;

  this->m_hWnd = nullptr;

  this->m_eWindowCreationState = WindowCreationState::NONE;
  this->m_eRenderingState = other.m_eRenderingState;
  this->m_eRenderingStateCommand = other.m_eRenderingStateCommand;
  this->m_pThread = make_unique<thread>(Window::RenderingThread, this);

  while (this->m_eWindowCreationState == WindowCreationState::NONE &&
         this->m_eRenderingState != RenderingState::Exitted);

  return *this;
}
Window& Window::operator=(Window&& other) noexcept
{
  this->m_hInstance = other.m_hInstance;
  this->m_sTitle = std::move(other.m_sTitle);
  this->m_sClassName = std::move(other.m_sClassName);
  this->m_iX = other.m_iX;
  this->m_iY = other.m_iY;
  this->m_iWidth = other.m_iWidth;
  this->m_iHeight = other.m_iHeight;
  this->m_bIsPrimary = other.m_bIsPrimary;
  this->m_bIsDebugEnabled = other.m_bIsDebugEnabled;

  this->m_hWnd = other.m_hWnd;

  this->m_eWindowCreationState = other.m_eWindowCreationState;
  this->m_eRenderingState = other.m_eRenderingState;
  this->m_eRenderingStateCommand = other.m_eRenderingStateCommand;
  this->m_pThread = std::move(other.m_pThread);

  other.m_hInstance = nullptr;
  other.m_iX = 0;
  other.m_iY = 0;
  other.m_iWidth = 0;
  other.m_iHeight = 0;
  other.m_bIsPrimary = false;
  other.m_bIsDebugEnabled = false;

  other.m_hWnd = nullptr;

  other.m_eWindowCreationState = WindowCreationState::NONE;
  other.m_eRenderingState = RenderingState::NONE;
  other.m_eRenderingStateCommand = RenderingState::NONE;
  other.m_pThread = nullptr;

  return *this;
}

bool Window::Primary() const
{
  return m_bIsPrimary;
}
void Window::Primary(bool isPrimary)
{
  m_bIsPrimary = isPrimary;
}

const TString& Window::Title() const
{
  return m_sTitle;
}
void Window::Title(const TString& title)
{
  if (m_sTitle == title) return;

  if (m_hWnd == nullptr)
  {
    DxsThrow(DxsT("Cannot change title of window with invalid handle"));
  }

  SetWindowText(m_hWnd, title.c_str());

  m_sTitle = title;
}

void Window::Show()
{
  if (m_hWnd == nullptr)
  {
    DxsThrow(DxsT("Cannot show window with invalid handle"));
  }

  ShowWindow(m_hWnd, SW_SHOW);
}
void Window::Hide()
{
  if (m_hWnd == nullptr)
  {
    DxsThrow(DxsT("Cannot hide window with invalid handle"));
  }

  ShowWindow(m_hWnd, SW_HIDE);
}

void Window::PauseRendering()
{
  if (m_eRenderingState == RenderingState::Exitted)
  {
    DxsThrow((m_sTitle + DxsT(" - Cannot pause rendering when it has exitted")).c_str());
  }

  m_eRenderingStateCommand = RenderingState::Paused;
}
void Window::ResumeRendering()
{
  if (m_eRenderingState == RenderingState::Exitted)
  {
    DxsThrow((m_sTitle + DxsT(" - Cannot resume rendering when it has already exitted")).c_str());
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
  m_pThread.get()->join();
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
    0,                        // dwExStyle
    m_sClassName.c_str(),     // lpClassName
    m_sTitle.c_str(),         // lpWindowName
    0,                        // dwStyle
    m_iX, m_iY, m_iWidth, m_iHeight,
    nullptr,                  // hWndParent
    nullptr,                  // hMenu
    m_hInstance,              // hInstance
    nullptr                   // lpParam
  );

  if (m_hWnd != nullptr)
  {
    m_eWindowCreationState = WindowCreationState::Success;

    MutexLock lock(_mutGlobalData);
    _handle2WindowMap[m_hWnd] = this;
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


//------------------------------------------------------------------------
//- 
//- Rendering thread
//- 
//------------------------------------------------------------------------
void Window::OnRenderingStateInit() {}
void Window::OnRenderingStateRunning() {}
void Window::OnRenderingStateExiting() {}
void Window::OnRenderingStateChanged(enum class RenderingState last, enum class RenderingState next) {}

void Window::RenderingThread(Window* w)
{
  enum class RenderingState lastRenderingState = RenderingState::NONE;
  enum class ThreadExitReason threadExitReason = ThreadExitReason::Normal;

  try
  {
    w->RegisterClassAndCreateWindow();
    w->Show();

    //- Calling Init when doing default construction
    if (w->m_eRenderingState == w->m_eRenderingStateCommand &&
      w->m_eRenderingStateCommand == RenderingState::NONE)
    {
      w->m_eRenderingState = RenderingState::Init;

      w->OnRenderingStateInitInternal();
      w->OnRenderingStateInit();

      w->OnRenderingStateChangedInternal(RenderingState::NONE, RenderingState::Init);
      w->OnRenderingStateChanged(RenderingState::NONE, RenderingState::Init);
    }

    while (w->m_eRenderingStateCommand != RenderingState::Exitted)
    {
      //- 
      //- If there are any state changes commanded?
      //- 

      switch (w->m_eRenderingStateCommand)
      {
      case RenderingState::NONE:
      case RenderingState::Init:
      case RenderingState::Running:

        if (w->m_eRenderingState != RenderingState::Running)
        {
          lastRenderingState = w->m_eRenderingState;
          w->m_eRenderingState = RenderingState::Running;

          w->OnRenderingStateChangedInternal(lastRenderingState, RenderingState::Running);
          w->OnRenderingStateChanged(lastRenderingState, RenderingState::Running);
        }

        break;

      case RenderingState::Paused:

        if (w->m_eRenderingState != RenderingState::Paused)
        {
          lastRenderingState = w->m_eRenderingState;
          w->m_eRenderingState = RenderingState::Paused;

          w->OnRenderingStateChangedInternal(lastRenderingState, RenderingState::Paused);
          w->OnRenderingStateChanged(lastRenderingState, RenderingState::Paused);
        }

        break;

      case RenderingState::Exitted:
        break;
      }

      //- 
      //- What to do within a rendering state?
      //- 

      switch (w->m_eRenderingState)
      {
      case RenderingState::NONE:
      case RenderingState::Init:
        //- we shouldn't be here - will assert later
        break;

      case RenderingState::Running:
        w->OnRenderingStateRunningInternal();
        w->OnRenderingStateRunning();
        break;

      case RenderingState::Paused:
        break;
      case RenderingState::Exitted:
        break;
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

  //- 
  //- Exiting
  //- 

  lastRenderingState = w->m_eRenderingState;
  DxsEncloseThrow(w->OnRenderingStateExitingInternal());
  DxsEncloseThrow(w->OnRenderingStateExiting());

  w->m_eRenderingState = RenderingState::Exitted;
  DxsEncloseThrow(w->OnRenderingStateChangedInternal(lastRenderingState, RenderingState::Exitted));
  DxsEncloseThrow(w->OnRenderingStateChanged(lastRenderingState, RenderingState::Exitted));

  DxsEncloseThrow(w->UnRegisterClassAndDestroyWindow());

  if (w->Primary())
  {
    ExitProcess(threadExitReason == ThreadExitReason::Exception ? -1 : 0);
  }
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
