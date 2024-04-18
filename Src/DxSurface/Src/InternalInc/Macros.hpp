#ifndef _DXSURFACE_DXSMACROS_H_
#define _DXSURFACE_DXSMACROS_H_


#include "DxsSettings.hpp"


#define ExceptionMessageLinePrefix       "[+]    "

/* ---------------------------------------------------------- */
/* Graphics calls & debugging                                 */
/* ---------------------------------------------------------- */
#if defined(DxsDebugBuild) && DxsGraphicsDebugEnabled == DxsTRUE
//-------------------------------
//- Debug version(s)
//-------------------------------
#  define DxDeviceCreationFlags D3D11_CREATE_DEVICE_DEBUG
#  define DxCall(func_call,ignored_fail_msg) {                                \
            m_pDebugIface->Mark();                                            \
            HRESULT hr = func_call;                                           \
            if (hr != S_OK) {                                                 \
              DxsThrowGraphicsHr(m_pDebugIface->GetMessages().c_str(), hr);   \
            }} /* Should already have "DxgiDebugInterface *m_pDebugIface" defined in scope. */

#else
//-------------------------------
//- Release version(s)
//-------------------------------
#  define DxDeviceCreationFlags 0
#  define DxCall(func_call,fail_msg) {   \
            HRESULT hr = func_call;      \
            if (hr != S_OK) {            \
              DxsThrowGraphicsHr(fail_msg DxsT(". Switch to debug build having 'DxsGraphicsDebugEnabled' set to 'DxsTRUE' in DxsSettings.hpp to get detailed information."), hr); \
            }}

#endif

/* ---------------------------------------------------------- */
/* Default shaders                                            */
/* ---------------------------------------------------------- */
#if DxsDefaultVertexShaderEnabled == DxsTRUE
#     define DefaultVertexShader \
               DxsLine("float4 main(float3 pos : Position) : SV_Position") \
               DxsLine("{") \
               DxsLine("  return float4(pos.x, pos.y, pos.z, 1.0);") \
               DxsLine("}")
#endif

#if DxsDefaultPixelShaderEnabled == DxsTRUE
#     define DefaultPixelShader \
               DxsLine("float4 main() : SV_Target") \
               DxsLine("{") \
               DxsLine("  return float4(1.0, 1.0, 1.0, 1.0);") \
               DxsLine("}")
#endif


#endif
