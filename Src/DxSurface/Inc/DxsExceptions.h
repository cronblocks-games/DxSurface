#ifndef _DXSURFACE_DXSEXCEPTIONS_H_
#define _DXSURFACE_DXSEXCEPTIONS_H_



#include <string>
#include <exception>

#include "DxsTypes.h"


namespace CB::DxSurface {

  class Exception : public std::exception {
  public:
    Exception() = delete;
    Exception(Exception&) = delete;
    Exception(Exception&&) = delete;
    Exception& operator =(Exception&) = delete;
    Exception& operator =(Exception&&) = delete;

    Exception(const char* file, int lineNumber, const char* message);
    char const* what() const override;

  private:
    mutable std::string m_sMessage;
  };

}



#endif
