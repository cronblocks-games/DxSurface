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
    enum class ResourceType { NONE, Icon, Cursor, Bitmap };
    enum class Source { NONE, System, ExeEmbedded, File };

    WinImageResource() = delete;
    WinImageResource(unsigned int resourceId, int cx, int cy, UINT flags, ResourceType, Source, HINSTANCE hInstance = nullptr);
    WinImageResource(const TString& filepath, int cx, int cy, UINT flags, ResourceType, Source);
    WinImageResource(const WinImageResource&);
    WinImageResource(WinImageResource&&) noexcept;
    virtual ~WinImageResource();
    WinImageResource& operator=(const WinImageResource&);
    WinImageResource& operator=(WinImageResource&&) noexcept;
    
    static unsigned int IncrementHandleCount(HANDLE rh) noexcept; // Returns updated count
    static unsigned int DecrementHandleCount(HANDLE rh); // Returns updated count

    HANDLE m_hResource;
    Source m_eSource;

  private:
    static Mutex s_mutResourceCount;
    static std::map<HANDLE, unsigned int> s_mapResourceCount;
  };

  class Icon : WinImageResource
  {
  public:
    Icon() = delete;
    Icon(SystemIcon, int prefWidth = 32, int prefHeight = 32, UINT loadFlags = LR_DEFAULTSIZE);
    Icon(unsigned int resourceId, int prefWidth = 32, int prefHeight = 32, UINT loadFlags = LR_DEFAULTSIZE, HINSTANCE hInstance = nullptr);
    Icon(const TString& filepath, int prefWidth = 32, int prefHeight = 32, UINT loadFlags = LR_DEFAULTSIZE);
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
    Cursor(SystemCursor, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE);
    Cursor(unsigned int resourceId, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE, HINSTANCE hInstance = nullptr);
    Cursor(const TString& filepath, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = LR_DEFAULTSIZE);
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
    Bitmap(SystemBitmap, int prefWidth = 0, int prefHeight = 0, UINT loadFlags = 0);
    Bitmap(unsigned int resourceId, int prefWidth, int prefHeight, UINT loadFlags = 0, HINSTANCE hInstance = nullptr);
    Bitmap(const TString& filepath, int prefWidth, int prefHeight, UINT loadFlags = 0);
    Bitmap(const Bitmap&);
    Bitmap(Bitmap&&) noexcept;
    ~Bitmap();
    Bitmap& operator=(const Bitmap&);
    Bitmap& operator=(Bitmap&&) noexcept;
    operator HBITMAP() const noexcept { return (HBITMAP)m_hResource; }
  }; // class Bitmap

  
} //- namespace CB::DxSurface



#endif
