#ifndef _DXSURFACE_DXSMACROS_H_
#define _DXSURFACE_DXSMACROS_H_



#ifndef DxsT
#  if defined(_UNICODE) || defined(UNICODE)
#    define DxsT(str_literal) L##str_literal
#  else
#    define DxsT(str_literal) str_literal
#  endif
#endif

#if defined(_UNICODE) || defined(UNICODE)
#  define DXSURFACE_VERSION_SIFFIX  "_UC"
#else
#  define DXSURFACE_VERSION_SIFFIX  "_MB"
#endif

#define DXSURFACE_VERSION                        0x00'00'00'00ul
#define DXSURFACE_VERSION_STRING                 DxsT("0.0.0.0" DXSURFACE_VERSION_SIFFIX)
#define DXSURFACE_VERSION_RELEASE_DATE_STRING    DxsT("FEB/21/2024")



#endif
