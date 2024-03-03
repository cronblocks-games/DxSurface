#ifndef _DXSURFACE_DXSRESOURCES_H_
#define _DXSURFACE_DXSRESOURCES_H_



#include "DxsTypes.hpp"

#include <map>

namespace CB::DxSurface {

  class Icon;
  class Cursor;
  class Bitmap;

  class Resources
  {

  }; // class Resources

  class WinImageResource
  {
  protected:
    enum class ResourceType { Icon, Cursor, Bitmap };
    WinImageResource() = delete;
    WinImageResource(unsigned int resourceId, UINT type, int cx, int cy, UINT fuLoad, ResourceType, HINSTANCE hInstance = nullptr);
    WinImageResource(TString filepath, UINT type, int cx, int cy, UINT fuLoad, ResourceType);
    WinImageResource(const WinImageResource&);
    WinImageResource(WinImageResource&&) noexcept;
    WinImageResource& operator=(const WinImageResource&);
    WinImageResource& operator=(WinImageResource&&) noexcept;
    
    static unsigned int IncrementHandleCount() noexcept; // Returns updated count
    static unsigned int DecrementHandleCount(); // Returns updated count

    HANDLE m_hResource = nullptr;
  private:
    static Mutex s_mutResourceCount;
    static std::map<HANDLE, unsigned int> s_mapResourceCount;
  };

  class Icon : WinImageResource
  {
  public:
    Icon() = delete;
    Icon(unsigned int resourceId, int prefWidth = 32, int prefHeight = 32, UINT loadFlags = LR_DEFAULTSIZE | LR_SHARED, HINSTANCE hInstance = nullptr);
    Icon(TString filepath, int prefWidth = 32, int prefHeight = 32, UINT loadFlags = LR_DEFAULTSIZE | LR_LOADFROMFILE);
    Icon(const Icon&);
    Icon(Icon&&) noexcept;
    ~Icon();
    Icon& operator=(const Icon&);
    Icon& operator=(Icon&&) noexcept;
    operator HICON() const noexcept { return (HICON)m_hResource; }
  }; // class Icon

  class Cursor : WinImageResource
  {
  public:
    Cursor() = delete;
    Cursor(unsigned int resourceId, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE | LR_SHARED, HINSTANCE hInstance = nullptr);
    Cursor(TString filepath, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE | LR_LOADFROMFILE);
    Cursor(const Cursor&);
    Cursor(Cursor&&) noexcept;
    ~Cursor();
    Cursor& operator=(const Cursor&);
    Cursor& operator=(Cursor&&) noexcept;
    operator HCURSOR() const noexcept { return (HCURSOR)m_hResource; }
  }; // class Cursor

  class Bitmap : WinImageResource
  {
  public:
    Bitmap() = delete;
    Bitmap(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags = LR_SHARED, HINSTANCE hInstance = nullptr);
    Bitmap(TString filepath, int prefWidth, int prefHeight, UINT loadFlags = LR_LOADFROMFILE);
    Bitmap(const Bitmap&);
    Bitmap(Bitmap&&) noexcept;
    ~Bitmap();
    Bitmap& operator=(const Bitmap&);
    Bitmap& operator=(Bitmap&&) noexcept;
    operator HBITMAP() const noexcept { return (HBITMAP)m_hResource; }
  }; // class Bitmap

  
} //- namespace CB::DxSurface



#endif
