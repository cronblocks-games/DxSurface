#ifndef _DXSURFACE_DXSSETTINGS_H_
#define _DXSURFACE_DXSSETTINGS_H_



#ifndef DxsT
#  if defined(_UNICODE) || defined(UNICODE)
#    define DxsT(str_literal) L##str_literal
#  else
#    define DxsT(str_literal) str_literal
#  endif
#endif

#if defined(_UNICODE) || defined(UNICODE)
#  define __DxsVersionSuffix  "_UC"
#else
#  define __DxsVersionSuffix  "_MB"
#endif

#define DxsVersion                        0x00'00'00'00ul
#define DxsVersionString                  DxsT("0.0.0.0" __DxsVersionSuffix)
#define DxsVersionReleaseDateString       DxsT("FEB/21/2024")

#define DxsDefaultThreadRefreshRateHz     30

#define DxsTimingModelContinuous          0 /* Don't stop time counting when user-code is stopped by debugger */
#define DxsTimingModelDebuggable          1 /* Stop time counting when user-code is stopped by debugger */
#define DxsTimingModelNoSleep             2 /* Run at full speed */
#ifndef DxsTimingModel
#  define DxsTimingModel DxsTimingModelContinuous
#endif



#endif
