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
  m_sFileName = file;
  m_iLineNumber = lineNumber;
  m_sProvidedMessage = message;
  m_sFinalMessage = DxsT("");
}

Exception::Exception(const Exception& o)
{
  if (this == &o) return;

  m_sFileName = o.m_sFileName;
  m_iLineNumber = o.m_iLineNumber;
  m_sProvidedMessage = o.m_sProvidedMessage;
  m_sFinalMessage = o.m_sFinalMessage;
}
Exception::Exception(Exception&& o) noexcept
{
  if (this == &o) return;

  m_sFileName = move(o.m_sFileName);
  m_iLineNumber = exchange(o.m_iLineNumber, -1);
  m_sProvidedMessage = move(o.m_sProvidedMessage);
  m_sFinalMessage = move(o.m_sFinalMessage);
}
Exception& Exception::operator=(const Exception& o)
{
  if (this == &o) return *this;

  m_sFileName = o.m_sFileName;
  m_iLineNumber = o.m_iLineNumber;
  m_sProvidedMessage = o.m_sProvidedMessage;
  m_sFinalMessage = o.m_sFinalMessage;

  return *this;
}
Exception& Exception::operator=(Exception&& o) noexcept
{
  if (this == &o) return *this;

  m_sFileName = move(o.m_sFileName);
  m_iLineNumber = exchange(o.m_iLineNumber, -1);
  m_sProvidedMessage = move(o.m_sProvidedMessage);
  m_sFinalMessage = move(o.m_sFinalMessage);

  return *this;
}

TString& Exception::Message() const
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss
    << "Exception:" << endl
    << "    -- " << m_sProvidedMessage << endl << endl
    << "File: " << start_of_str(m_sFileName.c_str(), "Src\\DxSurface\\Src") << endl
    << "Line: " << m_iLineNumber << endl
    << "Ver: " << hex << DxsVersion
    << " (" << DxsVersionString << ") - "
    << DxsVersionReleaseDateString;

  m_sFinalMessage = ss.str();
  return m_sFinalMessage;
}

