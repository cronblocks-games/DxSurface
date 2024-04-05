#include "pch.hpp"
#include "DxgiDebug.hpp"

#pragma comment(lib, "dxguid.lib")

using namespace CB::DxSurface;
using namespace std;


using IfcFunc = HRESULT(WINAPI*)(REFIID, void**);


DxgiDebug::DxgiDebug()
  : m_uCount(0)
{
  //- Let's load the library
	const HMODULE dll = LoadLibraryEx(DxsT("dxgidebug.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (dll == nullptr)
	{
		DxsThrowWindows(DxsT("Cannot load the DXGI debug library"));
	}

	const IfcFunc ifcFunc = reinterpret_cast<IfcFunc>(GetProcAddress(dll, "DXGIGetDebugInterface"));
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
	if (m_pInfoQueue)
	{
		m_uCount = m_pInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}
}

TString DxgiDebug::GetMessages() const
{
	if (m_pInfoQueue)
	{
#if defined(_UNICODE) || defined(UNICODE)
		wstringstream ss;
#else
		stringstream ss;
#endif

		HRESULT hr = 0;
		SIZE_T msgLen = 0;
		const unsigned long long currCount = m_pInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

		for (unsigned long long i = m_uCount; i < currCount; i++)
		{
			hr = m_pInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &msgLen);
			if (hr != S_OK)
			{
				DxsThrowGraphicsHr(DxsT("Cannot get message length from DXGI Info Queue"), hr);
			}

			PtrUnique<unsigned char[]> buff = make_unique<unsigned char[]>(msgLen);
			DXGI_INFO_QUEUE_MESSAGE* message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(buff.get());

			hr = m_pInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &msgLen);
			if (hr != S_OK)
			{
				DxsThrowGraphicsHr(DxsT("Cannot get message from DXGI Info Queue"), hr);
			}

			ss << message->pDescription;

			if (i < currCount - 1)
			{
				ss << endl;
			}
		}

		return ss.str();
	}

	return DxsT("");
}
