#ifndef _DXSURFACE_DXSWINAPI_H_
#define _DXSURFACE_DXSWINAPI_H_



//- Setting Windows version flags before using Windows API:
//-    0x0400 -> Windows NT 4.0
//-    0x0500 -> Windows 2000
//-    0x0501 -> Windows XP
//-    0x0502 -> Windows Server 2003
//-    0x0600 -> Windows Vista
//-    0x0600 -> Windows Vista
//-    0x0600 -> Windows Server 2008
//-    0x0600 -> Windows Vista
//-    0x0601 -> Windows 7
//-    0x0602 -> Windows 8
//-    0x0603 -> Windows 8.1
//-    0x0A00 -> Windows 10
//-    0x0A00 -> Windows 10
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>


#ifndef NO_WIN_DISABLING
#  define NOATOM
#  define NOCLIPBOARD
#  define NOCTLMGR
#  define NOCOLOR
#  define NOCOMM
#  define NODEFERWINDOWPOS
#  define NODRAWTEXT
#  define NOGDICAPMASKS
#  define NOHELP
#  define NOICONS
#  define NOIMAGE
#  define NOKERNEL
#  define NOKANJI
#  define NOMCX
#  define NOMEMMGR
#  define NOMENUS
#  define NOMETAFILE
#  define NOMINMAX
#  define NONLS
#  define NOOPENFILE
#  define NOPROFILER
#  define NOPROXYSTUB
#  define NORASTEROPS
#  define NORPC
#  define NOSCROLL
#  define NOSERVICE
#  define NOSOUND
#  define NOSYSMETRICS
#  define NOSYSCOMMANDS
#  define NOTAPE
#  define NOTEXTMETRIC
#  define NOWH
#  define OEMRESOURCE
#  define STRICT
#  define WIN32_LEAN_AND_MEAN
#endif


#include <Windows.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <wrl.h>



#endif
