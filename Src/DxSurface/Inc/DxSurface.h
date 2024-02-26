#ifndef _DXSURFACE_DXSURFACE_H_
#define _DXSURFACE_DXSURFACE_H_

#include "DxsHelpers.h"
#include "DxsWindow.h"
#include "DxsExceptions.h"


namespace CB::DxSurface {

  class DxSurface {

  public:
    DxSurface(HINSTANCE hInstance = nullptr) noexcept;
    DxSurface(const WindowCreationOptions& options, HINSTANCE hInstance = nullptr) noexcept;

    DxSurface(const DxSurface&);
    DxSurface(DxSurface&&) noexcept;
    DxSurface& operator=(const DxSurface&);
    DxSurface& operator=(DxSurface&&) noexcept;

    ~DxSurface() = default;

    void UpdateWindowCreationOptions(const WindowCreationOptions&) noexcept;

    Ptr<Window> CreateNewWindow();
    Ptr<Window> CreateNewWindow(const TString& title);
    Ptr<Window> CreateNewWindow(const TString& title, int x, int y);
    Ptr<Window> CreateNewWindow(const TString& title, int x, int y, int width, int height, bool isPrimary = false, bool debugEnabled = true);
    Ptr<Window> CreateNewWindow(const WindowCreationOptions&);

    void Run();

  private:
    HINSTANCE m_hInstance;
    WindowCreationOptions m_stOptions;

    Vector<Ptr<Window>> m_vWindows;
  };

} //- namespace CB::DxSurface



#endif
