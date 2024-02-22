#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include <string>

#include "DxsTypes.h"
#include "DxsWinApi.h"


namespace CB::DxSurface {

  class Window {
  public:
    Window() = delete;
    Window(const std::string& title, int x, int y, int width, int height, bool isPrimary, bool debugEnabled);
    Window(const Window& other);
    Window(Window&& other) noexcept;

    Window& operator=(const Window& other);
    Window& operator=(Window&& other) noexcept;

    virtual ~Window();

    bool IsPrimary();
    const std::string& Title();
    const std::string& Title(const std::string&);

    void Show();
    void Hide();
    void RunRendering();

  private:
    int m_iX, m_iY, m_iWidth, m_iHeight;
    bool m_bIsPrimary, m_bIsDebugEnabled;
    mutable std::string m_sTitle;

    HWND m_hWnd;

    HWND RegisterClassAndCreateWindow();
  };

}



#endif
