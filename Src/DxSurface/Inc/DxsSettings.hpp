#ifndef _DXSURFACE_DXSSETTINGS_H_
#define _DXSURFACE_DXSSETTINGS_H_



#define DxsTRUE     1
#define DxsFALSE    0


#if defined(NDEBUG) || defined(_NDEBUG)
     /* Release version */
#    define DxsReleaseBuild
#    undef  DxsDebugBuild
#else
     /* Debug version */
#    define DxsDebugBuild
#    undef  DxsReleaseBuild
#endif


#if defined(_UNICODE) || defined(UNICODE)
     /* Unicode version */
#    define DxsUnicodeBuild
#    undef  DxsMultiByteBuild
#    define __DxsVersionSuffix  "_UC"
#else
     /* Multi-byte version */
#    define DxsMultiByteBuild
#    undef  DxsUnicodeBuild
#    define __DxsVersionSuffix  "_MB"
#endif


#ifdef DxsDebugBuild
     /* Enabling/disabling debug traces */
#    define DxsGraphicsDebugEnabled          DxsTRUE
#    define DxsKeyboardDebugEnabled          DxsFALSE
#    define DxsMouseDebugEnabled             DxsFALSE
#    define DxsWindowsMessagesDebugEnabled   DxsFALSE
#endif


#ifndef DxsT
#  ifdef DxsUnicodeBuild
#    define DxsT(str_literal) L##str_literal
#  else
#    define DxsT(str_literal) str_literal
#  endif
#endif


#define DxsVersion                        0x00'00'00'00ul
#define DxsVersionString                  DxsT("0.0.0.0" __DxsVersionSuffix)
#define DxsVersionReleaseDateString       DxsT("FEB/21/2024")


#define DxsDefaultThreadRefreshRateHz     30
#define DxsTimingModelSleep               0 /* Allow sleep when execution iteration completes earlier than time limit */
#define DxsTimingModelNoSleep             1 /* Run iteration loops as fast as underlying processor allows */
#ifndef DxsTimingModel
#  define DxsTimingModel  DxsTimingModelSleep
#endif



#endif
