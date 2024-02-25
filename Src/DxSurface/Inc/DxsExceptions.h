#ifndef _DXSURFACE_DXSEXCEPTIONS_H_
#define _DXSURFACE_DXSEXCEPTIONS_H_



#include <exception>

#include "DxsHelpers.h"


namespace CB::DxSurface {

  class Exception : public std::exception {
  public:
    Exception() = delete;
    Exception(const Exception&) = delete;
    Exception(Exception&&) = delete;

    Exception& operator=(const Exception&) = delete;
    Exception& operator=(Exception&&) = delete;

    Exception(const char* file, int lineNumber, CStrCharPtr message);
    const String& Message() const;

  private:
    mutable String m_sMessage;
  };

}

#define DxsThrow(msg) {throw CB::DxSurface::Exception(__FILE__,__LINE__,msg);}
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



#endif
