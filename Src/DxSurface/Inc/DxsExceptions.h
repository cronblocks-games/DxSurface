#ifndef _DXSURFACE_DXSEXCEPTIONS_H_
#define _DXSURFACE_DXSEXCEPTIONS_H_



#include <string>
#include <exception>

#include "DxsTypes.h"


namespace CB::DxSurface {

  class Exception : public std::exception {
  public:
    Exception() = delete;
    Exception(const Exception&) = delete;
    Exception(Exception&&) = delete;

    Exception& operator=(const Exception&) = delete;
    Exception& operator=(Exception&&) = delete;

    Exception(const char* file, int lineNumber, const char* message);
    char const* what() const override;

  private:
    mutable std::string m_sMessage;
  };

}

#define DXSURFACE_THROW(msg) {throw CB::DxSurface::Exception(__FILE__,__LINE__,msg);}
#define DXSURFACE_ENCLOSE_THROW(call) {                                   \
            try {                                                         \
              call;                                                       \
            } catch (Exception& ex) {                                     \
              MessageBox(nullptr, ex.what(), "Error - DxSurface", 0);     \
            } catch (std::exception& ex) {                                \
              MessageBox(nullptr, ex.what(), "Error", 0);                 \
            } catch (...) {                                               \
              MessageBox(nullptr, "Unknown error occurred.", "Error", 0); \
            }}



#endif
