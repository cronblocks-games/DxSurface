#include "pch.hpp"
#include "DxsExceptions.hpp"


using namespace std;
using namespace CB::DxSurface;


// -----------------------------------------------------------------------------
// Exception
// -----------------------------------------------------------------------------

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


// -----------------------------------------------------------------------------
// WindowsException
// -----------------------------------------------------------------------------

WindowsException::WindowsException(const char* file, int lineNumber, ConstTCharPtr message, HRESULT hr)
  : Exception(file, lineNumber, message), m_hr(hr) {}
WindowsException::WindowsException(const WindowsException& o)
  : Exception(o), m_hr(o.m_hr) {}
WindowsException::WindowsException(WindowsException&& o) noexcept
  : Exception(move(o))
{
  if (this == &o) return;
  m_hr = exchange(o.m_hr, 0);
}
WindowsException& WindowsException::operator=(const WindowsException& o)
{
  if (this == &o) return *this;

  Exception::operator=(o);
  m_hr = o.m_hr;

  return *this;
}
WindowsException& WindowsException::operator=(WindowsException&& o) noexcept
{
  if (this == &o) return *this;

  Exception::operator=(move(o));
  m_hr = exchange(o.m_hr, 0);

  return *this;
}

TString& WindowsException::Message() const
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss
    << "Windows Exception:" << endl
    << "    -- " << m_sProvidedMessage << endl << endl
    << "File: " << start_of_str(m_sFileName.c_str(), "Src\\DxSurface\\Src") << endl
    << "Line: " << m_iLineNumber << endl
    << "HR: " << m_hr << endl
    << "    -- ";
  
  TCharPtr winMessage = nullptr;
  DWORD winMessageLen = FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // dwFlags
    nullptr,                                    // lpSource (optional)
    m_hr,                                       // dwMessageId
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // dwLanguageId
    reinterpret_cast<LPTSTR>(&winMessage),      // lpBuffer
    0,                                          // nSize
    nullptr);                                   // va_list *Arguments

  if (winMessageLen > 0)
    ss << winMessage;
  else
    ss << "Unknown error code";

  LocalFree(winMessage);

  ss
    << endl
    << "Ver: " << hex << DxsVersion
    << " (" << DxsVersionString << ") - "
    << DxsVersionReleaseDateString;

  m_sFinalMessage = ss.str();
  return m_sFinalMessage;
}


// -----------------------------------------------------------------------------
// GraphicsException
// -----------------------------------------------------------------------------

GraphicsException::GraphicsException(const char* file, int lineNumber, ConstTCharPtr message, HRESULT hr)
  : Exception(file, lineNumber, message), m_hr(hr) {}
GraphicsException::GraphicsException(const GraphicsException& o)
  : Exception(o), m_hr(o.m_hr) {}
GraphicsException::GraphicsException(GraphicsException&& o) noexcept
  : Exception(move(o))
{
  if (this == &o) return;
  m_hr = exchange(o.m_hr, 0);
}
GraphicsException& GraphicsException::operator=(const GraphicsException& o)
{
  if (this == &o) return *this;

  Exception::operator=(o);
  m_hr = o.m_hr;

  return *this;
}
GraphicsException& GraphicsException::operator=(GraphicsException&& o) noexcept
{
  if (this == &o) return *this;

  Exception::operator=(move(o));
  m_hr = exchange(o.m_hr, 0);

  return *this;
}

TString& GraphicsException::Message() const
{
#if defined(_UNICODE) || defined(UNICODE)
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss
    << "Graphics Exception:" << endl
    << "    -- " << m_sProvidedMessage << endl << endl
    << "File: " << start_of_str(m_sFileName.c_str(), "Src\\DxSurface\\Src") << endl
    << "Line: " << m_iLineNumber << endl
    << "HR: " << m_hr << endl
    << "    -- ";

  ss << "Unknown error code";

  ss
    << endl
    << "Ver: " << hex << DxsVersion
    << " (" << DxsVersionString << ") - "
    << DxsVersionReleaseDateString;

  m_sFinalMessage = ss.str();
  return m_sFinalMessage;
}

