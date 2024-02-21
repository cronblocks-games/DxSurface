#include "pch.h"
#include "DxSurface.h"

using namespace CB::DxSurface;

int WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PTSTR pCmdLine,
  int nCmdShow)
{
  try
  {
    DxSurface surface;

    Window& window = surface.CreateNewWindow("Test Window", 10, 10, 200, 200);

    surface.Run();
  }
  catch (Exception& ex)
  {
    MessageBox(nullptr, ex.what(), "Error - DxSurface", 0);
    return -1;
  }
  catch (std::exception& ex)
  {
    MessageBox(nullptr, ex.what(), "Error", 0);
    return -1;
  }
  catch ( ... )
  {
    MessageBox(nullptr, "Unknown error occurred.", "Error", 0);
    return -1;
  }

  return 0;
}