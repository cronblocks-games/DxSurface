#ifndef _DXSURFACE_DXSEXCEPTIONS_H_
#define _DXSURFACE_DXSEXCEPTIONS_H_



#include <exception>

#include "DxsHelpers.hpp"


namespace CB::DxSurface {

  class Exception : public std::exception {
  public:
    Exception() = delete;
    Exception(const char* file, int lineNumber, ConstTCharPtr message);
    Exception(const Exception&);
    Exception(Exception&&) noexcept;

    Exception& operator=(const Exception&);
    Exception& operator=(Exception&&) noexcept;

    const TString& Message() const;

  private:
    mutable TString m_sMessage;
  };

} //- namespace CB::DxSurface

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
