#ifndef _DXSURFACE_DXSDXGIDEBUGINTERFACE_H_
#define _DXSURFACE_DXSDXGIDEBUGINTERFACE_H_


#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class DxgiDebugInterface {
	public:
		DxgiDebugInterface();
		~DxgiDebugInterface() = default;

		DxgiDebugInterface(const DxgiDebugInterface&) = delete;
		DxgiDebugInterface(DxgiDebugInterface&&) noexcept = delete;
		DxgiDebugInterface& operator=(const DxgiDebugInterface&) = delete;
		DxgiDebugInterface& operator=(DxgiDebugInterface&&) noexcept = delete;

		void Mark() noexcept;
		TString GetMessages() const;

	private:
		unsigned long long m_uCount;
		PtrCom<DxDXGIInfoQueue> m_pInfoQueue;
  };

} //- namespace CB::DxSurface


#endif
