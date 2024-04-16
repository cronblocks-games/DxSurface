#ifndef _DXSURFACE_DXSMACROS_H_
#define _DXSURFACE_DXSMACROS_H_


#include "DxsSettings.hpp"


#define ExceptionMessageLinePrefix       "=>    "

/* ---------------------------------------------------------- */
/* Graphics calls & debugging                                 */
/* ---------------------------------------------------------- */
#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
//-------------------------------
//- Debug version(s)
//-------------------------------
#  define DxDeviceCreationFlags D3D11_CREATE_DEVICE_DEBUG

   // Should already have "DxgiDebugInterface *m_pDebugIface" defined in scope.
#  define DxCall(call,ndbg_fail_msg) {                                        \
            m_pDebugIface->Mark();                                            \
            HRESULT hr = call;                                                \
            if (hr != S_OK) {                                                 \
              DxsThrowGraphicsHr(m_pDebugIface->GetMessages().c_str(), hr);   \
            }}

#else
//-------------------------------
//- Release version(s)
//-------------------------------
#  define DxDeviceCreationFlags 0
#  define DxCall(call,fail_msg) {   \
            HRESULT hr = call;      \
            if (hr != S_OK) {       \
              DxsThrowGraphicsHr(fail_msg DxsT(". Switch to debug build with 'DxsGraphicsDebugEnabled' set to 'DxsTRUE' in the Settings to get detailed information."), hr); \
            }}

#endif

/* ---------------------------------------------------------- */
/* Default shaders                                            */
/* ---------------------------------------------------------- */
#if DxsDefaultVertexShaderEnabled == DxsTRUE
#  define DefaultVertexShader ""
#endif

#if DxsDefaultPixelShaderEnabled == DxsTRUE
#  define DefaultPixelShader ""
#endif


#endif
