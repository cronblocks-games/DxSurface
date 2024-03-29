#ifndef _DXSURFACE_DXSEXECUTIONMANAGER_H_
#define _DXSURFACE_DXSEXECUTIONMANAGER_H_



#include "DxsTypes.hpp"
#include "DxsExceptions.hpp"


namespace CB::DxSurface {

  class TimedExecutorBase {
  public:
    TimedExecutorBase(
      TString name, /* The name of the executor */
      unsigned int maxRefreshRateHz /* Zero value doesn't terminate execution in timing model DxsTimingModelNoSleep */
    );
    TimedExecutorBase(const TimedExecutorBase&);
    TimedExecutorBase(TimedExecutorBase&&) = delete;
    TimedExecutorBase& operator=(const TimedExecutorBase&);
    TimedExecutorBase& operator=(TimedExecutorBase&&) = delete;

    virtual ~TimedExecutorBase();

    const TString& GetName() const;
    unsigned int GetMaxRefreshRateHz() const;

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
    unsigned int m_uiMaxRefreshRateHz; // Zero value doesn't terminate execution in timing model DxsTimingModelNoSleep
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
  class TimedExecutor : public TimedExecutorBase {
  public:
    using FuncTInit = void (ClassT::*)();
    using FuncTRunning = void (ClassT::*)(const double deltaSec);
    using FuncTPaused = void (ClassT::*)(const double deltaSec);
    using FuncTExitted = void (ClassT::*)(ExecutionExitReason, const TString& message);
    using FuncTStateChanged = void (ClassT::*)(ExecutionState from, ExecutionState to);

    TimedExecutor(
      ClassT* classPtr,
      TString name, unsigned int maxRefreshRateHz,
      FuncTInit onInitFunc = nullptr,
      FuncTRunning onRunningFunc = nullptr,
      FuncTPaused onPausedFunc = nullptr,
      FuncTExitted onExittedFunc = nullptr,
      FuncTStateChanged onStateChangedFunc = nullptr) : TimedExecutorBase(name, maxRefreshRateHz)
    {
      if (classPtr == nullptr)
      {
        DxsThrow((name + DxsT(" - TimedExecutor<ClassT> - Invalid or nullptr for activated class provided")).c_str());
      }

      m_pClassPtr = classPtr;
      m_fpOnInitFunc = onInitFunc;
      m_fpOnRunningFunc = onRunningFunc;
      m_fpOnPausedFunc = onPausedFunc;
      m_fpOnExittedFunc = onExittedFunc;
      m_fpOnStateChangedFunc = onStateChangedFunc;
    }
    explicit TimedExecutor(const TimedExecutor& o) : TimedExecutorBase(o.GetName(), o.GetMaxRefreshRateHz())
    {
      m_pClassPtr = o.m_pClassPtr;
      m_fpOnInitFunc = o.m_fpOnInitFunc;
      m_fpOnRunningFunc = o.m_fpOnRunningFunc;
      m_fpOnPausedFunc = o.m_fpOnPausedFunc;
      m_fpOnExittedFunc = o.m_fpOnExittedFunc;
      m_fpOnStateChangedFunc = o.m_fpOnStateChangedFunc;
    }
    TimedExecutor(TimedExecutor&&) = delete;
    TimedExecutor& operator=(const TimedExecutor& o)
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
    TimedExecutor& operator=(TimedExecutor&&) = delete;

    ~TimedExecutor()
    {
    }

  protected:
    void OnExecutionStateInit() override final
    {
      if (m_fpOnInitFunc != nullptr)
      {
        (m_pClassPtr->*m_fpOnInitFunc)();
      }
    }
    void OnExecutionStateRunning(const double deltaSec) override final
    {
      if (m_fpOnRunningFunc != nullptr)
      {
        (m_pClassPtr->*m_fpOnRunningFunc)(deltaSec);
      }
    }
    void OnExecutionStatePaused(const double deltaSec) override final
    {
      if (m_fpOnPausedFunc != nullptr)
      {
        (m_pClassPtr->*m_fpOnPausedFunc)(deltaSec);
      }
    }
    void OnExecutionStateExitted(ExecutionExitReason r, const TString& message) override final
    {
      if (m_fpOnExittedFunc != nullptr)
      {
        (m_pClassPtr->*m_fpOnExittedFunc)(r, message);
      }
    }
    void OnExecutionStateChanged(ExecutionState last, ExecutionState next) override final
    {
      if (m_fpOnStateChangedFunc != nullptr)
      {
        (m_pClassPtr->*m_fpOnStateChangedFunc)(last, next);
      }
    }

