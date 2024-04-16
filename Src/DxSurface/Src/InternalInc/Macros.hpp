#ifndef _DXSURFACE_DXSMACROS_H_
#define _DXSURFACE_DXSMACROS_H_


#include "DxsSettings.hpp"


#define ExceptionMessageLinePrefix       "=>    "


#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
/* ---------------------------------------------------------- */
/* Debug version(s)                                           */
/* ---------------------------------------------------------- */
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
/* ---------------------------------------------------------- */
/* Release version(s)                                         */
/* ---------------------------------------------------------- */
#  define DxDeviceCreationFlags 0

   // Should have "PtrUnique<DxgiDebugInterface> m_pDebugIface" defined within
   // the invocation context.
#  define DxCall(call,fail_msg) {   \
            HRESULT hr = call;      \
            if (hr != S_OK) {       \
              DxsThrowGraphicsHr(fail_msg DxsT(". Switch to debug build with 'DxsGraphicsDebugEnabled' set to 'DxsTRUE' in the Settings to get detailed information."), hr); \
            }}

#endif


#endif
