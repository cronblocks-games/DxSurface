#ifndef _DXSURFACE_DXSMACROS_H_
#define _DXSURFACE_DXSMACROS_H_



#ifndef DxsT
#  if defined(_UNICODE) || defined(UNICODE)
#    define DxsT(str_literal) L##str_literal
#  else
#    define DxsT(str_literal) str_literal
#  endif
#endif



#endif
