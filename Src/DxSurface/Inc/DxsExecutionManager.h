#ifndef _DXSURFACE_DXSEXECUTIONMANAGER_H_
#define _DXSURFACE_DXSEXECUTIONMANAGER_H_



#include "DxsHelpers.hpp"
#include "DxsExceptions.hpp"


namespace CB::DxSurface {

  template<class T>
  class ExecutionManagerBase {
  public:
    ExecutionManagerBase(TString name, unsigned int maxRefreshRateHz)
    {
      m_sName = name;
      m_uiMaxRefreshRateHz = maxRefreshRateHz;
    }
    ExecutionManagerBase(const ExecutionManagerBase& o)
    {
      if (this == &o) return;
      *this = o;
    }
    ExecutionManagerBase(ExecutionManagerBase&& o) noexcept
    {
      if (this == &o) return;
      *this = std::move(o);
    }

    ExecutionManagerBase& operator=(const ExecutionManagerBase& o)
    {
      if (this == &o) return *this;
      //- TODO
    }
    ExecutionManagerBase& operator=(ExecutionManagerBase&& o) noexcept
    {
      if (this == &o) return *this;
      //- TODO
    }

    virtual ~ExecutionManagerBase()
    {
      //- TODO
    }

    void Run();
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
    virtual void OnExecutionStateRunning() = 0;
    virtual void OnExecutionStateExiting() = 0;
    virtual void OnExecutionStateChanged(ExecutionState last, ExecutionState next) = 0;

  private:
    TString m_sName;
    unsigned int m_uiMaxRefreshRateHz;
    Ptr<Thread> m_pThread;
    volatile ExecutionState m_eState;
    volatile ExecutionCommand m_eCommand;

    static void ExecutionThread()
    {
      while (m_eCommand != ExecutionCommand::Exit)
      {
        ;
      }
    }
  };

} //- namespace CB::DxSurface



#endif
