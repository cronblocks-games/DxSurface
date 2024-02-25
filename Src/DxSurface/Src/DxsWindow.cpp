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

Window::Window(const std::string& title, int x, int y, int width, int height, bool isPrimary, bool debugEnabled)
{
  this->m_sTitle = title;
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
  this->m_sTitle = other.m_sTitle;
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
  this->m_sTitle = std::move(other.m_sTitle);
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
  this->m_pThread = std::move(m_pThread);

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

const std::string& Window::Title() const
{
  return m_sTitle;
}
void Window::Title(const string& title)
{
  if (m_sTitle == title) return;

  if (m_hWnd == nullptr)
  {
    DXSURFACE_THROW("Cannot change title of window with invalid handle");
  }

  SetWindowText(m_hWnd, title.c_str());

  m_sTitle = title;
}

void Window::Show()
{
  if (m_hWnd == nullptr)
  {
    DXSURFACE_THROW("Cannot show window with invalid handle");
  }

  ShowWindow(m_hWnd, SW_SHOW);
}
void Window::Hide()
{
  if (m_hWnd == nullptr)
  {
    DXSURFACE_THROW("Cannot hide window with invalid handle");
  }

  ShowWindow(m_hWnd, SW_HIDE);
}

void Window::PauseRendering()
{
  if (m_eRenderingState == RenderingState::Exitted)
  {
    DXSURFACE_THROW((m_sTitle + " - Cannot pause rendering when it has exitted").c_str());
  }

  m_eRenderingStateCommand = RenderingState::Paused;
}
void Window::ResumeRendering()
{
  if (m_eRenderingState == RenderingState::Exitted)
  {
    DXSURFACE_THROW((m_sTitle + " - Cannot resume rendering when it has already exitted").c_str());
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
static mutex reg_class_mutex;
static map<string, WNDCLASSEX> reg_classes;

HWND Window::RegisterClassAndCreateWindow()
{
  if (m_hWnd != nullptr) return m_hWnd;

  const lock_guard<mutex> lock(reg_class_mutex);

  m_eWindowCreationState = WindowCreationState::Fail;
  DXSURFACE_THROW("Not implemented: RegisterClassAndCreateWindow");
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
    MessageBox(nullptr, ex.what(), "Error - DxSurface", 0);
    threadExitReason = ThreadExitReason::Exception;
  }
  catch (std::exception& ex)
  {
    MessageBox(nullptr, ex.what(), "Error", 0);
    threadExitReason = ThreadExitReason::Exception;
  }
  catch (...)
  {
    MessageBox(nullptr, "Unknown error occurred.", "Error", 0);
    threadExitReason = ThreadExitReason::Exception;
  }

  //- 
  //- Exiting
  //- 

  lastRenderingState = w->m_eRenderingState;
  DXSURFACE_ENCLOSE_THROW(w->OnRenderingStateExitingInternal());
  DXSURFACE_ENCLOSE_THROW(w->OnRenderingStateExiting());

  w->m_eRenderingState = RenderingState::Exitted;
  DXSURFACE_ENCLOSE_THROW(w->OnRenderingStateChangedInternal(lastRenderingState, RenderingState::Exitted));
  DXSURFACE_ENCLOSE_THROW(w->OnRenderingStateChanged(lastRenderingState, RenderingState::Exitted));

  if (threadExitReason != ThreadExitReason::Exception)
  {
    DXSURFACE_ENCLOSE_THROW(w->Hide());
  }

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
