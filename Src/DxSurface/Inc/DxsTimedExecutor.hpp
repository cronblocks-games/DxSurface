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

  template <class ClassT>
  class TimedClassExecutor : public TimedExecutorBase {
  public:
    using FuncTInit = void (ClassT::*)();
    using FuncTRunning = void (ClassT::*)(const double deltaSec);
    using FuncTPaused = void (ClassT::*)(const double deltaSec);
    using FuncTExitted = void (ClassT::*)(ExecutionExitReason, const TString& message);
    using FuncTStateChanged = void (ClassT::*)(ExecutionState from, ExecutionState to);

    TimedClassExecutor(
      Ptr<ClassT> classPtr,
      TString name, unsigned int maxRefreshRateHz,
      FuncTInit onInitFunc = nullptr,
      FuncTRunning onRunningFunc = nullptr,
      FuncTPaused onPausedFunc = nullptr,
      FuncTExitted onExittedFunc = nullptr,
      FuncTStateChanged onStateChangedFunc = nullptr) : TimedExecutorBase(name, maxRefreshRateHz)
    {
      if (!classPtr || classPtr.get() == nullptr)
      {
        DxsThrow(name + DxsT(" - TimedClassExecutor - Invalid or nullptr for activated class provided"));
      }

      m_pClassPtr = classPtr;
      m_fpOnInitFunc = onInitFunc;
      m_fpOnRunningFunc = onRunningFunc;
      m_fpOnPausedFunc = onPausedFunc;
      m_fpOnExittedFunc = onExittedFunc;
      m_fpOnStateChangedFunc = onStateChangedFunc;
    }
    TimedClassExecutor(const TimedClassExecutor& o)
    {
      if (this == &o) return;
    }
    TimedClassExecutor(TimedClassExecutor&&) = delete;
    TimedClassExecutor& operator=(const TimedClassExecutor& o)
    {
      if (this == &o) return *this;

      m_pClassPtr = o.m_pClassPtr;
      m_fpOnInitFunc = o.m_fpOnInitFunc;
      m_fpOnRunningFunc = o.m_fpOnRunningFunc;
      m_fpOnPausedFunc = o.m_fpOnPausedFunc;
      m_fpOnExittedFunc = o.m_fpOnExittedFunc;
      m_fpOnStateChangedFunc = o.m_fpOnStateChangedFunc;

      return *this;
    }
    TimedClassExecutor& operator=(TimedClassExecutor&&) = delete;

    ~TimedClassExecutor()
    {
    }

  protected:
    void OnExecutionStateInit() override final
    {
      if (m_fpOnInitFunc != nullptr)
      {
        m_pClassPtr->m_fpOnInitFunc();
      }
    }
    void OnExecutionStateRunning(const double deltaSec) override final
    {
      if (m_fpOnRunningFunc != nullptr)
      {
        m_pClassPtr->m_fpOnRunningFunc(deltaSec);
      }
    }
    void OnExecutionStatePaused(const double deltaSec) override final
    {
      if (m_fpOnPausedFunc != nullptr)
      {
        m_pClassPtr->m_fpOnPausedFunc(deltaSec);
      }
    }
    void OnExecutionStateExitted(ExecutionExitReason r, const TString& message) override final
    {
      if (m_fpOnExittedFunc != nullptr)
      {
        m_pClassPtr->m_fpOnExittedFunc(r, message);
      }
    }
    void OnExecutionStateChanged(ExecutionState last, ExecutionState next) override final
    {
      if (m_fpOnStateChangedFunc != nullptr)
      {
        m_pClassPtr->m_fpOnStateChangedFunc(last, next);
      }
    }

  private:
    Ptr<ClassT> m_pClassPtr;
    FuncTInit m_fpOnInitFunc;
    FuncTRunning m_fpOnRunningFunc;
    FuncTPaused m_fpOnPausedFunc;
    FuncTExitted m_fpOnExittedFunc;
    FuncTStateChanged m_fpOnStateChangedFunc;

  }; // class TimedClassExecutor

} //- namespace CB::DxSurface



#endif
