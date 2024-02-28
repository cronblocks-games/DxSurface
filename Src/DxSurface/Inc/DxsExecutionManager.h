#ifndef _DXSURFACE_DXSEXECUTIONMANAGER_H_
#define _DXSURFACE_DXSEXECUTIONMANAGER_H_



#include "DxsHelpers.hpp"
#include "DxsExceptions.hpp"


namespace CB::DxSurface {

  template<class T>
  class ExecutableObjectBase {
  public:
    ExecutableObjectBase(TString name, unsigned int maxRefreshRateHz)
    {
      m_sName = name;
      m_uiMaxRefreshRateHz = maxRefreshRateHz;
      m_pThread.reset();
      m_eState = ExecutionState::NONE;
      m_eCommand = ExecutionCommand::Exit;
      m_eExitReason = ExecutionExitReason::Normal;
      m_eExitMessage = DxsT("");
    }
    ExecutableObjectBase(const ExecutableObjectBase& o)
    {
      if (this == &o) return;
      *this = o;
    }
    ExecutableObjectBase(ExecutableObjectBase&& o) noexcept
    {
      if (this == &o) return;
      *this = std::move(o);
    }
    ExecutableObjectBase& operator=(const ExecutableObjectBase& o)
    {
      if (this == &o) return *this;
      
      m_sName = o.name;
      m_uiMaxRefreshRateHz = o.maxRefreshRateHz;
      m_pThread.reset(); // Thread cannot be copied, each object has its own thread
      m_eState = ExecutionState::NONE;
      m_eCommand = ExecutionCommand::Exit;
      m_eExitReason = ExecutionExitReason::Normal;
      m_eExitMessage = DxsT("");
    }
    ExecutableObjectBase& operator=(ExecutableObjectBase&& o) noexcept
    {
      if (this == &o) return *this;
      //- TODO
    }

    virtual ~ExecutableObjectBase()
    {
      m_eCommand = ExecutionCommand::Exit;
      if (m_pThread) m_pThread->join();
      m_pThread.reset();
    }

    void operator()()
    {
      Run();
    }
    void Run()
    {
      if (m_pThread) return;
      m_eCommand = ExecutionCommand::Run;
      m_pThread = std::make_shared<Thread>(ExecutableObjectBase::ExecutionThread, this);
    }
    ExecutionState GetState() const
    {
      return m_eState;
    }
    void Pause()
    {
      if (m_eState == ExecutionState::Exitted)
      {
        DxsThrow((m_sName + DxsT(" - cannot pause execution when it has already exitted")).c_str());
      }

      m_eCommand = ExecutionCommand::Pause;
    }
    void Resume()
    {
      if (m_eState == ExecutionState::Exitted)
      {
        DxsThrow((m_sName + DxsT(" - cannot resume execution when it has already exitted")).c_str());
      }

      m_eCommand = ExecutionCommand::Run;
    }
    void Exit()
    {
      m_eCommand = ExecutionCommand::Exit;
    }
    ExecutionExitReason GetExitReason() const
    {
      return m_eExitReason;
    }
    const TString& GetExitMessage() const
    {
      return m_eExitMessage;
    }
    void WaitForExit()
    {
      if (m_pThread)
      {
        m_pThread->join();
        m_pThread.reset();
      }
    }

  protected:
    virtual void OnExecutionStateInit() = 0;
    virtual void OnExecutionStateRunning(double deltaSec) = 0;
    virtual void OnExecutionStateExiting() = 0;
    virtual void OnExecutionStateChanged(ExecutionState last, ExecutionState next) = 0;

  private:
    TString m_sName;
    unsigned int m_uiMaxRefreshRateHz;
    Ptr<Thread> m_pThread;
    volatile ExecutionState m_eState;
    volatile ExecutionCommand m_eCommand;
    volatile ExecutionExitReason m_eExitReason;
    TString m_eExitMessage;

    void SetExecutionState(ExecutionState s)
    {
      if (m_eState != s)
      {
        OnExecutionStateChanged(m_eState, s)
        m_eState = s;
      }
    }
    void ProcessExecutionState(double deltaSec)
    {
      //
    }

    static void ExecutionThread(ExecutableObjectBase* const b)
    {
      b->m_eExitReason = ExecutionExitReason::Normal;
      b->m_eExitMessage = DxsT("");

      try
      {
        b->SetExecutionState(ExecutionState::Init);
        b->ProcessExecutionState(0.0);

        while (b->m_eCommand != ExecutionCommand::Exit)
        {
          //- Setting the state if changed
          switch (b->m_eCommand)
          {
          case ExecutionCommand::Run:
            b->SetExecutionState(ExecutionState::Running);
            break;
          case ExecutionCommand::Pause:
            b->SetExecutionState(ExecutionState::Paused);
            break;
          }
          //- Processing the state as needed
          b->ProcessExecutionState(0.0);
        }
      }
      catch (const Exception& ex)
      {
        b->m_eExitReason = ExecutionExitReason::Exception;
        b->m_eExitMessage = ex.Message();
      }
      catch (...)
      {
        b->m_eExitReason = ExecutionExitReason::Exception;
        b->m_eExitMessage = DxsT("Unknown error occurred");
      }

      b->SetExecutionState(ExecutionState::Exitted);
      b->ProcessExecutionState(0.0);
    }
  };

} //- namespace CB::DxSurface



#endif
