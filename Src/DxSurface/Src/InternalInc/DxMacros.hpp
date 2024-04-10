#ifndef _DXSURFACE_DXSDXMACROS_H_
#define _DXSURFACE_DXSDXMACROS_H_


#include "DxsSettings.hpp"

#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE

/* Debug version(s) */

#  define DxDeviceCreationFlags D3D11_CREATE_DEVICE_DEBUG

   // Should have "PtrUnique<DxgiDebugInterface> m_pDebugIface" defined within
   // the invocation context.
#  define DxCall(call,ndbg_fail_msg) {                                        \
            m_pDebugIface->Mark();                                            \
            HRESULT hr = call;                                                \
            if (hr != S_OK) {                                                 \
              DxsThrowGraphicsHr(m_pDebugIface->GetMessages().c_str(), hr);   \
            }}

#else

/* Release version(s) */

#  define DxDeviceCreationFlags 0

   // Should have "PtrUnique<DxgiDebugInterface> m_pDebugIface" defined within
   // the invocation context.
#  define DxCall(call,fail_msg) {   \
            HRESULT hr = call;      \
            if (hr != S_OK) {       \
              DxsThrowGraphicsHr(fail_msg DxsT(" Build debug version with debug flag enabled for more information."), hr); \
            }}

#endif


#endif
