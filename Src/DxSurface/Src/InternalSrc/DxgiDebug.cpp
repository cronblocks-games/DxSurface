#include "pch.hpp"
#include "DxgiDebug.hpp"

#pragma comment(lib, "dxguid.lib")

using namespace CB::DxSurface;


using IfcFunc = HRESULT(WINAPI*)(REFIID, void**);


DxgiDebug::DxgiDebug(bool isDebuggingEnabled)
  : m_bIsDebuggingEnabled(isDebuggingEnabled)
{
  m_uCount = 0;

  //- No need to load library when debugging is not enabled
  if (!m_bIsDebuggingEnabled) return;

  //- Let's load the library
	const HMODULE lib = LoadLibraryEx(DxsT("dxgidebug.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (lib == nullptr)
	{
		DxsThrowWindows(DxsT("Cannot load the DXGI debug library"));
	}

	const IfcFunc ifcFunc = reinterpret_cast<IfcFunc>(GetProcAddress(lib, "DXGIGetDebugInterface"));

	if (ifcFunc == nullptr)
	{
		DxsThrowWindows(DxsT("Cannot locate DXGI debug interface function from the loaded library"));
	}

	HRESULT hr = ifcFunc(__uuidof(DxDXGIInfoQueue), &m_pInfoQueue);
	if (hr != S_OK)
	{
		DxsThrowGraphicsHr(DxsT("Cannot get DXGI Info Queue"), hr);
	}
}

void DxgiDebug::Mark() noexcept
{
}

Vector<TString> DxgiDebug::GetMessages() const
{
  return Vector<TString>();
}
