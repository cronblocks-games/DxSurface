#ifndef _DXAPP_GAMELOGIC_H_
#define _DXAPP_GAMELOGIC_H_


#include <DxSurface.hpp>


void GameWindowPositionChanged(CB::DxSurface::Window&, CB::DxSurface::WindowRect&, CB::DxSurface::ClientRect&);
void GameWindowSizeChanged(CB::DxSurface::Window&, CB::DxSurface::WindowRect&, CB::DxSurface::ClientRect&);

void GameRenderingInit(CB::DxSurface::Window&);
void GameRenderingRunning(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, double deltaTimeSec);
void GameRenderingPaused(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, double deltaTimeSec);
void GameRenderingExitted(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, CB::DxSurface::ExecutionExitReason reason, const CB::DxSurface::TString& message);
void GameRenderingStateChanged(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, CB::DxSurface::ExecutionState from, CB::DxSurface::ExecutionState to);

void GameProcessingInit(CB::DxSurface::Window&);
void GameProcessingRunning(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, double deltaTimeSec);
void GameProcessingPaused(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, double deltaTimeSec);
void GameProcessingExitted(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, CB::DxSurface::ExecutionExitReason reason, const CB::DxSurface::TString& message);
void GameProcessingStateChanged(CB::DxSurface::Window&, CB::DxSurface::Keyboard&, CB::DxSurface::Mouse&, CB::DxSurface::ExecutionState from, CB::DxSurface::ExecutionState to);



#endif
