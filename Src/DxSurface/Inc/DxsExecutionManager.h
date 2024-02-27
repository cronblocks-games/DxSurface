#ifndef _DXSURFACE_DXSEXECUTIONMANAGER_H_
#define _DXSURFACE_DXSEXECUTIONMANAGER_H_



#include "DxsHelpers.hpp"
#include "DxsExceptions.hpp"


namespace CB::DxSurface {

  template<class T>
  class ExecutionManager {
  public:
    ExecutionManager(TString name, unsigned int maxRefreshRateHz)
    {
      m_sName = name;
      m_uiMaxRefreshRateHz = maxRefreshRateHz;
    }
    ExecutionManager(const ExecutionManager& o)
    {
      if (this == &o) return;
      *this = o;
    }
    ExecutionManager(ExecutionManager&& o) noexcept
    {
      if (this == &o) return;
      *this = std::move(o);
    }

    ExecutionManager& operator=(const ExecutionManager& o)
    {
      if (this == &o) return *this;
      //- TODO
    }
    ExecutionManager& operator=(ExecutionManager&& o) noexcept
    {
      if (this == &o) return *this;
      //- TODO
    }

    virtual ~ExecutionManager()
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
        DxsThrow((m_sName + DxsT(" - ExecutionManager - cannot pause execution when it has already exitted")).c_str());
      }

      m_eCommand = ExecutionCommand::Pause;
    }
    void Resume()
    {
      if (m_eState == ExecutionState::Exitted)
      {
        DxsThrow((m_sName + DxsT(" - ExecutionManager - cannot resume execution when it has already exitted")).c_str());
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
    virtual void OnExecutionManagerStateInit() {}
    virtual void OnExecutionManagerStateRunning() {}
    virtual void OnExecutionManagerStateExiting() {}
    virtual void OnExecutionManagerStateChanged(ExecutionState last, ExecutionState next) {}

  private:
    TString m_sName;
    unsigned int m_uiMaxRefreshRateHz;
    Ptr<Thread> m_pThread;
    volatile ExecutionState m_eState;
    volatile ExecutionCommand m_eCommand;

    static void ExecutionThread();
  };

} //- namespace CB::DxSurface



#endif
