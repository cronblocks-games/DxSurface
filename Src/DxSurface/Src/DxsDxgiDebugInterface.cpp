#include "pch.hpp"
#include "DxsDxgiDebugInterface.hpp"

#pragma comment(lib, "dxguid.lib")

using namespace CB::DxSurface;
using namespace std;


using DbgIfcFunc = HRESULT(WINAPI*)(REFIID, void**);


DxgiDebugInterface::DxgiDebugInterface() : m_uCount(0)
{
	const HMODULE lib = LoadLibraryEx(DxsT("dxgidebug.dll"), nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (lib == nullptr)
	{
		DxsThrowWindows(DxsT("Cannot load the DXGI debug library \"dxgidebug.dll\""));
	}

	const DbgIfcFunc func = reinterpret_cast<DbgIfcFunc>(GetProcAddress(lib, "DXGIGetDebugInterface"));
	if (func == nullptr)
	{
		DxsThrowWindows(DxsT("Cannot locate DXGI debug interface provider function from \"dxgidebug.dll\""));
	}

	HRESULT hr = func(__uuidof(DxDXGIInfoQueue), &m_pInfoQueue);
	if (hr != S_OK)
	{
		DxsThrowGraphicsHr(DxsT("Failed getting interface to DXGI Info Queue"), hr);
	}
}

void DxgiDebugInterface::Mark() noexcept
{
	if (m_pInfoQueue)
	{
		m_uCount = m_pInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}
}

TString DxgiDebugInterface::GetMessages() const
{
	if (m_pInfoQueue)
	{
#ifdef DxsUnicodeBuild
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
			if (hr == S_FALSE)
			{
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
					ss << endl << endl;
				}
			}
			else
			{
				DxsThrowGraphicsHr(DxsT("Cannot get message length from DXGI Info Queue"), hr);
			}
		}

		return ss.str();
	}

	return DxsT("");
}
