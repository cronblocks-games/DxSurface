#include "pch.h"
#include "DxsExceptions.h"


using namespace std;
using namespace CB::DxSurface;

static const char* start_of_str(const char* line, const char* str)
{
  const char* match = strstr(line, str);
  return match == nullptr ? line : match;
}

Exception::Exception(const char* file, int lineNumber, ConstTCharPtr message)
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss
    << "Exception:" << endl
    << "    -- " << message << endl << endl

    << "File: " << start_of_str(file, "Src\\DxSurface\\Src") << endl
    << "Line: " << lineNumber << endl
    << "Ver: " << hex << DxsVersion
    << " (" << DxsVersionString << ") - "
    << DxsVersionReleaseDateString;

  m_sMessage = ss.str();
}

const TString& Exception::Message() const
{
  return m_sMessage;
}

