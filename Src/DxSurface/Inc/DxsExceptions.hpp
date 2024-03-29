#ifndef _DXSURFACE_DXSEXCEPTIONS_H_
#define _DXSURFACE_DXSEXCEPTIONS_H_



#include <exception>

#include "DxsTypes.hpp"


namespace CB::DxSurface {

  class Exception : public std::exception {
  public:
    Exception() = delete;
    Exception(const char* file, int lineNumber, ConstTCharPtr message);
    Exception(const Exception&);
    Exception(Exception&&) noexcept;

    Exception& operator=(const Exception&);
    Exception& operator=(Exception&&) noexcept;

    virtual TString& Message() const;

  protected:
    int m_iLineNumber;
    std::string m_sFileName;
    TString m_sProvidedMessage;
    mutable TString m_sFinalMessage;
  }; //- class Exception



  class WindowsException : public Exception {
  public:
    WindowsException() = delete;
    WindowsException(const char* file, int lineNumber, ConstTCharPtr message, HRESULT hr);
    WindowsException(const WindowsException&);
    WindowsException(WindowsException&&) noexcept;

    WindowsException& operator=(const WindowsException&);
    WindowsException& operator=(WindowsException&&) noexcept;

    virtual TString& Message() const override;

  protected:
    HRESULT m_hr;
  }; //- class WindowsException



  class GraphicsException : public Exception {
  public:
    GraphicsException() = delete;
    GraphicsException(const char* file, int lineNumber, ConstTCharPtr message, HRESULT hr);
    GraphicsException(const GraphicsException&);
    GraphicsException(GraphicsException&&) noexcept;

    GraphicsException& operator=(const GraphicsException&);
    GraphicsException& operator=(GraphicsException&&) noexcept;

    virtual TString& Message() const override;

  protected:
    HRESULT m_hr;
  }; //- class GraphicsException

} //- namespace CB::DxSurface

#define DxsFailed(expr)    ((expr) == 0)
#define DxsFailedNz(expr)  ((expr) != 0)
#define DxsFailedPtr(expr) ((expr) == nullptr)

#define DxsThrow(msg)              { throw CB::DxSurface::Exception(__FILE__, __LINE__, msg);                        }
#define DxsThrowWindows(msg)       { throw CB::DxSurface::WindowsException(__FILE__, __LINE__, msg, GetLastError()); }
#define DxsThrowWindowsHr(msg,hr)  { throw CB::DxSurface::WindowsException(__FILE__, __LINE__, msg, hr);             }
#define DxsThrowGraphicsHr(msg,hr) { throw CB::DxSurface::GraphicsException(__FILE__, __LINE__, msg, hr);            }

#define DxsEncloseThrow(call) {                                                         \
            try {                                                                       \
              call;                                                                     \
            } catch (Exception& ex) {                                                   \
              MessageBox(nullptr, ex.Message().c_str(), DxsT("Error - DxSurface"), 0);  \
            } catch (std::exception& ex) {                                              \
              MessageBoxA(nullptr, ex.what(), "Error", 0);                              \
            } catch (...) {                                                             \
              MessageBoxA(nullptr, "Unknown error occurred.", "Error", 0);              \
            }}
#define DxsEncloseThrowNoMessage(call) {try { call; } catch (...) {}}



#endif
