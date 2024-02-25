#ifndef _DXSURFACE_DXSMACROS_H_
#define _DXSURFACE_DXSMACROS_H_



#define DXSURFACE_VERSION                        0x00'00'00'00ul
#define DXSURFACE_VERSION_STRING                 "0.0.0.0"
#define DXSURFACE_VERSION_RELEASE_DATE_STRING    "FEB/21/2024"

#ifndef T
#  if defined(_UNICODE) || defined(UNICODE)
#    define T(str_literal) L##str_literal
#  else
#    define T(str_literal) str_literal
#  endif
#endif



#endif
