#include "pch.h"
#include "DxsExceptions.h"


using namespace std;
using namespace CB::DxSurface;

static const char* start_of_str(const char* line, const char* str)
{
  const char* match = strstr(line, str);
  return match == nullptr ? line : match;
}

Exception::Exception(const char* file, int lineNumber, const char* message)
{
  stringstream ss;

  ss
    << "Exception:" << endl
    << "    -- " << message << endl << endl

    << "File: " << start_of_str(file, "Src\\DxSurface\\Src") << endl
    << "Line: " << lineNumber << endl
    << "Ver: " << hex << DXSURFACE_VERSION
    << " (" << DXSURFACE_VERSION_STRING << ") - "
    << DXSURFACE_VERSION_RELEASE_DATE_STRING;

  m_sMessage = ss.str();
}

char const* CB::DxSurface::Exception::what() const
{
  return m_sMessage.c_str();
}
