#include "pch.hpp"
#include "DxSurface.hpp"

#include <string>
#include <sstream>

using namespace std;
using namespace CB::DxSurface;

//- 
//- Window Changes
//- 
void GameWindowPositionChanged(Window& w, WindowRect& wr, ClientRect& cr)
{
}
void GameWindowSizeChanged(Window& w, WindowRect& wr, ClientRect& cr)
{
}

//- 
//- Rendering
//- 
void GameRenderingInit(Window& w, Graphics& gfx)
{
}
void GameRenderingRunning(Window& w, Graphics& gfx, Keyboard& k, Mouse& m, double deltaTimeSec)
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
    if (m.GetButtonStatus(MouseButton::Left) == ButtonStatus::Pressed) s << "L";
    if (m.GetButtonStatus(MouseButton::Middle) == ButtonStatus::Pressed) s << "M";
    if (m.GetButtonStatus(MouseButton::Right) == ButtonStatus::Pressed) s << "R";
    w.SetTitle(s.str());
  }

  if (k.GetKeyStatus(KeyCode::F) == KeyStatus::Pressed)
  {
    wstringstream s;
    s << "T/Frames = " << gfx.GetTotalFrames() << ",  AvgFPS = " << gfx.GetAverageFPS() << ",  FPS = " << gfx.GetFPS() << "";
    w.SetTitle(s.str());
  }
}
void GameRenderingPaused(Window& w, Graphics& gfx, Keyboard& k, Mouse& m, double deltaTimeSec)
{
}
void GameRenderingExitted(Window& w, Graphics& gfx, Keyboard& k, Mouse& m, ExecutionExitReason reason, const TString& message)
{
  if (reason == ExecutionExitReason::Exception)
  {
    MessageBox(nullptr, message.c_str(), DxsT("Rendering Error"), 0);
  }
}
void GameRenderingStateChanged(Window& w, Graphics& gfx, Keyboard& k, Mouse& m, ExecutionState from, ExecutionState to)
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