  private:
    ClassT* m_pClassPtr;
    FuncTInit m_fpOnInitFunc;
    FuncTRunning m_fpOnRunningFunc;
    FuncTPaused m_fpOnPausedFunc;
    FuncTExitted m_fpOnExittedFunc;
    FuncTStateChanged m_fpOnStateChangedFunc;

  }; // class TimedExecutor<T>

  class SimpleTimedExecutor : public TimedExecutorBase {
  public:
    using FuncTInit = void (*)();
    using FuncTRunning = void (*)(const double deltaSec);
    using FuncTPaused = void (*)(const double deltaSec);
    using FuncTExitted = void (*)(ExecutionExitReason, const TString& message);
    using FuncTStateChanged = void (*)(ExecutionState from, ExecutionState to);

    SimpleTimedExecutor(
      TString name, unsigned int maxRefreshRateHz,
      FuncTInit onInitFunc = nullptr,
      FuncTRunning onRunningFunc = nullptr,
      FuncTPaused onPausedFunc = nullptr,
      FuncTExitted onExittedFunc = nullptr,
      FuncTStateChanged onStateChangedFunc = nullptr) : TimedExecutorBase(name, maxRefreshRateHz)
    {
      m_fpOnInitFunc = onInitFunc;
      m_fpOnRunningFunc = onRunningFunc;
      m_fpOnPausedFunc = onPausedFunc;
      m_fpOnExittedFunc = onExittedFunc;
      m_fpOnStateChangedFunc = onStateChangedFunc;
    }
    explicit SimpleTimedExecutor(const SimpleTimedExecutor& o) : TimedExecutorBase(o.GetName(), o.GetMaxRefreshRateHz())
    {
      m_fpOnInitFunc = o.m_fpOnInitFunc;
      m_fpOnRunningFunc = o.m_fpOnRunningFunc;
      m_fpOnPausedFunc = o.m_fpOnPausedFunc;
      m_fpOnExittedFunc = o.m_fpOnExittedFunc;
      m_fpOnStateChangedFunc = o.m_fpOnStateChangedFunc;
    }
    SimpleTimedExecutor(SimpleTimedExecutor&&) = delete;
    SimpleTimedExecutor& operator=(const SimpleTimedExecutor& o)
    {
      if (this == &o) return *this;

      m_fpOnInitFunc = o.m_fpOnInitFunc;
      m_fpOnRunningFunc = o.m_fpOnRunningFunc;
      m_fpOnPausedFunc = o.m_fpOnPausedFunc;
      m_fpOnExittedFunc = o.m_fpOnExittedFunc;
      m_fpOnStateChangedFunc = o.m_fpOnStateChangedFunc;

      return *this;
    }
    SimpleTimedExecutor& operator=(SimpleTimedExecutor&&) = delete;

    ~SimpleTimedExecutor()
    {
    }

  protected:
    void OnExecutionStateInit() override final
    {
      if (m_fpOnInitFunc != nullptr)
      {
        (*m_fpOnInitFunc)();
      }
    }
    void OnExecutionStateRunning(const double deltaSec) override final
    {
      if (m_fpOnRunningFunc != nullptr)
      {
        (*m_fpOnRunningFunc)(deltaSec);
      }
    }
    void OnExecutionStatePaused(const double deltaSec) override final
    {
      if (m_fpOnPausedFunc != nullptr)
      {
        (*m_fpOnPausedFunc)(deltaSec);
      }
    }
    void OnExecutionStateExitted(ExecutionExitReason r, const TString& message) override final
    {
      if (m_fpOnExittedFunc != nullptr)
      {
        (*m_fpOnExittedFunc)(r, message);
      }
    }
    void OnExecutionStateChanged(ExecutionState last, ExecutionState next) override final
    {
      if (m_fpOnStateChangedFunc != nullptr)
      {
        (*m_fpOnStateChangedFunc)(last, next);
      }
    }

  private:
    FuncTInit m_fpOnInitFunc;
    FuncTRunning m_fpOnRunningFunc;
    FuncTPaused m_fpOnPausedFunc;
    FuncTExitted m_fpOnExittedFunc;
    FuncTStateChanged m_fpOnStateChangedFunc;

  }; // class SimpleTimedExecutor

} //- namespace CB::DxSurface



#endif
