#include "pch.h"
#include "DxsExceptions.h"


using namespace std;
using namespace CB::DxSurface;

Exception::Exception(const char* file, int lineNumber, const char* message)
{
  stringstream ss;
  ss << "Exception (ver " << DXSURFACE_VERSION_STRING << ")"
    << " in file \"" << file << "\""
    << " @ line# " << lineNumber << " -- "
    << message;

  m_sMessage = ss.str();
}

char const* CB::DxSurface::Exception::what() const
{
  return m_sMessage.c_str();
}
