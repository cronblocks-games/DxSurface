#include "pch.h"
#include "DxSurface.h"

using namespace CB::DxSurface;

int WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  PSTR pCmdLine,
  int nCmdShow)
{
  try
  {
    DxSurface surface(hInstance);

    Ptr<Window> w1 = surface.CreateNewWindow(DxsT("Test Window 1"), 10, 10, 200, 200);

    surface.Run();
  }
  catch (Exception& ex)
  {
    MessageBox(nullptr, ex.Message().c_str(), DxsT("Error - DxSurface"), 0);
    return -1;
  }
  catch (std::exception& ex)
  {
    MessageBoxA(nullptr, ex.what(), "Error", 0);
    return -1;
  }
  catch ( ... )
  {
    MessageBoxA(nullptr, "Unknown error occurred.", "Error", 0);
    return -1;
  }

  return 0;
}