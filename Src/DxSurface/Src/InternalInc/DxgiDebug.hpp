#ifndef _DXSURFACE_DXSDXGIDEBUG_H_
#define _DXSURFACE_DXSDXGIDEBUG_H_


#include "DxsTypes.hpp"

namespace CB::DxSurface {

  class DxgiDebug {
	public:
		DxgiDebug(bool isDebuggingEnabled);
		~DxgiDebug() = default;

		DxgiDebug(const DxgiDebug&) = delete;
		DxgiDebug(DxgiDebug&&) noexcept = delete;
		DxgiDebug& operator=(const DxgiDebug&) = delete;
		DxgiDebug& operator=(DxgiDebug&&) noexcept = delete;

		void Mark() noexcept;
		Vector<TString> GetMessages() const;

	private:
		unsigned long long m_uCount;
		PtrCom<DxDXGIInfoQueue> m_pInfoQueue;
  };

} //- namespace CB::DxSurface


#endif
