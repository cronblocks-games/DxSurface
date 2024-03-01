#include "pch.hpp"
#include "DxSurface.hpp"

using namespace CB::DxSurface;

//- 
//- Rendering
//- 
void GameRenderingInit(Window& w)
{

}
void GameRenderingRunning(Window& w, Keyboard& k, Mouse& m, double deltaTimeSec)
{
  if (k.GetKeyStatus(KeyCode::Esc) == KeyStatus::Pressed)
  {
    w.Title(DxsT("Exiting"));
    w.Exit();
  }

  if (k.GetKeyStatus(KeyCode::F1) == KeyStatus::Pressed)
  {
    w.Title(DxsT("Help"));
  }
}
void GameRenderingPaused(Window& w, Keyboard& k, Mouse& m, double deltaTimeSec)
{
}
void GameRenderingExitted(Window& w, Keyboard& k, Mouse& m, ExecutionExitReason reason, const TString& message)
{
}
void GameRenderingStateChanged(Window& w, Keyboard& k, Mouse& m, ExecutionState from, ExecutionState to)
{
}

//- 
//- Processing
//- 
void GameProcessingInit(Window& w)
{

}
void GameProcessingRunning(Window& w, Keyboard& k, Mouse& m, double deltaTimeSec)
{

}
void GameProcessingPaused(Window& w, Keyboard& k, Mouse& m, double deltaTimeSec)
{

}
void GameProcessingExitted(Window& w, Keyboard& k, Mouse& m, ExecutionExitReason reason, const TString& message)
{

}
void GameProcessingStateChanged(Window& w, Keyboard& k, Mouse& m, ExecutionState from, ExecutionState to)
{

}

