#include "pch.hpp"
#include "DxSurface.hpp"

#include <string>
#include <sstream>

using namespace std;
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
    w.SetTitle(DxsT("Exiting"));
    w.Exit();
  }

  if (k.GetKeyStatus(KeyCode::F1) == KeyStatus::Pressed)
  {
    w.SetTitle(DxsT("Help"));
  }

  if (k.GetKeyStatus(KeyCode::M) == KeyStatus::Pressed)
  {
    POINT pt = m.GetPosition();
    wstringstream s;
    s << "(" << pt.x << ", " << pt.y << ") ";
    if (m.GetButtonStatus(MouseButton::Left) == KeyStatus::Pressed) s << "L";
    if (m.GetButtonStatus(MouseButton::Middle) == KeyStatus::Pressed) s << "M";
    if (m.GetButtonStatus(MouseButton::Right) == KeyStatus::Pressed) s << "R";
    w.SetTitle(s.str());
  }
}
void GameRenderingPaused(Window& w, Keyboard& k, Mouse& m, double deltaTimeSec)
{
}
void GameRenderingExitted(Window& w, Keyboard& k, Mouse& m, ExecutionExitReason reason, const TString& message)
{
  if (reason == ExecutionExitReason::Exception)
  {
    MessageBox(nullptr, message.c_str(), DxsT("Rendering Error"), 0);
  }
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
  if (reason == ExecutionExitReason::Exception)
  {
    MessageBox(nullptr, message.c_str(), DxsT("Processing Error"), 0);
  }
}
void GameProcessingStateChanged(Window& w, Keyboard& k, Mouse& m, ExecutionState from, ExecutionState to)
{

}

