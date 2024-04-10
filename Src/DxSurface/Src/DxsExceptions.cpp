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

Exception::Exception(const char* file, int lineNumber, TCCharPtr message)
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
#ifdef DxsUnicodeBuild
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

WindowsException::WindowsException(const char* file, int lineNumber, TCCharPtr message, HRESULT hr)
  : Exception(file, lineNumber, message), m_hr(hr), m_isGraphicsException(false), m_isValidGraphicsHR(false) {}
WindowsException::WindowsException(const WindowsException& o)
  : Exception(o), m_hr(o.m_hr), m_isGraphicsException(false), m_isValidGraphicsHR(false) {}
WindowsException::WindowsException(WindowsException&& o) noexcept
  : Exception(move(o))
{
  if (this == &o) return;
  m_hr = exchange(o.m_hr, 0);
  m_isGraphicsException = exchange(o.m_isGraphicsException, false);
  m_isValidGraphicsHR = exchange(o.m_isValidGraphicsHR, false);
}
WindowsException& WindowsException::operator=(const WindowsException& o)
{
  if (this == &o) return *this;

  Exception::operator=(o);
  m_hr = o.m_hr;
  m_isGraphicsException = o.m_isGraphicsException;
  m_isValidGraphicsHR = o.m_isValidGraphicsHR;

  return *this;
}
WindowsException& WindowsException::operator=(WindowsException&& o) noexcept
{
  if (this == &o) return *this;

  Exception::operator=(move(o));
  m_hr = exchange(o.m_hr, 0);
  m_isGraphicsException = exchange(o.m_isGraphicsException, false);
  m_isValidGraphicsHR = exchange(o.m_isValidGraphicsHR, false);

  return *this;
}

TString& WindowsException::Message() const
{
#ifdef DxsUnicodeBuild
  wstringstream ss;
#else
  stringstream ss;
#endif

  ss
    << (m_isGraphicsException ? "Graphics" : "Windows") << " Exception:" << endl
    << "    -- " << m_sProvidedMessage << endl << endl;

  if (!m_isGraphicsException || m_isValidGraphicsHR)
  {
    ss
      << "HRESULT: " << m_hr << endl
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
      ss << winMessage << endl << endl;
    else
      ss << "Unknown error code" << endl << endl;

    LocalFree(winMessage);
  }

  ss
    << "File: " << start_of_str(m_sFileName.c_str(), "Src\\DxSurface\\Src") << endl
    << "Line: " << m_iLineNumber << endl
    << "Ver: " << hex << DxsVersion
    << " (" << DxsVersionString << ") - "
    << DxsVersionReleaseDateString;

  m_sFinalMessage = ss.str();
  return m_sFinalMessage;
}


// -----------------------------------------------------------------------------
// GraphicsException
// -----------------------------------------------------------------------------

GraphicsException::GraphicsException(const char* file, int lineNumber, TCCharPtr message)
  : WindowsException(file, lineNumber, message, 0)
{
  m_isGraphicsException = true;
  m_isValidGraphicsHR = false;
}
GraphicsException::GraphicsException(const char* file, int lineNumber, TCCharPtr message, HRESULT hr)
  : WindowsException(file, lineNumber, message, hr)
{
  m_isGraphicsException = true;
  m_isValidGraphicsHR = true;
}
GraphicsException::GraphicsException(const GraphicsException& o)
  : WindowsException(o)
{
  m_isGraphicsException = true;
  m_isValidGraphicsHR = o.m_isValidGraphicsHR;
}
GraphicsException::GraphicsException(GraphicsException&& o) noexcept
  : WindowsException(move(o))
{
  if (this == &o) return;
  m_isGraphicsException = true;
  m_isValidGraphicsHR = exchange(o.m_isValidGraphicsHR, false);
}
GraphicsException& GraphicsException::operator=(const GraphicsException& o)
{
  if (this == &o) return *this;

  WindowsException::operator=(o);
  m_isGraphicsException = true;
  m_isValidGraphicsHR = o.m_isValidGraphicsHR;

  return *this;
}
GraphicsException& GraphicsException::operator=(GraphicsException&& o) noexcept
{
  if (this == &o) return *this;

  WindowsException::operator=(move(o));
  m_isGraphicsException = true;
  m_isValidGraphicsHR = exchange(o.m_isValidGraphicsHR, false);

  return *this;
}


