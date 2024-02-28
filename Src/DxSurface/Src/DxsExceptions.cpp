#include "pch.hpp"
#include "DxsExceptions.hpp"


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

Exception::Exception(const Exception& o)
{
  if (this == &o) return;
  *this = o;
}
Exception::Exception(Exception&& o) noexcept
{
  if (this == &o) return;
  *this = move(o);
}
Exception& Exception::operator=(const Exception& o)
{
  if (this == &o) return *this;
  m_sMessage = o.m_sMessage;
  return *this;
}
Exception& Exception::operator=(Exception&& o) noexcept
{
  if (this == &o) return *this;
  m_sMessage = exchange(o.m_sMessage, DxsT(""));
  return *this;
}

const TString& Exception::Message() const
{
  return m_sMessage;
}

