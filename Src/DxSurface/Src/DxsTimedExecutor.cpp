#include "pch.hpp"
#include "DxsTimedExecutor.hpp"

using namespace std;
using namespace CB::DxSurface;

TimedExecutorBase::TimedExecutorBase(TString name, unsigned int maxRefreshRateHz)
{
  m_sName = name;
  m_uiMaxRefreshRateHz = maxRefreshRateHz;

  if (maxRefreshRateHz == 0)
  {
    DxsThrow((m_sName + DxsT(" - Zero (Hz) refresh rate is invalid")).c_str());
  }

  m_pThread.reset();
  m_eState = ExecutionState::NONE;
  m_eCommand = ExecutionCommand::Exit;
  m_eExitReason = ExecutionExitReason::Normal;
  m_eExitMessage = DxsT("");
}
TimedExecutorBase::TimedExecutorBase(const TimedExecutorBase& o)
{
  if (this == &o) return;

  this->operator=(o);
}
TimedExecutorBase::~TimedExecutorBase()
{
  m_eCommand = ExecutionCommand::Exit;

  if (m_pThread)
  {
    m_pThread->join();
    m_pThread.reset();
  }
}

TimedExecutorBase& TimedExecutorBase::operator=(const TimedExecutorBase& o)
{
  if (this == &o) return *this;

  m_sName = o.m_sName;
  m_uiMaxRefreshRateHz = o.m_uiMaxRefreshRateHz;

  // Thread cannot be copied, each object has its own thread
  m_pThread.reset();
  m_eState = ExecutionState::NONE;
  m_eCommand = ExecutionCommand::Exit;
  m_eExitReason = ExecutionExitReason::Normal;
  m_eExitMessage = DxsT("");

  return *this;
}

const TimedExecutorBase& TimedExecutorBase::operator()()
{
  Run();
  return *this;
}

void TimedExecutorBase::Run()
{
  if (m_pThread) return;
  m_eCommand = ExecutionCommand::Run;
  m_pThread = std::make_shared<Thread>(TimedExecutorBase::ExecutionThread, this);
}

ExecutionState TimedExecutorBase::GetState() const
{
  return m_eState;
}

void TimedExecutorBase::Pause()
{
  if (m_eState == ExecutionState::Exitted)
  {
    DxsThrow((m_sName + DxsT(" - cannot pause execution when it has already exitted")).c_str());
  }

  m_eCommand = ExecutionCommand::Pause;
}

void TimedExecutorBase::Resume()
{
  if (m_eState == ExecutionState::Exitted)
  {
    DxsThrow((m_sName + DxsT(" - cannot resume execution when it has already exitted")).c_str());
  }

  m_eCommand = ExecutionCommand::Run;
}

void TimedExecutorBase::Exit()
{
  m_eCommand = ExecutionCommand::Exit;
}

ExecutionExitReason TimedExecutorBase::GetExitReason() const
{
  return m_eExitReason;
}

const TString& TimedExecutorBase::GetExitMessage() const
{
  return m_eExitMessage;
}

void TimedExecutorBase::WaitForExit()
{
  if (m_pThread)
  {
    m_pThread->join();
    m_pThread.reset();
  }
}

bool TimedExecutorBase::SetExecutionState(ExecutionState s)
{
  if (m_eState != s)
  {
    OnExecutionStateChanged(m_eState, s);
    m_eState = s;
    return true;
  }
  return false;
}
void TimedExecutorBase::ProcessExecutionState(const double deltaSec)
{
  switch (m_eState)
  {
  case ExecutionState::NONE: break;
  case ExecutionState::Init: OnExecutionStateInit(); break;
  case ExecutionState::Running: OnExecutionStateRunning(deltaSec); break;
  case ExecutionState::Paused: OnExecutionStatePaused(deltaSec); break;
  case ExecutionState::Exitted: break;
  }
}

void TimedExecutorBase::ExecutionThread(TimedExecutorBase* const b)
{
  b->m_eExitReason = ExecutionExitReason::Normal;
  b->m_eExitMessage = DxsT("");

  try
  {
    b->SetExecutionState(ExecutionState::Init);
    b->ProcessExecutionState(0.0);

    if (true)
    {
      const double timePerIterationMSec = 1000.0 / b->m_uiMaxRefreshRateHz;

      TimePoint iterationStartTime, lastExecutionTime = Clock::now();

      while (b->m_eCommand != ExecutionCommand::Exit)
      {
        iterationStartTime = Clock::now(); // timestamp

        //- Setting the state if changed
        switch (b->m_eCommand)
        {
        case ExecutionCommand::Run:
          if (b->SetExecutionState(ExecutionState::Running))
          {
            lastExecutionTime = Clock::now();
          }
          break;
        case ExecutionCommand::Pause:
          if (b->SetExecutionState(ExecutionState::Paused))
          {
            lastExecutionTime = Clock::now();
          }
          break;
        }

        //- Processing the state as needed
        b->ProcessExecutionState(TimeDurationSec(Clock::now() - lastExecutionTime).count());
        lastExecutionTime = Clock::now(); // timestamp

        //- Exit if commanded, before going to sleep
        if (b->m_eCommand == ExecutionCommand::Exit) { break; }

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
      b->m_eExitMessage = DxsT("Zero (Hz) refresh rate has caused an exit from execution");
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
  b->OnExecutionStateExitted(b->m_eExitReason, b->m_eExitMessage);
}

