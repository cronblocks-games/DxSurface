#ifndef _DXSURFACE_DXSEXECUTIONMANAGER_H_
#define _DXSURFACE_DXSEXECUTIONMANAGER_H_



#include "DxsHelpers.hpp"
#include "DxsExceptions.hpp"


namespace CB::DxSurface {

  class TimedExecutorBase {
  public:
    TimedExecutorBase(TString name, unsigned int maxRefreshRateHz);
    TimedExecutorBase(const TimedExecutorBase&);
    TimedExecutorBase(TimedExecutorBase&&) = delete;
    TimedExecutorBase& operator=(const TimedExecutorBase&);
    TimedExecutorBase& operator=(TimedExecutorBase&&) = delete;

    virtual ~TimedExecutorBase();

    const TimedExecutorBase& operator()();
    void Run();

    ExecutionState GetState() const;

    void Pause();
    void Resume();
    void Exit();

    ExecutionExitReason GetExitReason() const;
    const TString& GetExitMessage() const;
    void WaitForExit();

  protected:
    virtual void OnExecutionStateInit() = 0;
    virtual void OnExecutionStateRunning(const double deltaSec) = 0;
    virtual void OnExecutionStatePaused(const double deltaSec) = 0;
    virtual void OnExecutionStateExitted(ExecutionExitReason, const TString& message) = 0;
    virtual void OnExecutionStateChanged(ExecutionState last, ExecutionState next) = 0;

  private:
    TString m_sName;
    unsigned int m_uiMaxRefreshRateHz;
    Ptr<Thread> m_pThread;
    volatile ExecutionState m_eState;
    volatile ExecutionCommand m_eCommand;
    volatile ExecutionExitReason m_eExitReason;
    TString m_eExitMessage;

    bool SetExecutionState(ExecutionState);
    void ProcessExecutionState(const double deltaSec);

    static void ExecutionThread(TimedExecutorBase* const b);

  }; // class TimedExecutorBase

} //- namespace CB::DxSurface



#endif
