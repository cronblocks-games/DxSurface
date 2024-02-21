#ifndef _DXSURFACE_DXSWINDOW_H_
#define _DXSURFACE_DXSWINDOW_H_



#include <string>

#include "DxsTypes.h"
#include "DxsWinApi.h"


namespace CB {
  namespace DxSurface {

    class Window {
      public:
        Window() = delete;
        Window(const std::string& name, int x, int y, int width, int height, bool isPrimary, bool debugEnabled);

        Window(const Window& other);
        Window(Window&& other) noexcept;
        
        Window& operator=(const Window& other);
        Window& operator=(Window&& other) noexcept;

        virtual ~Window();

        void Show();
        void Hide();
        void RunRendering();

        const std::string& GetName();

      private:
        int x, y, width, height;
        bool isPrimary, debugEnabled;
        mutable std::string name;
    };
  }
}



#endif
